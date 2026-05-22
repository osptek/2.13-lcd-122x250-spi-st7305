#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "st7305.h"
#include "test_1.h"

static const char *TAG = "bringup";

static ST7305_LCD lcd(DC_PIN, RES_PIN, CS_PIN, SCLK_PIN, SDIN_PIN, SPI2_HOST);

static void AppInit() {
    lcd.initialize();
    lcd.High_Power_Mode();
    lcd.display_on(true);
    lcd.display_Inversion(false);
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "ST7305 2.13 show test_1.png (%dx%d)", test_1_w, test_1_h);

    AppInit();

    lcd.loadBuffer(test_1, test_1_len);
    lcd.display();

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
