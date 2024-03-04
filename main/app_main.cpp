#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"

#include "ws2812.h"
#include "sk6812.h"
#include "apa106.h"

using namespace esp_idf;

/* GPIO assignment, use GPIO_NUM_8 for internal led at esp32c6 SoC  */
#define LED_STRIP_GPIO_NUMBER GPIO_NUM_8
#define LED_STRIP_LED_NUMBERS 1

static const char *TAG = "demo";

extern "C" void app_main(void)
{
    /*
    TODO: test commented code. 
    Investigate why memory allocation for second tx_channel fails.
    file: led_strip.cpp, line: 47
    log:
        E (387) rmt: rmt_del_tx_channel(344): channel not in init state
        E (397) rmt: rmt_tx_register_to_group(133): no free tx channels
        E (407) rmt: rmt_new_tx_channel(252): register channel failed
    */
    // {
    //     ws2812 *fls_ptr;
    //     for (int i = 0; i < 2; i++)
    //     {
    //         gpio_num_t pin = (gpio_num_t)(i+2);
    //         ESP_LOGI(TAG, "Creating object instance %d", i);
    //         fls_ptr = new ws2812(pin, LED_STRIP_LED_NUMBERS);
    //         ESP_ERROR_CHECK(fls_ptr->initialize());
    //         delete(fls_ptr);
    //     }
    // }

   
    /* Make single flash */
    ws2812 fls(LED_STRIP_GPIO_NUMBER, LED_STRIP_LED_NUMBERS);
    ESP_ERROR_CHECK(fls.initialize());
    fls.set_color(0xFF0000);
    fls.update();
    vTaskDelay(pdMS_TO_TICKS(1000));
    fls.set_color(0);
    fls.update();
    vTaskDelay(pdMS_TO_TICKS(1000));

    /* Demo loop */
    ESP_LOGI(TAG, "Starting demo loop");
    while (1)
    {       
        const uint8_t min_level = 0;
        const uint8_t max_level = 100;

        for (int i = min_level; i < max_level; i++)
        {
            fls.set_color(i, i, i);
            fls.update();
            vTaskDelay(pdMS_TO_TICKS(4));
        }
        for (int i = max_level; i > min_level; i--)
        {
            fls.set_color(i, i, i);
            fls.update();
            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}