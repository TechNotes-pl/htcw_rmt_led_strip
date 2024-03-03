#pragma once

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
    private:
        ws2812(const ws2812 &rhs) = delete;
        ws2812 &operator=(const ws2812 &rhs) = delete;
        void do_move(ws2812 &rhs);

    public:
        /**
         * @brief Construct a new ws2812::ws2812 object
         *
         * @param pin GPIO port number
         * @param length number of light points in the led strip
         * @param rmt_channel channel number, default is 0
         * @param rmt_interrupt interrupt number, default is 23
         */
        ws2812(uint8_t pin, size_t length, uint8_t rmt_channel = 0, uint8_t rmt_interrupt = 23) : 
                led_strip(pin, length, rmt_channel, rmt_interrupt)
        { }

        //ws2812(ws2812 &&rhs) { do_move(rhs); }

        virtual ~ws2812() { deinitialize(); }

        ws2812 &operator=(ws2812 &&rhs);

    private:
        virtual rmt_ticks_t get_timings() override;
    };
}