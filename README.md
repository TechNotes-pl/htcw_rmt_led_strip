# rmt_led_strip

This component allows you to control  **WS2812** / **SK6812** / **APA106**  LED strips, or any combination of the above, as long as the total is __8__ or fewer.

It uses the RMT hardware of the ESP32 so keeping the LEDs refreshed requires no CPU cycles. This makes it a very efficient choice for driving neopixel LEDs from an ESP32.

The library works in Arduino or the ESP-IDF.

Be careful to choose a different interrupt and channel for each successive strip.

## Usage

1. Include header file and declare a namespace:
```CPP
include "rmt_led_strip.hpp"

/* Namespase name for Esspresif ESP32 IDF is "esp_idf"
 * For Arduino IDE use different namespace name "arduino" */
using namespace esp_idf
```

2. Create an object based on the class depending on the type of LED strip you are using:
* `ws2812`
* `sk6812`
* `apa106`
  
Pass the relevant information like the GPIO number to the constructor.

3. Call `initialize()` method.

4. Use `color()` accessor to change ledstrip color; you can retrieve the colors using ...

5. Use `update()` method to submitt changes. Changes will take place once 'update()' is called.
