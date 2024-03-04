#pragma once
#include "strip_base.h"

namespace esp_idf
{
    class color: virtual public strip_base
    {
    public:
        color()
        {
            ESP_LOGD(DTAG, "color() constructor called");
        }

        uint32_t get_color(size_t index) const;
        void set_color(size_t index, uint32_t color);
        void set_color(size_t index, uint8_t red, uint8_t green, uint8_t blue);
        void set_color(size_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
        
        void set_color(uint32_t color) 
        {
            for(int i=0; i<get_length(); i++) 
            {
                set_color(i, color);
            }
        }
        void set_color(uint8_t red, uint8_t green, uint8_t blue) 
        {
            for(int i=0; i<get_length(); i++) 
            {
                set_color(i, red, green, blue);
            }
        }
    };
}