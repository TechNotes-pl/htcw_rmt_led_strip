#pragma once
#include <stddef.h>
#include <inttypes.h>
#include "esp_log.h"

#define DTAG "demo"

namespace esp_idf
{
    /**
     * @brief 
     * 
     */
    class strip_base
    {
    public:
        strip_base() : 
            m_strip{}, m_length{}
        { 
            ESP_LOGD(DTAG, "strip_base() constructor called, reseting m_lenght (%d) and m_strip (%p)", m_length, m_strip);
        }

        size_t get_length() const { 
            return m_length; 
        }

        void set_length(size_t len) { 
            m_length = len; 
        }

        /**
         * @brief Check if the index is correct, i.e. there is a light point with this number
         * 
         * @param index light point number
         * @return true 
         * @return false 
         */
        bool exists(size_t index) const
        {
            return is_initialized() && index < m_length;
        }

        /**
         * @brief Check if the LED strip has been initialized
         * 
         * @return true 
         * @return false 
         */
        bool is_initialized() const
        {
            return m_strip != nullptr;
        }

    protected:
        void *m_strip;
        size_t m_length;
        
    };
}