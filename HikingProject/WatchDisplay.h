#ifndef WATCHDISPLAY_H
#define WATCHDISPLAY_H

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

void updateBatteryIcon(lv_icon_battery_t index);
void updateBatteryLevel();
void updateStepCount();
void updateDistance();
void updateHikingTime();
void updateCalories();

void setupGui();
void GUI_Initialize();
void Main_GUI_Handler();

#endif /*__GUI_H */
