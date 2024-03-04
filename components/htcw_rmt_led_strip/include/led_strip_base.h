#pragma once

#include "strip_base.h"

namespace esp_idf
{
    class led_strip_base : virtual public strip_base
    {
    public: 
        led_strip_base()
        {
            ESP_LOGD(DTAG, "led_strip_base() constructor called, m_length = %d", m_length);
        }

        led_strip_base(size_t length)
        { 
            m_length = length;
            ESP_LOGD(DTAG, "led_strip_base(size_t length) constructor called, m_length = %d", m_length);
        }

        virtual esp_err_t initialize() = 0;
        virtual void deinitialize() = 0;
        /**
         * @brief Refresh the strip to send data
         */
        virtual void update() = 0;
    };
}