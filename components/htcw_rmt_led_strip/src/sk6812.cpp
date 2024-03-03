/**
 * @file sk6812.cpp
 * @brief
 *
 */
#include "sk6812.h"
#include "rmt_encoder.h"

using namespace esp_idf;

sk6812::sk6812(uint8_t pin, size_t length, uint8_t rmt_channel, uint8_t rmt_interrupt) : 
            led_strip(pin, length, rmt_channel, rmt_interrupt)
{ }

// sk6812::sk6812(sk6812&& rhs) { do_move(rhs); }

sk6812 &sk6812::operator=(sk6812 &&rhs)
{
    deinitialize();
    do_move(rhs);
    return *this;
}
sk6812::~sk6812()
{
    deinitialize();
}

void sk6812::do_move(sk6812 &rhs)
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

rmt_ticks_t sk6812::get_timings()
{
    return {
        .bit_0_high = LED_STRIP_RMT_TICKS_BIT_0_HIGH_SK6812,
        .bit_0_low = LED_STRIP_RMT_TICKS_BIT_0_LOW_SK6812,
        .bit_1_high = LED_STRIP_RMT_TICKS_BIT_1_HIGH_SK6812,
        .bit_1_low = LED_STRIP_RMT_TICKS_BIT_1_LOW_SK6812,
    };
}