#include "app_native_config.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "ref";

led_strip_handle_t configure_led(gpio_num_t gpio_num, uint32_t max_leds, 
                                led_model_t led_model, led_pixel_format_t led_pixel_format)
{
    // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
    const uint32_t LED_STRIP_RMT_RES_HZ = 10 * 1000 * 1000;

    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {};
    strip_config.strip_gpio_num = gpio_num;
    strip_config.max_leds = max_leds;
    strip_config.led_pixel_format = led_pixel_format;
    strip_config.led_model = led_model;
    strip_config.flags.invert_out = false; // whether to invert the output signal

    // LED strip backend configuration: RMT
    led_strip_rmt_config_t rmt_config = {};
    rmt_config.clk_src = RMT_CLK_SRC_DEFAULT;        // different clock source can lead to different power consumption
    rmt_config.resolution_hz = LED_STRIP_RMT_RES_HZ; // RMT counter clock frequency
    rmt_config.flags.with_dma = false;               // DMA feature is available on ESP target like ESP32-S3

    // LED Strip object handle
    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    ESP_LOGI(TAG, "Created LED strip object with RMT backend");
    return led_strip;
}