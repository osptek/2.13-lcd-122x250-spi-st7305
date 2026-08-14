#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 本板 2.13 寸接线（219/154 工程用 GPIO9~13，与此不同）
#define TE_PIN   GPIO_NUM_9
#define CS_PIN   GPIO_NUM_10
#define DC_PIN   GPIO_NUM_11
#define RES_PIN  GPIO_NUM_12
#define SDIN_PIN GPIO_NUM_13
#define SCLK_PIN GPIO_NUM_14

class ST7305_LCD {
public:
    ST7305_LCD(gpio_num_t dc,
               gpio_num_t rst,
               gpio_num_t cs,
               gpio_num_t sclk,
               gpio_num_t mosi,
               spi_host_device_t host = SPI2_HOST,
               int spi_clock_hz = 40 * 1000 * 1000);

    ~ST7305_LCD();

    void initialize();
    void fill(uint8_t data);
    void clearDisplay();
    void writePoint(uint16_t x, uint16_t y, bool enabled);
    void writePoint(uint16_t x, uint16_t y, uint16_t data);
    void DisplayImageAt(const uint8_t *src, int imgW, int imgH, int x, int y);
    void FillRectAt(int imgW, int imgH, int x, int y, bool is_black);
    /** 载入与 display() 一致的 4125 字节显存（可由 PNG 转换脚本生成） */
    void loadBuffer(const uint8_t *src, size_t len);
    void display();

    void Low_Power_Mode();
    void High_Power_Mode();
    void display_on(bool enabled);
    void display_sleep(bool enabled);
    void display_Inversion(bool enabled);

private:
    esp_err_t write_cmd(uint8_t cmd);
    esp_err_t write_data(const uint8_t *data, size_t len);
    esp_err_t write_param(uint8_t p);
    void address();
    void Initial_ST7305();

    static constexpr int COL_OFFSET = 10;

    const gpio_num_t DC_PIN;
    const gpio_num_t RES_PIN;
    const gpio_num_t CS_PIN;
    const gpio_num_t SCLK_PIN;
    const gpio_num_t SDIN_PIN;

    const int LCD_WIDTH = 122;
    const int LCD_HIGH = 250;
    const int LCD_DATA_WIDTH = 33;
    const int LCD_DATA_HIGH = 125;
    const int DISPLAY_BUFFER_LENGTH = 4125;

    bool HPM_MODE = true;
    bool LPM_MODE = false;

    spi_host_device_t spi_host_;
    int spi_clock_hz_;
    spi_device_handle_t spi_dev_ = nullptr;
    uint8_t *display_buffer = nullptr;
};
