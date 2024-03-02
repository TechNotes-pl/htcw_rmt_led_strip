#include <cstring>    // for memset
#include "led_strip_utils.h"
#include "esp_log.h"

using namespace esp_idf;

static const char* TAG = "demo-utils";

size_t esp_idf::rmt_encode_led_strip(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, 
                            size_t data_size, rmt_encode_state_t *ret_state)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_handle_t bytes_encoder = led_encoder->bytes_encoder;
    rmt_encoder_handle_t copy_encoder = led_encoder->copy_encoder;
    rmt_encode_state_t session_state = (rmt_encode_state_t)0;
    rmt_encode_state_t state = (rmt_encode_state_t)0;
    size_t encoded_symbols = 0;

    switch (led_encoder->state)
    {
    case 0: // send RGB data
        encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, primary_data, data_size, &session_state);
        if (session_state & RMT_ENCODING_COMPLETE)
        {
            led_encoder->state = 1; // switch to next state when current encoding session finished
        }
        if (session_state & RMT_ENCODING_MEM_FULL)
        {
            state = (rmt_encode_state_t)(((uint32_t)state) | RMT_ENCODING_MEM_FULL);
            goto out; // yield if there's no free space for encoding artifacts
        }
    // fall-through
    case 1: // send reset code
        encoded_symbols += copy_encoder->encode(copy_encoder, channel, &led_encoder->reset_code,
                                                sizeof(led_encoder->reset_code), &session_state);
        if (session_state & RMT_ENCODING_COMPLETE)
        {
            led_encoder->state = 0; // back to the initial encoding session
            state = (rmt_encode_state_t)(((uint32_t)state) | RMT_ENCODING_COMPLETE);
        }
        if (session_state & RMT_ENCODING_MEM_FULL)
        {
            state = (rmt_encode_state_t)(((uint32_t)state) | RMT_ENCODING_MEM_FULL);
            goto out; // yield if there's no free space for encoding artifacts
        }
    }
out:
    *ret_state = state;
    return encoded_symbols;
}

esp_err_t esp_idf::rmt_del_led_strip_encoder(rmt_encoder_t *encoder)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_del_encoder(led_encoder->bytes_encoder);
    rmt_del_encoder(led_encoder->copy_encoder);
    free(led_encoder);
    return ESP_OK;
}

esp_err_t esp_idf::rmt_led_strip_encoder_reset(rmt_encoder_t *encoder)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_reset(led_encoder->bytes_encoder);
    rmt_encoder_reset(led_encoder->copy_encoder);
    led_encoder->state = 0;
    return ESP_OK;
}

esp_err_t esp_idf::rmt_new_led_strip_encoder(const led_strip_encoder_config_t *config, 
                                    size_t ticks_lo_bit0, size_t ticks_hi_bit0, size_t ticks_lo_bit1, size_t ticks_hi_bit1, 
                                    rmt_encoder_handle_t *ret_encoder)
{
    esp_err_t ret = ESP_OK;
    rmt_led_strip_encoder_t *led_encoder = NULL;
    rmt_bytes_encoder_config_t bytes_encoder_config;
    rmt_copy_encoder_config_t copy_encoder_config = {};
    // rmt_symbol_word_t reset_code;
    uint32_t reset_ticks;
    ESP_GOTO_ON_FALSE(config && ret_encoder, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");
    led_encoder = (rmt_led_strip_encoder_t *)calloc(1, sizeof(rmt_led_strip_encoder_t));
    ESP_GOTO_ON_FALSE(led_encoder, ESP_ERR_NO_MEM, err, TAG, "no mem for led strip encoder");
    led_encoder->base.encode = esp_idf::rmt_encode_led_strip;
    led_encoder->base.del = esp_idf::rmt_del_led_strip_encoder;
    led_encoder->base.reset = esp_idf::rmt_led_strip_encoder_reset;
    // different led strip might have its own timing requirements, following parameter is for WS2812
    bytes_encoder_config.bit0.level0 = 1;

    bytes_encoder_config.bit0.duration0 = ((float)ticks_hi_bit0 / 10.0f) * config->resolution / 1000000; // T0H=0.3us
    bytes_encoder_config.bit0.level1 = 0;
    bytes_encoder_config.bit0.duration1 = ((float)ticks_lo_bit0 / 10.0f) * config->resolution / 1000000; // T0L=0.9us

    bytes_encoder_config.bit1.level0 = 1;
    bytes_encoder_config.bit1.duration0 = ((float)ticks_hi_bit1 / 10.0f) * config->resolution / 1000000; // T1H=0.9us
    bytes_encoder_config.bit1.level1 = 0;
    bytes_encoder_config.bit1.duration1 = ((float)ticks_lo_bit1 / 10.0f) * config->resolution / 1000000; // T1L=0.3us

    bytes_encoder_config.flags.msb_first = 1; // WS2812 transfer bit order: G7...G0R7...R0B7...B0

    ESP_GOTO_ON_ERROR(rmt_new_bytes_encoder(&bytes_encoder_config, &led_encoder->bytes_encoder), err, TAG, "create bytes encoder failed");
    ESP_GOTO_ON_ERROR(rmt_new_copy_encoder(&copy_encoder_config, &led_encoder->copy_encoder), err, TAG, "create copy encoder failed");
    reset_ticks = config->resolution / 1000000 * 50 / 2; // reset code duration defaults to 50us
    led_encoder->reset_code.level0 = 0;
    led_encoder->reset_code.duration0 = reset_ticks;
    led_encoder->reset_code.level1 = 0;
    led_encoder->reset_code.duration1 = reset_ticks;
    *ret_encoder = &led_encoder->base;
    return ESP_OK;
err:
    if (led_encoder)
    {
        if (led_encoder->bytes_encoder)
        {
            rmt_del_encoder(led_encoder->bytes_encoder);
        }
        if (led_encoder->copy_encoder)
        {
            rmt_del_encoder(led_encoder->copy_encoder);
        }
        free(led_encoder);
    }
    return ret;
}