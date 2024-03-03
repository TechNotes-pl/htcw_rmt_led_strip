#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <driver/rmt_tx.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_check.h"
#include "esp_err.h"

namespace esp_idf
{
    // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
    #define RMT_LED_STRIP_RESOLUTION_HZ 10000000

    typedef struct
    {
        uint32_t resolution; /*!< Encoder resolution, in Hz */
    } led_strip_encoder_config_t;

    typedef struct
    {
        rmt_encoder_t base;
        rmt_encoder_t *bytes_encoder;
        rmt_encoder_t *copy_encoder;
        int state;
        rmt_symbol_word_t reset_code;
    } rmt_led_strip_encoder_t;

    size_t rmt_encode_led_strip(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, 
                            size_t data_size, rmt_encode_state_t *ret_state);
    esp_err_t rmt_del_led_strip_encoder(rmt_encoder_t *encoder);
    esp_err_t rmt_led_strip_encoder_reset(rmt_encoder_t *encoder);
    esp_err_t rmt_new_led_strip_encoder(const led_strip_encoder_config_t *config, 
                                    size_t ticks_lo_bit0, size_t ticks_hi_bit0, 
                                    size_t ticks_lo_bit1, size_t ticks_hi_bit1, 
                                    rmt_encoder_handle_t *ret_encoder);
}