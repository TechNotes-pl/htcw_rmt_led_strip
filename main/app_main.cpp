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
#define LED_STRIP_GPIO_NUMBER 8
#define LED_STRIP_LED_NUMBERS 1

static const char *TAG = "demo";

extern "C" void app_main(void)
{
    const uint8_t red = 0xFF;
    const uint8_t green = 0;
    const uint8_t blue = 0;

    ws2812 fls(LED_STRIP_GPIO_NUMBER, LED_STRIP_LED_NUMBERS);
    bool res = fls.initialize();
    printf("Initialization %s\n", res ? "OK" : "failed!!!");

    fls.set_color(0, red, green, blue);
    fls.update();

    printf("Led strip turned on for 1 second, color value: 0x%06lX\n", fls.get_color(0));
    vTaskDelay(pdMS_TO_TICKS(1000));

    const uint8_t min_level = 0;
    const uint8_t max_level = 0xFF;
    uint8_t level = 0;
    bool direction = level >= min_level;

    while (1)
    {
        ESP_LOGI(TAG, "Start demo loop");

        for (int i = 0; i < LED_STRIP_LED_NUMBERS; i++) {
            fls.set_color(i, level, level, level);
        }
        /* Refresh the strip to send data */
        fls.update();

        if(direction)
            level++;
        else
            level--;

        if(level >= max_level || level <= min_level)
            level = !level;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}