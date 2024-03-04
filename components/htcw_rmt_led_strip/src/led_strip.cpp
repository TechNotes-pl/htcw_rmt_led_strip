
#include <stddef.h>
#include <inttypes.h>
#include <cstring>    // for memset
#include "led_strip.h"
#include "rmt_encoder.h"

using namespace esp_idf;

static const char *TAG = "init";

esp_err_t led_strip::initialize()
{
    esp_err_t err = ESP_OK;
    if( is_initialized() ) {
        return err;
    }  

    if(m_length == 0) {
        return ESP_ERR_INVALID_SIZE;
    }
    //assert(m_length > 0);

    m_encoder = nullptr;
    m_channel = nullptr;

    // Allocate memory for led strip points
    //
    ESP_LOGI(TAG, "Memory allocation for %d items", m_length);
    m_strip = malloc(m_length * 3);
    if (m_strip == nullptr)
    {
        return ESP_FAIL;
    }
    memset(m_strip, 0, m_length * 3);

    // Create RMT transmit channel
    //
    ESP_LOGI(TAG, "Create RMT transmit channel on GPIO_NUM_%d", m_pin);
    rmt_channel_handle_t led_chan = nullptr;
    rmt_tx_channel_config_t tx_chan_config {};
    tx_chan_config.clk_src = RMT_CLK_SRC_DEFAULT; // select source clock
    tx_chan_config.gpio_num = m_pin;
    tx_chan_config.mem_block_symbols = 64; // increase the block size can make the LED less flickering
    tx_chan_config.resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ;
    tx_chan_config.trans_queue_depth = 4; // set the number of transactions that can be pending in the background
    err = rmt_new_tx_channel(&tx_chan_config, &led_chan);
    if (ESP_OK != err)
    {
        free(m_strip);
        m_strip = nullptr;
        return err;
    }

    // Create RMT encoder
    //
    ESP_LOGI(TAG, "Create RMT encoder");
    rmt_encoder_handle_t led_encoder = NULL;
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ
        };

    rmt_ticks_t timings = get_timings();
    err = rmt_new_led_strip_encoder(&encoder_config, 
        timings.bit_0_low, timings.bit_0_high, timings.bit_1_low, timings.bit_1_high, 
        &led_encoder);
    if (ESP_OK != err)
    {
        free(m_strip);
        m_strip = nullptr;
        rmt_del_channel(led_chan);
        return err;
    }

    // Enable RMT
    //
    ESP_LOGI(TAG, "Enable RMT");
    err = rmt_enable(led_chan);
    if (ESP_OK != err)
    {
        free(m_strip);
        m_strip = nullptr;
        rmt_del_led_strip_encoder(led_encoder);
        rmt_del_channel(led_chan);
    }

    m_encoder = led_encoder;
    m_channel = led_chan;

    return err;
}

void led_strip::deinitialize()
{
    if (is_initialized())
    {
        rmt_del_led_strip_encoder(m_encoder);
        rmt_del_channel(m_channel);

        free(m_strip);
        m_strip = nullptr;
    }
}

void led_strip::update()
{
    if (is_initialized())
    {
        rmt_transmit_config_t tx_config{};
        rmt_tx_wait_all_done(m_channel, portMAX_DELAY);
        rmt_transmit(m_channel, m_encoder, m_strip, m_length * 3, &tx_config);
    }
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