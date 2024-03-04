#pragma once

#include <utility>
#include "led_strip.h"

/****************************
        WS2812 Timing
 ****************************/
#define LED_STRIP_RMT_TICKS_BIT_1_HIGH_WS2812 9 // 900ns (900ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_1_LOW_WS2812 3  // 300ns (350ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_0_HIGH_WS2812 3 // 300ns (350ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_0_LOW_WS2812 9  // 900ns (900ns +/- 150ns per datasheet)

namespace esp_idf
{
    class ws2812 final : public led_strip
    {
    public:
        /**
         * @brief Construct a new object with parameters
         *
         * @param pin GPIO port number
         * @param length number of light points in the led strip
         */
        ws2812(gpio_num_t pin, size_t length) : 
            led_strip(pin, length, 0, 23)
        { }

        // Copy constructor
        ws2812(const ws2812& other) = delete;

        // Copy assignment
        ws2812& operator=(const ws2812& rhs) = delete;

        // Move constructor
        ws2812(ws2812&& other): led_strip(std::move(other))
        { }

        // Move assignment
        ws2812& operator=(ws2812&& rhs)
        {
            led_strip::operator=(std::move(rhs));
            return *this;
        }

        // Destructor
        virtual ~ws2812()
        { }

    private:
        virtual rmt_ticks_t get_timings() override;
    };
}