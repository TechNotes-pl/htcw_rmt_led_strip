#pragma once

#include <led_strip_base.h>

namespace esp_idf
{
    typedef struct _rmt_ticks
    {
        uint8_t bit_0_high;
        uint8_t bit_0_low;
        uint8_t bit_1_high;
        uint8_t bit_1_low;
    } rmt_ticks_t;

    class led_strip : public led_strip_base
    {
    protected:
        uint8_t m_pin;
        size_t m_length;
        uint8_t m_rmt_channel;
        uint8_t m_rmt_interrupt;
        void *m_strip;
        void *m_encoder;
        void *m_channel;

    public:
        led_strip(uint8_t pin, size_t length, uint8_t rmt_channel, uint8_t rmt_interrupt);
        virtual bool initialize() override;

        virtual size_t length() const override;
        virtual bool initialized() const override;
        virtual void deinitialize() override;
        virtual void update() override;

        virtual uint32_t color(size_t index) const override;
        virtual void color(size_t index, uint32_t color) override;
        virtual void color(size_t index, uint8_t red, uint8_t green, uint8_t blue) override;
        virtual void color(size_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) override;

    private:
        virtual rmt_ticks_t get_timings() = 0;
    };
}