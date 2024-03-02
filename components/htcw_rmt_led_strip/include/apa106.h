#pragma once

#include "led_strip.h"

/****************************
        APA106 Timing
 ****************************/
#define LED_STRIP_RMT_TICKS_BIT_1_HIGH_APA106 14  // 1.36us +/- 150ns per datasheet
#define LED_STRIP_RMT_TICKS_BIT_1_LOW_APA106 3    // 350ns +/- 150ns per datasheet
#define LED_STRIP_RMT_TICKS_BIT_0_HIGH_APA106 3   // 350ns +/- 150ns per datasheet
#define LED_STRIP_RMT_TICKS_BIT_0_LOW_APA106 14   // 1.36us +/- 150ns per datasheet

namespace esp_idf
{
    class apa106 final : public led_strip
    {
    private:
        apa106(const apa106 &rhs) = delete;
        apa106 &operator=(const apa106 &rhs) = delete;
        void do_move(apa106 &rhs);

    public:
        apa106(uint8_t pin, size_t length, uint8_t rmt_channel = 0, uint8_t rmt_interrupt = 23);
        //apa106(apa106 &&rhs);
        virtual ~apa106();
        apa106 &operator=(apa106 &&rhs);

    private:
        virtual rmt_ticks_t get_timings() override;
    };
}