#pragma once

#include <utility>
#include "led_strip.h"

/****************************
        APA106 Timing
 ****************************/
#define LED_STRIP_RMT_TICKS_BIT_1_HIGH_APA106 14 // 1.36us +/- 150ns per datasheet
#define LED_STRIP_RMT_TICKS_BIT_1_LOW_APA106 3   // 350ns +/- 150ns per datasheet
#define LED_STRIP_RMT_TICKS_BIT_0_HIGH_APA106 3  // 350ns +/- 150ns per datasheet
#define LED_STRIP_RMT_TICKS_BIT_0_LOW_APA106 14  // 1.36us +/- 150ns per datasheet

namespace esp_idf
{
    class apa106 final : public led_strip
    {
    public:
        apa106(gpio_num_t pin, size_t length) : 
            led_strip(pin, length, 0, 23)
        { }

        // Copy constructor
        apa106(const apa106& other) = delete;

        // Copy assignment
        apa106& operator=(const apa106& rhs) = delete;

        // Move constructor
        apa106(apa106&& other): led_strip(std::move(other))
        { }

        // Move assignment
        apa106& operator=(apa106&& rhs)
        {
            led_strip::operator=(std::move(rhs));
            return *this;
        }

        // Destructor
        virtual ~apa106()
        { }

    private:
        virtual rmt_ticks_t get_timings() override;
    };
}