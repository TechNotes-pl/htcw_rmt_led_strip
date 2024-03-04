# rmt_led_strip

This component allows you to control  **WS2812** / **SK6812** / **APA106**  LED strips, or any combination of the above, as long as the total is __8__ or fewer.

It uses the RMT hardware of the ESP32 so keeping the LEDs refreshed requires no CPU cycles. This makes it a very efficient choice for driving neopixel LEDs from an ESP32.

The component requires the ESP32 IDF version 5.0.0 and above.

Be careful to choose a different interrupt and channel for each successive strip.

## Usage

1. Include header file and declare a namespace. Namespase name for Esspresif ESP32 IDF is `esp_idf`:
```CPP
#include "ws2812.h"
using namespace esp_idf;
```

2. Create an object based on the class depending on the type of LED strip you are using:
* `ws2812`
* `sk6812`
* `apa106`

```CPP
#define LED_STRIP_GPIO_NUMBER GPIO_NUM_8
#define LED_STRIP_LED_NUMBERS 1

ws2812 fls(LED_STRIP_GPIO_NUMBER, LED_STRIP_LED_NUMBERS);
fls.initialize();
fls.set_color(0, 0xFF, 0, 0);
fls.update();
```

Pass the required information to the constructor -  GPIO number and number of light points in the stip. For the first led strip object you can use default values of __rmt channel number__ and __interrupt number__ as shown above. 

For the next ledstrips be careful to choose a different interrupt and channel for each successive strip.

3. Call `initialize()` method.

4. Use `color()` accessor to change ledstrip color; you can retrieve the colors using ...

5. Use `update()` method to submitt changes. Changes will take place once 'update()' is called.
