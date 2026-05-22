#include "st7305.h"

static const char *TAG = "ST7305_2p13";

ST7305_LCD::ST7305_LCD(gpio_num_t dc, gpio_num_t rst, gpio_num_t cs,
                       gpio_num_t sclk, gpio_num_t mosi, spi_host_device_t host,
                       int spi_clock_hz)
    : DC_PIN(dc),
      RES_PIN(rst),
      CS_PIN(cs),
      SCLK_PIN(sclk),
      SDIN_PIN(mosi),
      spi_host_(host),
      spi_clock_hz_(spi_clock_hz) {
    display_buffer =
        (uint8_t *)heap_caps_malloc(DISPLAY_BUFFER_LENGTH, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
    if (display_buffer) {
        memset(display_buffer, 0x00, DISPLAY_BUFFER_LENGTH);
    }
}

ST7305_LCD::~ST7305_LCD() {
    if (spi_dev_) {
        spi_bus_remove_device(spi_dev_);
        spi_dev_ = nullptr;
    }
    spi_bus_free(spi_host_);
    if (display_buffer) {
        free(display_buffer);
        display_buffer = nullptr;
    }
}

void ST7305_LCD::initialize() {
    gpio_config_t io = {};
    io.mode = GPIO_MODE_OUTPUT;
    io.pin_bit_mask = (1ULL << DC_PIN) | (1ULL << RES_PIN);
    io.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io.pull_up_en = GPIO_PULLUP_DISABLE;
    io.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&io));

    spi_bus_config_t buscfg = {};
    buscfg.sclk_io_num = SCLK_PIN;
    buscfg.mosi_io_num = SDIN_PIN;
    buscfg.miso_io_num = -1;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = DISPLAY_BUFFER_LENGTH + 16;
    ESP_ERROR_CHECK(spi_bus_initialize(spi_host_, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t dev = {};
    dev.clock_speed_hz = spi_clock_hz_;
    dev.mode = 0;
    dev.spics_io_num = CS_PIN;
    dev.queue_size = 4;
    dev.flags = SPI_DEVICE_NO_DUMMY;
    ESP_ERROR_CHECK(spi_bus_add_device(spi_host_, &dev, &spi_dev_));

    gpio_set_level(RES_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(RES_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(RES_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(10));

    Initial_ST7305();
    fill(0x00);
    ESP_LOGI(TAG, "init %dx%d MOSI=%d SCLK=%d CS=%d DC=%d RES=%d",
             LCD_WIDTH, LCD_HIGH, SDIN_PIN, SCLK_PIN, CS_PIN, DC_PIN, RES_PIN);
}

void ST7305_LCD::fill(uint8_t data) {
    if (!display_buffer) {
        return;
    }
    memset(display_buffer, data, DISPLAY_BUFFER_LENGTH);
}

void ST7305_LCD::clearDisplay() {
    memset(display_buffer, 0x00, DISPLAY_BUFFER_LENGTH);
}

void ST7305_LCD::writePoint(uint16_t x, uint16_t y, bool enabled) {
    if (!display_buffer || x >= LCD_WIDTH || y >= LCD_HIGH) {
        return;
    }

    x += COL_OFFSET;
    uint16_t real_x = x / 4;
    uint16_t real_y = y / 2;
    uint32_t write_byte_index = (uint32_t)real_y * LCD_DATA_WIDTH + real_x;

    uint8_t one_two = (y % 2 == 0) ? 0 : 1;
    uint8_t line_bit_4 = x % 4;
    uint8_t write_bit = 7 - (line_bit_4 * 2 + one_two);

    if (enabled) {
        display_buffer[write_byte_index] |= (1 << write_bit);
    } else {
        display_buffer[write_byte_index] &= ~(1 << write_bit);
    }
}

void ST7305_LCD::writePoint(uint16_t x, uint16_t y, uint16_t data) {
    writePoint(x, y, data != 0);
}

esp_err_t ST7305_LCD::write_cmd(uint8_t cmd) {
    gpio_set_level(DC_PIN, 0);
    spi_transaction_t t = {};
    t.length = 8;
    t.tx_buffer = &cmd;
    return spi_device_polling_transmit(spi_dev_, &t);
}

esp_err_t ST7305_LCD::write_param(uint8_t p) {
    gpio_set_level(DC_PIN, 1);
    spi_transaction_t t = {};
    t.length = 8;
    t.tx_buffer = &p;
    return spi_device_polling_transmit(spi_dev_, &t);
}

esp_err_t ST7305_LCD::write_data(const uint8_t *data, size_t len) {
    gpio_set_level(DC_PIN, 1);
    spi_transaction_t t = {};
    t.length = len * 8;
    t.tx_buffer = data;
    return spi_device_polling_transmit(spi_dev_, &t);
}

void ST7305_LCD::Initial_ST7305() {
    // 2.13 寸专用（examples/ST7305_2p13_BW_DisplayDriver.cpp），非 219/154 那套
    write_cmd(0xD6);
    write_param(0x17);
    write_param(0x02);
    write_cmd(0xD1);
    write_param(0x01);

    write_cmd(0xC0);
    write_param(0x0E);
    write_param(0x05);
    write_cmd(0xC1);
    write_param(0x3C);
    write_param(0x3E);
    write_param(0x3C);
    write_param(0x3C);
    write_cmd(0xC2);
    write_param(0x23);
    write_param(0x21);
    write_param(0x23);
    write_param(0x23);
    write_cmd(0xC4);
    write_param(0x5A);
    write_param(0x5C);
    write_param(0x5A);
    write_param(0x5A);
    write_cmd(0xC5);
    write_param(0x37);
    write_param(0x35);
    write_param(0x37);
    write_param(0x37);

    write_cmd(0xD8);
    write_param(0xA6);
    write_param(0xE9);
    write_cmd(0xB2);
    write_param(0x15);

    write_cmd(0xB3);
    const uint8_t b3[] = {0xE5, 0xF6, 0x17, 0x77, 0x77, 0x77,
                          0x77, 0x77, 0x77, 0x71};
    write_data(b3, sizeof(b3));
    write_cmd(0xB4);
    const uint8_t b4[] = {0x05, 0x46, 0x77, 0x77, 0x77, 0x77, 0x76, 0x45};
    write_data(b4, sizeof(b4));
    write_cmd(0x62);
    const uint8_t gtim[] = {0x32, 0x03, 0x1F};
    write_data(gtim, sizeof(gtim));
    write_cmd(0xB7);
    write_param(0x13);
    write_cmd(0xB0);
    write_param(0x3F);

    write_cmd(0x11);
    vTaskDelay(pdMS_TO_TICKS(120));

    write_cmd(0xC9);
    write_param(0x00);
    write_cmd(0x36);
    write_param(0x48);
    write_cmd(0x3A);
    write_param(0x11);
    write_cmd(0xB9);
    write_param(0x20);
    write_cmd(0xB8);
    write_param(0x29);

    write_cmd(0x2A);
    write_param(0x19);
    write_param(0x23);
    write_cmd(0x2B);
    write_param(0x00);
    write_param(0x7C);

    write_cmd(0x35);
    write_param(0x00);
    write_cmd(0xD0);
    write_param(0xFF);
    write_cmd(0x38);

    HPM_MODE = true;
    LPM_MODE = false;

    write_cmd(0x29);
    write_cmd(0x20);
    write_cmd(0xBB);
    write_param(0x4F);
}

void ST7305_LCD::Low_Power_Mode() {
    if (LPM_MODE) {
        HPM_MODE = false;
        LPM_MODE = true;
    } else {
        HPM_MODE = false;
        LPM_MODE = true;

        write_cmd(0xC1);
        write_param(0x3C);
        write_param(0x3E);
        write_param(0x3C);
        write_param(0x3C);
        write_cmd(0xC2);
        write_param(0x23);
        write_param(0x21);
        write_param(0x23);
        write_param(0x23);
        write_cmd(0xC4);
        write_param(0x5A);
        write_param(0x5C);
        write_param(0x5A);
        write_param(0x5A);
        write_cmd(0xC5);
        write_param(0x37);
        write_param(0x35);
        write_param(0x37);
        write_param(0x37);
        write_cmd(0xC9);
        write_param(0x00);

        vTaskDelay(pdMS_TO_TICKS(20));
        write_cmd(0x39);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void ST7305_LCD::High_Power_Mode() {
    if (HPM_MODE) {
        HPM_MODE = true;
        LPM_MODE = false;
    } else {
        HPM_MODE = true;
        LPM_MODE = false;

        write_cmd(0x38);
        vTaskDelay(pdMS_TO_TICKS(300));

        write_cmd(0xC1);
        write_param(0x3C);
        write_param(0x3E);
        write_param(0x3C);
        write_param(0x3C);
        write_cmd(0xC2);
        write_param(0x23);
        write_param(0x21);
        write_param(0x23);
        write_param(0x23);
        write_cmd(0xC4);
        write_param(0x5A);
        write_param(0x5C);
        write_param(0x5A);
        write_param(0x5A);
        write_cmd(0xC5);
        write_param(0x37);
        write_param(0x35);
        write_param(0x37);
        write_param(0x37);
        write_cmd(0xC9);
        write_param(0x00);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void ST7305_LCD::display_on(bool enabled) {
    write_cmd(enabled ? 0x29 : 0x28);
}

void ST7305_LCD::display_sleep(bool enabled) {
    if (enabled) {
        if (LPM_MODE) {
            write_cmd(0x38);
            vTaskDelay(pdMS_TO_TICKS(300));
        }
        write_cmd(0x10);
        vTaskDelay(pdMS_TO_TICKS(100));
    } else {
        write_cmd(0x11);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void ST7305_LCD::display_Inversion(bool enabled) {
    write_cmd(enabled ? 0x21 : 0x20);
}

void ST7305_LCD::address() {
    write_cmd(0x2A);
    write_param(0x19);
    write_param(0x23);
    write_cmd(0x2B);
    write_param(0x00);
    write_param(0x7C);
    write_cmd(0x2C);
}

void ST7305_LCD::display() {
    address();
    gpio_set_level(DC_PIN, 1);
    spi_transaction_t t = {};
    t.length = DISPLAY_BUFFER_LENGTH * 8;
    t.tx_buffer = display_buffer;
    ESP_ERROR_CHECK(spi_device_polling_transmit(spi_dev_, &t));
}

void ST7305_LCD::loadBuffer(const uint8_t *src, size_t len) {
    if (!src || !display_buffer || len != (size_t)DISPLAY_BUFFER_LENGTH) {
        return;
    }
    memcpy(display_buffer, src, DISPLAY_BUFFER_LENGTH);
}

void ST7305_LCD::DisplayImageAt(const uint8_t *src, int imgW, int imgH, int x, int y) {
    if (!src || !display_buffer) {
        return;
    }

    int src_stride_blocks = imgW / 4;
    int dst_block_x = (x + COL_OFFSET) / 4;
    int dst_pair_y = y / 2;

    int copy_blocks = std::min(src_stride_blocks, LCD_DATA_WIDTH - dst_block_x);
    int copy_pairs = std::min(imgH / 2, (LCD_HIGH / 2) - dst_pair_y);
    if (copy_blocks <= 0 || copy_pairs <= 0) {
        return;
    }

    const uint8_t *src_row = src;
    for (int rp = 0; rp < copy_pairs; ++rp) {
        uint32_t dst_idx = (uint32_t)(dst_pair_y + rp) * LCD_DATA_WIDTH + dst_block_x;
        memcpy(&display_buffer[dst_idx], src_row, (size_t)copy_blocks);
        src_row += src_stride_blocks;
    }
}

void ST7305_LCD::FillRectAt(int imgW, int imgH, int x, int y, bool is_black) {
    if (!display_buffer || imgW <= 0 || imgH <= 0) {
        return;
    }

    int dst_block_x = (x + COL_OFFSET) / 4;
    int dst_pair_y = y / 2;
    int want_blocks = imgW / 4;
    int want_pairs = imgH / 2;

    int fill_blocks = std::min(want_blocks, LCD_DATA_WIDTH - dst_block_x);
    int fill_pairs = std::min(want_pairs, (LCD_HIGH / 2) - dst_pair_y);
    if (fill_blocks <= 0 || fill_pairs <= 0) {
        return;
    }

    const uint8_t byte_val = is_black ? 0xFF : 0x00;
    for (int rp = 0; rp < fill_pairs; ++rp) {
        uint32_t dst_idx = (uint32_t)(dst_pair_y + rp) * LCD_DATA_WIDTH + dst_block_x;
        memset(&display_buffer[dst_idx], byte_val, (size_t)fill_blocks);
    }
}
