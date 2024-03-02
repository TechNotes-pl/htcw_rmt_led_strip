#pragma once

#include <stddef.h>
#include <inttypes.h>

namespace esp_idf
{
    class led_strip_base
    {
    public:
        virtual bool initialized() const = 0;
        virtual bool initialize() = 0;
        virtual void deinitialize() = 0;
        virtual size_t length() const = 0;
        virtual uint32_t color(size_t index) const = 0;
        virtual void color(size_t index, uint32_t color) = 0;
        virtual void color(size_t index, uint8_t red, uint8_t green, uint8_t blue) = 0;
        virtual void color(size_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) = 0;
        virtual void update() = 0;
    };
}