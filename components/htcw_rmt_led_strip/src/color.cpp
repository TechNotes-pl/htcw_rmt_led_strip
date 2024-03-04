#include "color.h"

using namespace esp_idf;

uint32_t color::get_color(size_t index) const
{
    if (exists(index))
    {
        size_t i = index * 3;
        return (((uint8_t *)m_strip)[i + 1] << 16) |
               (((uint8_t *)m_strip)[i + 0] << 8) |
               (((uint8_t *)m_strip)[i + 2]);
    }
    return 0;
}

void color::set_color(size_t index, uint32_t value)
{
    if (exists(index))
    {
        int i = index * 3;
        ((uint8_t *)m_strip)[i + 1] = (value & 0xFF0000) >> 16;
        ((uint8_t *)m_strip)[i + 0] = (value & 0x00FF00) >> 8;
        ((uint8_t *)m_strip)[i + 2] = (value & 0x0000FF);
    }
}

void color::set_color(size_t index, uint8_t r, uint8_t g, uint8_t b)
{
    set_color(index, (r << 16) | (g << 8) | b);
}

void color::set_color(size_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    set_color(index, (w << 24) | (r << 16) | (g << 8) | b);
}