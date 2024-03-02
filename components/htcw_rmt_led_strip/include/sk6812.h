#pragma once

#include <esp_idf_version.h>
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
        sk6812(const sk6812 &rhs) = delete;
        sk6812 &operator=(const sk6812 &rhs) = delete;
        void do_move(sk6812 &rhs);

    public:
        sk6812(uint8_t pin, size_t length, uint8_t rmt_channel = 0, uint8_t rmt_interrupt = 23);
        sk6812(sk6812 &&rhs);
        virtual ~sk6812();
        sk6812 &operator=(sk6812 &&rhs);
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