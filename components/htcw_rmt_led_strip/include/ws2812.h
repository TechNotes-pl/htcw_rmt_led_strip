#pragma once

#include <esp_idf_version.h>
#include "led_strip.h"

/****************************
        WS2812 Timing
 ****************************/
#define LED_STRIP_RMT_TICKS_BIT_1_HIGH_WS2812 9  // 900ns (900ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_1_LOW_WS2812 3   // 300ns (350ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_0_HIGH_WS2812 3  // 300ns (350ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_0_LOW_WS2812 9   // 900ns (900ns +/- 150ns per datasheet)

namespace esp_idf
{
    class ws2812 final : public led_strip
    {
        uint8_t m_pin;
        size_t m_length;
        uint8_t m_rmt_channel;
        uint8_t m_rmt_interrupt;
        void *m_strip;
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
        void *m_rmt_items;
#else
        void *m_encoder;
        void *m_channel;
#endif
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
        ws2812(uint8_t pin, size_t length, uint8_t rmt_channel = 0, uint8_t rmt_interrupt = 23);
        ws2812(ws2812 &&rhs);
        virtual ~ws2812();

        ws2812 &operator=(ws2812 &&rhs);
        virtual bool initialized() const override;
        virtual bool initialize() override;
        virtual void deinitialize() override;
        virtual size_t length() const override;
        virtual uint32_t color(size_t index) const override;
        virtual void color(size_t index, uint32_t color) override;
        virtual void color(size_t index, uint8_t red, uint8_t green, uint8_t blue) override;
        virtual void color(size_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) override;
        virtual void update() override;
    };
}