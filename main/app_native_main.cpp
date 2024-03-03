#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"
#include "esp_err.h"
#include "app_native_config.h"

/* GPIO assignment, use GPIO_NUM_8 for internal led at esp32c6 SoC  */
#define LED_STRIP_GPIO_NUMBER GPIO_NUM_8
#define LED_STRIP_LED_NUMBERS 1

static const char *TAG = "demo";

extern "C" void app_main(void)
{
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
}