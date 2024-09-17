#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_intr_alloc.h>

#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "main.h"


void hw_init()
{
    esp_log_level_set(MAIN_APP_LOG_TAG, MAIN_APP_LOG_LEVEL);

    // Init I2C for GNSS
    GNSS_HW_WIRE.setPins(GNSS_HW_I2C_SDA, GNSS_HW_I2C_SCL);
    GNSS_HW_WIRE.begin();

    // Init WiFi

    // start the Access Point mode
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PSK);
    ESP_LOGI(MAIN_APP_LOG_TAG, "AP IP address: %s", WiFi.softAPIP().toString().c_str());

    // start the Station mode
    WiFi.begin((char *)WIFI_STA_SSID, (char *)WIFI_STA_PSK);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      ESP_LOGI(MAIN_APP_LOG_TAG, "Connecting to WiFi...");
    }
    ESP_LOGI(MAIN_APP_LOG_TAG, "Connected to WiFi");

}

void main_app()
{
    hw_init();
    uros_app_init();
    gnss_app_init();

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    uros_app_start();
    gnss_app_start();
    
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}