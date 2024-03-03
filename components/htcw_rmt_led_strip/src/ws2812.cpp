/**
 * @file ws2812.cpp
 * @brief Class implements
 *
 */
#include "ws2812.h"
#include "rmt_encoder.h"

using namespace esp_idf;

ws2812 &ws2812::operator=(ws2812 &&rhs)
{
    deinitialize();
    do_move(rhs);
    return *this;
}

void ws2812::do_move(ws2812 &rhs)
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

rmt_ticks_t ws2812::get_timings()
{
    return {
        .bit_0_high = LED_STRIP_RMT_TICKS_BIT_0_HIGH_WS2812,
        .bit_0_low = LED_STRIP_RMT_TICKS_BIT_0_LOW_WS2812,
        .bit_1_high = LED_STRIP_RMT_TICKS_BIT_1_HIGH_WS2812,
        .bit_1_low = LED_STRIP_RMT_TICKS_BIT_1_LOW_WS2812,
    };
}