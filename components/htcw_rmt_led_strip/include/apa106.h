#pragma once

#include <esp_idf_version.h>
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
        apa106(const apa106 &rhs) = delete;
        apa106 &operator=(const apa106 &rhs) = delete;
        void do_move(apa106 &rhs);

    public:
        apa106(uint8_t pin, size_t length, uint8_t rmt_channel = 0, uint8_t rmt_interrupt = 23);
        apa106(apa106 &&rhs);
        virtual ~apa106();
        apa106 &operator=(apa106 &&rhs);
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