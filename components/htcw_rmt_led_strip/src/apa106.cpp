/**
 * @file apa106.cpp
 * @brief 
 *
 */
#include "apa106.h"
#include "led_strip_utils.h"

using namespace esp_idf;

apa106::apa106(uint8_t pin, size_t length, uint8_t rmt_channel, uint8_t rmt_interrupt) : m_pin(pin),
                                                                                         m_length(length),
                                                                                         m_rmt_channel(rmt_channel),
                                                                                         m_rmt_interrupt(rmt_interrupt),
                                                                                         m_strip(nullptr),
                                                                                         m_encoder(nullptr),
                                                                                         m_channel(nullptr){
}

apa106::apa106(apa106&& rhs) {
    do_move(rhs);
}

apa106& apa106::operator=(apa106&& rhs) {
    deinitialize();
    do_move(rhs);
    return *this;
}
apa106::~apa106() {
    deinitialize();
}
void apa106::do_move(apa106& rhs) {
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

size_t apa106::length() const {
    return m_length;
}
bool apa106::initialized() const {
    return m_strip != nullptr;
}
bool apa106::initialize() {
    if (m_strip != nullptr) {
        return true;
    }
    if (m_length == 0) {
        return false;
    }

    return led_strip_initialize(m_pin, m_length,
                                m_rmt_channel,
                                m_rmt_interrupt,
                                LED_STRIP_RMT_TICKS_BIT_0_LOW_APA106,
                                LED_STRIP_RMT_TICKS_BIT_0_HIGH_APA106,
                                LED_STRIP_RMT_TICKS_BIT_1_LOW_APA106,
                                LED_STRIP_RMT_TICKS_BIT_1_HIGH_APA106,
                                &m_strip,
                                &m_encoder,
                                &m_channel);
}
void apa106::deinitialize() {
    if (m_strip == nullptr) {
        return;
    }
    rmt_del_led_strip_encoder((rmt_encoder_t*)m_encoder);
    rmt_del_channel((rmt_channel_handle_t)m_channel);

    free(m_strip);
    m_strip = nullptr;
}
void apa106::update() {
    if (m_strip == nullptr) {
        return;
    }

    rmt_transmit_config_t tx_config;
    tx_config.flags.eot_level = 0;
    tx_config.loop_count = 0;
    rmt_tx_wait_all_done((rmt_channel_handle_t)m_channel, portMAX_DELAY);
    rmt_transmit((rmt_channel_handle_t)m_channel, (rmt_encoder_handle_t)m_encoder, m_strip, m_length * 3, &tx_config);
}

uint32_t apa106::color(size_t index) const {
    if (m_strip == nullptr || index >= m_length) {
        return 0;
    }
    size_t i = index * 3;
    return (((uint8_t*)m_strip)[i + 1] << 16) |
           (((uint8_t*)m_strip)[i + 0] << 8) |
           (((uint8_t*)m_strip)[i + 2]);
}
void apa106::color(size_t index, uint32_t value) {
    if (m_strip == nullptr || index >= m_length) {
        return;
    }

    int i = index * 3;
    ((uint8_t*)m_strip)[i + 1] = (value & 0xFF0000) >> 16;
    ((uint8_t*)m_strip)[i + 0] = (value & 0x00FF00) >> 8;
    ((uint8_t*)m_strip)[i + 2] = (value & 0x0000FF);
}
void apa106::color(size_t index, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t value = (r << 16) | (g << 8) | b;
    color(index, value);
}
void apa106::color(size_t index, uint8_t r, uint8_t g, uint8_t b,uint8_t w) {
    uint32_t value = (w << 24) | (r << 16) | (g << 8) | b;
    color(index, value);
}
