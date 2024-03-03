#pragma once
#include <stddef.h>
#include <inttypes.h>
#include "strip_base.h"

namespace esp_idf
{
    class color: virtual public strip_base
    {
    public:
        uint32_t get_color(size_t index) const;
        void set_color(size_t index, uint32_t color);
        void set_color(size_t index, uint8_t red, uint8_t green, uint8_t blue);
        void set_color(size_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
    };
}