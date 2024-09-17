#ifndef __MAIN_H__
#define __MAIN_H__

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// WiFi info
#define WIFI_STA_SSID "serene_2G"
#define WIFI_STA_PSK "10541054"

// Main defines
#define SYSTEM_LOG_TAG "system"
#define SYSTEM_LOG_LEVEL ESP_LOG_INFO

// uROS defines
#define UROS_LOG_TAG "uros"
#define UROS_LOG_LEVEL ESP_LOG_INFO
// #define UROS_HW_UART_RX 16
// #define UROS_HW_UART_TX 17
// #define UROS_HW_UART_BAUDRATE 115200
#define UROS_WIFI_AGENT_IP {192,168,1,12}
#define UROS_WIFI_AGENT_PORT 8888

#define UROS_NODE_NAME "esp_gnss"
#define UROS_NODE_NAMESPACE ""
#define UROS_SUB_TOPIC_NAME "/rtcm"
#define UROS_PUB_TOPIC_NAME "/esp_gnss_demo/fix"
#define UROS_PUB_TIMER_PERIOD_MS 25 // 40 Hz

#define UROS_SUB_RTCM_MSG_DATA_SIZE 512

#define UROS_GNSS_QUEUE_SIZE 10
#define UROS_RTCM_QUEUE_SIZE 10

#define UROS_TASK_PRIORITY 5
#define UROS_TASK_STACK_SIZE 4096

// GNSS defines
#define GNSS_HW_WIRE Wire1
#define GNSS_HW_I2C_SDA 23
#define GNSS_HW_I2C_SCL 22
#define GNSS_HW_I2C_ADDRESS 0x42

#define GNSS_LOG_TAG "gnss"
#define GNSS_LOG_LEVEL ESP_LOG_INFO

#define GNSS_TASK_PRIORITY 5
#define GNSS_TASK_STACK_SIZE 4096

typedef struct
{
    double latitude;
    double longitude;
    double altitude;
} gnss_data_t;

typedef struct
{
    uint8_t data[UROS_SUB_RTCM_MSG_DATA_SIZE];
    size_t size;
} rtcm_data_t;

extern QueueHandle_t uros_gnss_queue;
extern QueueHandle_t uros_rtcm_queue;

extern void system_launch(); 

extern void gnss_init();
extern void gnss_task_init();
extern void uros_init();
extern void uros_task_init();

#endif // __MAIN_H__