#pragma once

#include <stddef.h>
#include <inttypes.h>
#include "strip_base.h"

namespace esp_idf
{
    class led_strip_base : virtual public strip_base
    {
    public: 
        virtual bool initialize() = 0;
        virtual void deinitialize() = 0;
        virtual void update() = 0;
        
        virtual bool initialized() const = 0;
    };
}