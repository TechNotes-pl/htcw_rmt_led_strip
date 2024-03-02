/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"
#include "esp_err.h"

#define APP_DEMO

#ifdef APP_DEMO
#include "ws2812.h"
#include "sk6812.h"
#include "apa106.h"
using namespace esp_idf;
#else
#include "app_reference.h"
#endif

/* GPIO assignment */
#define LED_STRIP_GPIO_NUMBER GPIO_NUM_8
/* Numbers of the LED in the strip, max 180 (3m WS2812B 5V) */
#define LED_STRIP_LED_NUMBERS 1

static const char *TAG = "demo";

extern "C" void app_main(void)
{
    #ifdef APP_DEMO

    const uint8_t red = 0xFF;
    const uint8_t green = 0;
    const uint8_t blue = 0;

    ws2812 fls(LED_STRIP_GPIO_NUMBER, LED_STRIP_LED_NUMBERS);
    fls.initialize();
    fls.color(0, red, green, blue);
    fls.update();
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        ESP_LOGI(TAG, "Start demo loop");

        for (uint8_t level = 0; level < 0xFF; level++)
        {
            for (int i = 0; i < LED_STRIP_LED_NUMBERS; i++)
            {
                fls.color(i, level, level, level);
            }
            /* Refresh the strip to send data */
            fls.update();
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }

    #else

    const uint16_t max_hue = 360;
    const uint8_t saturation = 255;
    const uint8_t value = 128;
    led_strip_handle_t led_strip = configure_led(LED_STRIP_GPIO_NUMBER, LED_STRIP_LED_NUMBERS);

    while (1)
    {
        ESP_LOGI(TAG, "Start reference loop");

        for (uint16_t hue = 0; hue < max_hue; hue++)
        {
            /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
            for (int i = 0; i < LED_STRIP_LED_NUMBERS; i++)
            {
                ESP_ERROR_CHECK(led_strip_set_pixel_hsv(led_strip, i, hue, saturation, value));
            }
            /* Refresh the strip to send data */
            ESP_ERROR_CHECK(led_strip_refresh(led_strip));
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
    #endif
}