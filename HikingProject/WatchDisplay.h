/*
Copyright (c) 2019 lewis he
This is just a demonstration. Most of the functions are not implemented.
The main implementation is low-power standby. 
The off-screen standby (not deep sleep) current is about 4mA.
Select standard motherboard and standard backplane for testing.
Created by Lewis he on October 10, 2019.
*/

#ifndef __GUI_H
#define __GUI_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include <soc/rtc.h>
#include "esp_sleep.h"


typedef enum {
    LV_ICON_BAT_EMPTY,
    LV_ICON_BAT_1,
    LV_ICON_BAT_2,
    LV_ICON_BAT_3,
    LV_ICON_BAT_FULL,
    LV_ICON_CHARGE,
    LV_ICON_CALCULATION
} lv_icon_battery_t;


typedef enum {
    LV_STATUS_BAR_BATTERY_LEVEL = 0,
    LV_STATUS_BAR_BATTERY_ICON = 1,
    LV_STATUS_BAR_WIFI = 2,
    LV_STATUS_BAR_BLUETOOTH = 3,
} lv_icon_status_bar_t;

enum {
    Q_EVENT_BMA_INT,
    Q_EVENT_AXP_INT,
} ;

#define DEFAULT_SCREEN_TIMEOUT  30*1000

void setupGui();
void updateBatteryIcon(lv_icon_battery_t index);
void updateBatteryLevel();

void updateStepCount();
void updateDistance();

void GUI_Initialize();
void Main_GUI_Handler();

extern QueueHandle_t g_event_queue_handle;
extern EventGroupHandle_t g_event_group;
extern TTGOClass *watch;

extern uint32_t StepCount;
extern float distance;

#endif /*__GUI_H */
