#pragma once
#include <stddef.h>
#include <inttypes.h>

namespace esp_idf
{
    class strip_base
    {
    public:
        strip_base(void *strip, size_t len) : m_strip{strip}, m_length{len}
        { }
        strip_base() : m_strip{}, m_length{}
        { }

        size_t get_length() const { return m_length; }
        void set_length(size_t len) { m_length = len; }

    protected:
        void *m_strip;
        size_t m_length;
        
    };
}