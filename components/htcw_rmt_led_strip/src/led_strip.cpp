#include "led_strip.h"
#include "rmt_encoder.h"
#include <stddef.h>
#include <inttypes.h>
#include <driver/rmt_tx.h>
#include <cstring>    // for memset

using namespace esp_idf;

bool led_strip::initialize()
{
    if (m_strip != nullptr)
    {
        return true;
    }

    if (m_length == 0)
    {
        return false;
    }

    rmt_ticks_t timings = get_timings();

    m_encoder = nullptr;
    m_channel = nullptr;
    if (m_length == 0)
    {
        m_strip = nullptr;
        return false;
    }

    m_strip = malloc(m_length * 3);
    if (m_strip == nullptr)
    {
        return false;
    }
    memset(m_strip, 0, m_length * 3);

    rmt_channel_handle_t led_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config;
    memset(&tx_chan_config, 0, sizeof(rmt_tx_channel_config_t));
    tx_chan_config.clk_src = RMT_CLK_SRC_DEFAULT; // select source clock
    tx_chan_config.gpio_num = (gpio_num_t)m_pin;
    tx_chan_config.mem_block_symbols = 64; // increase the block size can make the LED less flickering
    tx_chan_config.resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ;
    tx_chan_config.trans_queue_depth = 4; // set the number of transactions that can be pending in the background

    if (ESP_OK != rmt_new_tx_channel(&tx_chan_config, &led_chan))
    {
        free(m_strip);
        m_strip = nullptr;
        return false;
    }

    rmt_encoder_handle_t led_encoder = NULL;
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ
        };

    if (ESP_OK != rmt_new_led_strip_encoder(&encoder_config, 
        timings.bit_0_low, timings.bit_0_high, timings.bit_1_low, timings.bit_1_high, 
        &led_encoder))
    {
        free(m_strip);
        m_strip = nullptr;
        rmt_del_channel(led_chan);
        return false;
    }

    if (ESP_OK != rmt_enable(led_chan))
    {
        free(m_strip);
        m_strip = nullptr;
        rmt_del_led_strip_encoder(led_encoder);
        rmt_del_channel(led_chan);
    }

    m_encoder = led_encoder;
    m_channel = led_chan;

    return true;
}


void led_strip::do_move(led_strip &rhs)
{
    m_pin = rhs.m_pin;
    m_length = rhs.m_length;
    rhs.m_length = 0;
    m_rmt_channel = rhs.m_rmt_channel;
    m_rmt_interrupt = rhs.m_rmt_channel;
    m_strip = rhs.m_strip;
    rhs.m_strip = nullptr;
    m_encoder = rhs.m_encoder;
    rhs.m_encoder = nullptr;
    m_channel = rhs.m_channel;
    rhs.m_channel = nullptr;
}

bool led_strip::initialized() const
{
    return m_strip != nullptr;
}

void led_strip::deinitialize() {
    if (m_strip == nullptr) {
        return;
    }
    rmt_del_led_strip_encoder((rmt_encoder_t*)m_encoder);
    rmt_del_channel((rmt_channel_handle_t)m_channel);

    free(m_strip);
    m_strip = nullptr;
}
void led_strip::update() {
    if (m_strip == nullptr) {
        return;
    }

    rmt_transmit_config_t tx_config;
    tx_config.flags.eot_level = 0;
    tx_config.loop_count = 0;
    rmt_tx_wait_all_done((rmt_channel_handle_t)m_channel, portMAX_DELAY);
    rmt_transmit((rmt_channel_handle_t)m_channel, (rmt_encoder_handle_t)m_encoder, m_strip, m_length * 3, &tx_config);
}

/*
uint32_t led_strip::color(size_t index) const
{
    if (m_strip == nullptr || index >= m_length)
    {
        return 0;
    }
    size_t i = index * 3;
    return (((uint8_t *)m_strip)[i + 1] << 16) |
           (((uint8_t *)m_strip)[i + 0] << 8) |
           (((uint8_t *)m_strip)[i + 2]);
}

void led_strip::color(size_t index, uint32_t value)
{
    if (m_strip == nullptr || index >= m_length)
    {
        return;
    }

    int i = index * 3;
    ((uint8_t *)m_strip)[i + 1] = (value & 0xFF0000) >> 16;
    ((uint8_t *)m_strip)[i + 0] = (value & 0x00FF00) >> 8;
    ((uint8_t *)m_strip)[i + 2] = (value & 0x0000FF);
}

void led_strip::color(size_t index, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t value = (r << 16) | (g << 8) | b;
    color(index, value);
}

void led_strip::color(size_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    uint32_t value = (w << 24) | (r << 16) | (g << 8) | b;
    color(index, value);
}

*/