#pragma once

#include <driver/rmt_tx.h>
#include "led_strip_base.h"
#include "color.h"

namespace esp_idf
{
    typedef struct _rmt_ticks
    {
        uint8_t bit_0_high;
        uint8_t bit_0_low;
        uint8_t bit_1_high;
        uint8_t bit_1_low;
    } rmt_ticks_t;

    class led_strip : public led_strip_base, public color 
    {
    public:
        led_strip(gpio_num_t pin, size_t length, uint8_t rmt_channel, uint8_t rmt_interrupt) : 
            led_strip_base(length), m_pin(pin), m_rmt_channel(rmt_channel), m_rmt_interrupt(rmt_interrupt), m_encoder{}, m_channel{} 
            {
                ESP_LOGD(DTAG, "led_strip(pin, len) constructor called, m_lenght = %d", m_length);
            };

        // Copy constructor
        led_strip(const led_strip& other) = delete;

        // Copy assignment
        led_strip& operator=(const led_strip& rhs) = delete;

        // Move constructor
        led_strip(led_strip&& other)
        { 
            do_move(other); 
        }

        // Move assignment
        led_strip& operator=(led_strip&& rhs)
        {
            // self-asignment check
            if (this != &rhs) {
                // delete old data
                deinitialize();
                // move from rhs to this
                do_move(rhs);
            }
            return *this;
        }

        // Destructor must be virtual
        virtual ~led_strip() 
        { 
            ESP_LOGD(DTAG, "~led_strip() destructor called\n");
            deinitialize(); 
        }

        virtual esp_err_t initialize() override;
        virtual void deinitialize() override;
        virtual void update() override;

    protected:
        void do_move(led_strip &rhs);

    private:
        virtual rmt_ticks_t get_timings() = 0;

    protected:
        gpio_num_t m_pin;
        uint8_t m_rmt_channel;
        uint8_t m_rmt_interrupt;

        rmt_encoder_handle_t m_encoder;
        rmt_channel_handle_t m_channel;
    };
}