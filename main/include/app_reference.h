#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"

/**
 * @brief Create LED strip object with RMT backend
 * 
 * @param gpio_num The GPIO that connected to the LED strip's data line
 * @param max_leds The number of LEDs in the strip
 * @param led_model LED strip model, default LED_MODEL_WS2812
 * @param led_pixel_format Pixel format of your LED strip, default LED_PIXEL_FORMAT_GRB
 * @return led_strip_handle_t LED strip object created 
 */
led_strip_handle_t configure_led(gpio_num_t gpio_num, uint32_t max_leds, 
                    led_model_t led_model = LED_MODEL_WS2812,
                    led_pixel_format_t led_pixel_format = LED_PIXEL_FORMAT_GRB);