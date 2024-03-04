#include "apa106.h"

using namespace esp_idf;

rmt_ticks_t apa106::get_timings()
{
    return {
        .bit_0_high = LED_STRIP_RMT_TICKS_BIT_0_HIGH_APA106,
        .bit_0_low = LED_STRIP_RMT_TICKS_BIT_0_LOW_APA106,
        .bit_1_high = LED_STRIP_RMT_TICKS_BIT_1_HIGH_APA106,
        .bit_1_low = LED_STRIP_RMT_TICKS_BIT_1_LOW_APA106,
    };
}