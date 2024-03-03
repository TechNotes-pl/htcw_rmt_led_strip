set(srcs    "src/ws2812.cpp"
            "src/sk6812.cpp"
            "src/apa106.cpp"
            "src/led_strip.cpp"
            "src/rmt_encoder.cpp"
            "src/color.cpp"
            )

idf_component_register( SRCS "${srcs}"
                        INCLUDE_DIRS "include"
                        PRIV_REQUIRES led_strip
)