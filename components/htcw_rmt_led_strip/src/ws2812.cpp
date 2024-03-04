#include "ws2812.h"

using namespace esp_idf;

rmt_ticks_t ws2812::get_timings()
{
    return {
        .bit_0_high = LED_STRIP_RMT_TICKS_BIT_0_HIGH_WS2812,
        .bit_0_low = LED_STRIP_RMT_TICKS_BIT_0_LOW_WS2812,
        .bit_1_high = LED_STRIP_RMT_TICKS_BIT_1_HIGH_WS2812,
        .bit_1_low = LED_STRIP_RMT_TICKS_BIT_1_LOW_WS2812,
    };
}