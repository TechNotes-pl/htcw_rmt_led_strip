# rmt_led_strip

This library allows you to control up to 8 ws2812 strips, sk6812 strips, apa106 strips, or any combination of the above, as long as the total is 8 or fewer.

It uses the RMT hardware of the ESP32 so keeping the LEDs refreshed requires no CPU cycles. This makes it a very efficient choice for driving neopixel LEDs from an ESP32.

The library works in Arduino or the ESP-IDF.

Be careful to choose a different interrupt and channel for each successive strip.

Using it is simple.

include `<ws2812.hpp>`, `<sk6812.hpp>`, and/or `<apa106.hpp>` depending on the type of LED strip you are using.

You may then declare it, passing the relevant information like the GPIO# to the constructor.

Next call initialize().

From there you can update or retrive the colors using color() accessor.

Changes will take place once update() is called.

PIO library is codewitch-honey-crisis/htcw_rmt_led_strip
