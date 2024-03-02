#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <esp_idf_version.h>
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
#include <driver/rmt_tx.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_check.h"
typedef struct
{
    uint32_t resolution; /*!< Encoder resolution, in Hz */
} led_strip_encoder_config_t;
#else
#include <driver/rmt.h>
#endif
#include "esp_err.h"

// 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_RESOLUTION_HZ 10000000

namespace esp_idf
{
    /**
     * @brief 
     * 
     * @param pin 
     * @param length 
     * @param rmt_channel 
     * @param rmt_interrupt 
     * @param ticks_lo_bit0 
     * @param ticks_hi_bit0 
     * @param ticks_lo_bit1 
     * @param ticks_hi_bit1 
     * @param strip 
     * @param encoder 
     * @param channel 
     * @return true 
     * @return false 
     */
    bool led_strip_initialize(uint8_t pin, size_t length, uint8_t rmt_channel, uint8_t rmt_interrupt,
                              size_t ticks_lo_bit0, size_t ticks_hi_bit0, size_t ticks_lo_bit1, size_t ticks_hi_bit1,
                              void **strip, void **encoder, void **channel);

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
                                    size_t ticks_lo_bit0, size_t ticks_hi_bit0, size_t ticks_lo_bit1, size_t ticks_hi_bit1, 
                                    rmt_encoder_handle_t *ret_encoder);
}