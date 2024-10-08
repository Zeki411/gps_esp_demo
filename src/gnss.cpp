#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_intr_alloc.h>

#include <Wire.h>
#include <SparkFun_u-blox_GNSS_v3.h>

#include "main.h"

TaskHandle_t gnss_task_handle;
SFE_UBLOX_GNSS gnss_dev;
gnss_data_t gnss_data;

void gnss_init()
{
    esp_log_level_set(GNSS_LOG_TAG, GNSS_LOG_LEVEL);

    // Init GNSS
    while (gnss_dev.begin(GNSS_HW_WIRE, GNSS_HW_I2C_ADDRESS) == false) //Connect to the u-blox module using our custom port and address
    {
        ESP_LOGE(GNSS_LOG_TAG, "u-blox GNSS not detected. Retrying...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    ESP_LOGI(GNSS_LOG_TAG, "u-blox GNSS detected!");

    // Configure GNSS

    gnss_dev.setNavigationRate(1);
    gnss_dev.setMeasurementRate(100);
    // gnss_dev.setNavigationFrequency(40);
    // gnss_dev.setHNRNavigationRate(40);

    gnss_dev.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    gnss_dev.setUART1Output(0); //Turn off UART output

    ESP_LOGI(GNSS_LOG_TAG, "GNSS initialized");
}

void gnss_task(void *arg)
{
    ESP_LOGI(GNSS_LOG_TAG, "GNSS task initialized");

    int64_t last_time = esp_timer_get_time();
    uint8_t rx_cnt = 0;

    while (1)
    {
        // Check if there is any RTCM message
        rtcm_data_t rtcm_data;
        if (xQueueReceive(uros_rtcm_queue, &rtcm_data, 0) == pdTRUE)
        {
            // ESP_LOGI(GNSS_APP_LOG_TAG, "Received RTCM data length: %d", rtcm_data.size); 
            gnss_dev.pushRawData(rtcm_data.data, rtcm_data.size); // Push the RTCM data to the GNSS module
        }
        
        if (gnss_dev.getPVT() == true)
        {
            gnss_data.latitude = (double)gnss_dev.getLatitude() / 10000000.0;
            gnss_data.longitude = (double)gnss_dev.getLongitude() / 10000000.0;
            gnss_data.altitude = (double)gnss_dev.getAltitudeMSL() / 1000.0; // Altitude above Mean Sea Level

            // xQueueOverwrite(uros_gnss_queue, &gnss_data);
            xQueueSend(uros_gnss_queue, &gnss_data, 0);

            // ESP_LOGI(GNSS_APP_LOG_TAG, "Lat: %f, Long: %f, Alt: %f", gnss_data.latitude, gnss_data.longitude, gnss_data.altitude);
            if(rx_cnt == 100) // Print every 100th message
            {
                ESP_LOGI(GNSS_LOG_TAG, "Rx Rate: %f Hz", 100.0 / ((esp_timer_get_time() - last_time) / 1000000.0));
                ESP_LOGI(GNSS_LOG_TAG, "Heap Free: %d", xPortGetFreeHeapSize());
                last_time = esp_timer_get_time();
                rx_cnt = 0;
            }
            rx_cnt++;

        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void gnss_task_init()
{
    xTaskCreate(gnss_task,\
                "gnss_task",\
                GNSS_TASK_STACK_SIZE,\
                NULL,\
                GNSS_TASK_PRIORITY,\
                &gnss_task_handle);
}

