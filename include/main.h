#ifndef __MAIN_H__
#define __MAIN_H__

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// WiFi info
#define WIFI_SSID "serene_2G"
#define WIFI_PSK "10541054"

// uROS defines
#define UROS_APP_LOG_TAG "uros-app"
#define UROS_APP_LOG_LEVEL ESP_LOG_INFO

#define UROS_WIFI_AGENT_IP {192,168,1,12}
#define UROS_WIFI_AGENT_PORT 8888

#define UROS_NODE_NAME "esp_gnss"
#define UROS_NODE_NAMESPACE ""
#define UROS_SUB_TOPIC_NAME "/rtcm"
#define UROS_PUB_TOPIC_NAME "/esp_gnss_demo/fix"
#define UROS_PUB_TIMER_PERIOD_MS 20
 
#define UROS_SUB_RTCM_MSG_DATA_SIZE 512

#define UROS_GNSS_QUEUE_SIZE 1
#define UROS_RTCM_QUEUE_SIZE 10

#define UROS_APP_TASK_PRIORITY 5
#define UROS_APP_TASK_STACK_SIZE 4096*2

// GNSS defines

#define GNSS_HW_UART_RX 16
#define GNSS_HW_UART_TX 17
#define UROS_HW_UART_BAUDRATE 115200

#define GNSS_APP_LOG_TAG "gnss-app"
#define GNSS_APP_LOG_LEVEL ESP_LOG_INFO

#define GNSS_APP_TASK_PRIORITY 5
#define GNSS_APP_TASK_STACK_SIZE 4096*2

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

extern void main_app(); 

extern void gnss_app_init();
extern void gnss_app_start();
extern void uros_app_init();
extern void uros_app_start();

#endif // __MAIN_H__