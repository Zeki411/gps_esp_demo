#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <WiFi.h>
#include <WiFiUdp.h>

#include <Wire.h>

#include "main.h"

void wifi_init()
{
    WiFi.begin(WIFI_STA_SSID, WIFI_STA_PSK);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
}

void i2c_init()
{
    GNSS_HW_WIRE.setPins(GNSS_HW_I2C_SDA, GNSS_HW_I2C_SCL);
    GNSS_HW_WIRE.begin();
}

void system_launch()
{
    esp_log_level_set(SYSTEM_LOG_TAG, SYSTEM_LOG_LEVEL);

    wifi_init();
    i2c_init();

    uros_init();
    gnss_init();

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    uros_task_init();
    gnss_task_init();
    
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}