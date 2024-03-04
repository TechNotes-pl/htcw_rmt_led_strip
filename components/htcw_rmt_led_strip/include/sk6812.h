#pragma once

#include <utility>
#include "led_strip.h"

/****************************
        SK6812 Timing
 ****************************/
#define LED_STRIP_RMT_TICKS_BIT_1_HIGH_SK6812 6
#define LED_STRIP_RMT_TICKS_BIT_1_LOW_SK6812 6
#define LED_STRIP_RMT_TICKS_BIT_0_HIGH_SK6812 3
#define LED_STRIP_RMT_TICKS_BIT_0_LOW_SK6812 9

namespace esp_idf
{
    class sk6812 final : public led_strip
    {
    public:
        sk6812(gpio_num_t pin, size_t length) : led_strip(pin, length, 0, 23)
        { }

        // Copy constructor
        sk6812(const sk6812& other) = delete;

        // Copy assignment
        sk6812& operator=(const sk6812& rhs) = delete;

        // Move constructor
        sk6812(sk6812&& other): led_strip(std::move(other))
        { }

        // Move assignment
        sk6812& operator=(sk6812&& rhs)
        {
            led_strip::operator=(std::move(rhs));
            return *this;
        }

        // Destructor
        virtual ~sk6812()
        { }

    private:
        virtual rmt_ticks_t get_timings() override;
    };
}