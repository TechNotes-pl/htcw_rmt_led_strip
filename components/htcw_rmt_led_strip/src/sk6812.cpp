/**
 * @file sk6812.cpp
 * @brief 
 *
 */
#include "sk6812.h"
#include "led_strip_utils.h"

using namespace esp_idf;

sk6812::sk6812(uint8_t pin, size_t length, uint8_t rmt_channel, uint8_t rmt_interrupt) : m_pin(pin),
                                                                                         m_length(length),
                                                                                         m_rmt_channel(rmt_channel),
                                                                                         m_rmt_interrupt(rmt_interrupt),
                                                                                         m_strip(nullptr),
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
                                                                                         m_rmt_items(nullptr) {
#else
                                                                                         m_encoder(nullptr),
                                                                                         m_channel(nullptr){
#endif
}
sk6812::sk6812(sk6812&& rhs) {
    do_move(rhs);
}
sk6812& sk6812::operator=(sk6812&& rhs) {
    deinitialize();
    do_move(rhs);
    return *this;
}
sk6812::~sk6812() {
    deinitialize();
}
void sk6812::do_move(sk6812& rhs) {
    m_pin = rhs.m_pin;
    m_length = rhs.m_length;
    rhs.m_length = 0;
    m_rmt_channel = rhs.m_rmt_channel;
    m_rmt_interrupt = rhs.m_rmt_channel;
    m_strip = rhs.m_strip;
    rhs.m_strip = nullptr;
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
    m_rmt_items = rhs.m_rmt_items;
    rhs.m_rmt_items = nullptr;
#else
    m_encoder = rhs.m_encoder;
    rhs.m_encoder = nullptr;
    m_channel = rhs.m_channel;
    rhs.m_channel = nullptr;
#endif
}

size_t sk6812::length() const {
    return m_length;
}
bool sk6812::initialized() const {
    return m_strip != nullptr;
}
bool sk6812::initialize() {
    if (m_strip != nullptr) {
        return true;
    }
    if (m_length == 0) {
        return false;
    }
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    return led_strip_initialize(m_pin, m_length,
                                m_rmt_channel,
                                m_rmt_interrupt,
                                LED_STRIP_RMT_TICKS_BIT_0_LOW_SK6812,
                                LED_STRIP_RMT_TICKS_BIT_0_HIGH_SK6812,
                                LED_STRIP_RMT_TICKS_BIT_1_LOW_SK6812,
                                LED_STRIP_RMT_TICKS_BIT_1_HIGH_SK6812,
                                &m_strip,
                                &m_encoder,
                                &m_channel);
#else
    return led_strip_initialize(m_pin, m_length, m_rmt_channel, m_rmt_interrupt, (uint32_t **)&m_strip, &m_rmt_items);
#endif
}
void sk6812::deinitialize() {
    if (m_strip == nullptr) {
        return;
    }
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    rmt_del_led_strip_encoder((rmt_encoder_t*)m_encoder);
    rmt_del_channel((rmt_channel_handle_t)m_channel);

    free(m_strip);
    m_strip = nullptr;
#else
    rmt_driver_uninstall((rmt_channel_t)m_rmt_channel);
    free(m_strip);
    free(m_rmt_items);
    m_strip = nullptr;
#endif
}
void sk6812::update() {
    if (m_strip == nullptr) {
        return;
    }
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    rmt_transmit_config_t tx_config;
    tx_config.flags.eot_level = 0;
    tx_config.loop_count = 0;
    // rmt_tx_wait_all_done((rmt_channel_handle_t)m_channel,portMAX_DELAY);
    rmt_transmit((rmt_channel_handle_t)m_channel, (rmt_encoder_handle_t)m_encoder, m_strip, m_length * 3, &tx_config);
#else
    rmt_wait_tx_done((rmt_channel_t)m_rmt_channel, portMAX_DELAY);
    led_strip_fill_rmt_items_sk6812((uint32_t *)m_strip, (rmt_item32_t *)m_rmt_items, m_length);
    rmt_write_items((rmt_channel_t)m_rmt_channel, (rmt_item32_t *)m_rmt_items, (LED_STRIP_NUM_RMT_ITEMS_PER_LED * m_length), false);
#endif
}

uint32_t sk6812::color(size_t index) const {
    if (m_strip == nullptr || index >= m_length) {
        return 0;
    }
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    size_t i = index * 3;
    return (((uint8_t*)m_strip)[i + 1] << 16) |
           (((uint8_t*)m_strip)[i + 0] << 8) |
           (((uint8_t*)m_strip)[i + 2]);
#else
    return ((uint32_t *)m_strip)[index];
#endif
}
void sk6812::color(size_t index, uint32_t value) {
    if (m_strip == nullptr || index >= m_length) {
        return;
    }
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    int i = index * 3;
    ((uint8_t*)m_strip)[i + 1] = (value & 0xFF0000) >> 16;
    ((uint8_t*)m_strip)[i + 0] = (value & 0x00FF00) >> 8;
    ((uint8_t*)m_strip)[i + 2] = (value & 0x0000FF);
#else
    ((uint32_t *)m_strip)[index] = value;
#endif
}
void sk6812::color(size_t index, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t value = (r << 16) | (g << 8) | b;
    color(index, value);
}
void sk6812::color(size_t index, uint8_t r, uint8_t g, uint8_t b,uint8_t w) {
    uint32_t value = (w << 24) | (r << 16) | (g << 8) | b;
    color(index, value);
}
