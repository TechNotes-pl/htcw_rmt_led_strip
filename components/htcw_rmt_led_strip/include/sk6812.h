#pragma once

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
    private:
        sk6812(const sk6812 &rhs) = delete;
        sk6812 &operator=(const sk6812 &rhs) = delete;
        void do_move(sk6812 &rhs);

    public:
        sk6812(uint8_t pin, size_t length, uint8_t rmt_channel = 0, uint8_t rmt_interrupt = 23);
        //sk6812(sk6812 &&rhs);
        virtual ~sk6812();
        sk6812 &operator=(sk6812 &&rhs);

    private:
        virtual rmt_ticks_t get_timings() override;
    };
}