#pragma once

#include "led_strip_base.h"
#include "color.h"

namespace esp_idf
{
    typedef struct _rmt_ticks
    {
        uint8_t bit_0_high;
        uint8_t bit_0_low;
        uint8_t bit_1_high;
        uint8_t bit_1_low;
    } rmt_ticks_t;

    class led_strip : public led_strip_base, public color
    {
    protected:
        uint8_t m_pin;
        uint8_t m_rmt_channel;
        uint8_t m_rmt_interrupt;
        void *m_encoder;
        void *m_channel;

    public:
        led_strip(uint8_t pin, size_t length, uint8_t rmt_channel, uint8_t rmt_interrupt) : strip_base(nullptr, length), m_pin(pin),
                                                                                            m_rmt_channel(rmt_channel), m_rmt_interrupt(rmt_interrupt),
                                                                                            m_encoder{}, m_channel{} {};

        led_strip(led_strip &&rhs)
        {
            do_move(rhs);
        }

        virtual bool initialize() override;
        virtual bool initialized() const override;
        virtual void deinitialize() override;
        virtual void update() override;

    private:
        virtual rmt_ticks_t get_timings() = 0;
        void do_move(led_strip &rhs);
    };
}