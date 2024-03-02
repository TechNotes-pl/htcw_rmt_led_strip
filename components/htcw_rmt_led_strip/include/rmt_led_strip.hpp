#pragma once

#ifndef ESP_PLATFORM
#error "This library requires an ESP32"
#endif

#if __has_include(<Arduino.h>)
#include <Arduino.h>
#endif

#include "ws2812.h"
#include "sk6812.h"
#include "apa106.h"