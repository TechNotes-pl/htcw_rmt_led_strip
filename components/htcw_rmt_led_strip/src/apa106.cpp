/**
 * @file apa106.cpp
 * @brief
 *
 */
#include "apa106.h"
#include "rmt_encoder.h"

using namespace esp_idf;

apa106::apa106(uint8_t pin, size_t length, uint8_t rmt_channel, uint8_t rmt_interrupt) : 
                led_strip(pin, length, rmt_channel, rmt_interrupt)
{ }

// apa106::apa106(apa106&& rhs) { do_move(rhs); }

apa106 &apa106::operator=(apa106 &&rhs)
{
    deinitialize();
    do_move(rhs);
    return *this;
}

apa106::~apa106()
{
    deinitialize();
}

void apa106::do_move(apa106 &rhs)
{
    m_pin = rhs.m_pin;
    m_length = rhs.m_length;
    rhs.m_length = 0;
    m_rmt_channel = rhs.m_rmt_channel;
    m_rmt_interrupt = rhs.m_rmt_channel;
    m_strip = rhs.m_strip;
    rhs.m_strip = nullptr;
    m_encoder = rhs.m_encoder;
    rhs.m_encoder = nullptr;
    m_channel = rhs.m_channel;
    rhs.m_channel = nullptr;
}

rmt_ticks_t apa106::get_timings()
{
    return {
        .bit_0_high = LED_STRIP_RMT_TICKS_BIT_0_HIGH_APA106,
        .bit_0_low = LED_STRIP_RMT_TICKS_BIT_0_LOW_APA106,
        .bit_1_high = LED_STRIP_RMT_TICKS_BIT_1_HIGH_APA106,
        .bit_1_low = LED_STRIP_RMT_TICKS_BIT_1_LOW_APA106,
    };
}