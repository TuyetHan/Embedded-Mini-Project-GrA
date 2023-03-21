#include "config.h"
#include "WatchDisplay.h"
#include "Sub_StepCaloDis.h"
#include <Arduino.h>

#include <time.h>
#include <Ticker.h>

LV_FONT_DECLARE(Geometr);
LV_FONT_DECLARE(Ubuntu);
LV_IMG_DECLARE(bg);
LV_IMG_DECLARE(bg1);
LV_IMG_DECLARE(bg2);
LV_IMG_DECLARE(bg3);
LV_IMG_DECLARE(WALLPAPER_1_IMG);
LV_IMG_DECLARE(WALLPAPER_2_IMG);
LV_IMG_DECLARE(WALLPAPER_3_IMG);
LV_IMG_DECLARE(step);
LV_IMG_DECLARE(menu);

//---------------------- Define Local Variable-------------------------//
static lv_style_t settingStyle;
static lv_obj_t *mainBar = nullptr;
static lv_obj_t *timeLabel = nullptr;

static lv_obj_t *StepCount_Label;
static lv_obj_t *Distance_Label;
static lv_obj_t *Calories_Label;
static lv_obj_t *Hiking_Time_Label;

static lv_obj_t *Start_Stop_Button = nullptr;
static lv_obj_t *SS_Button_Label;

QueueHandle_t g_event_queue_handle = NULL;
EventGroupHandle_t g_event_group = NULL;

class StatusBar
{
    typedef struct {
        bool vaild;
        lv_obj_t *icon;
    } lv_status_bar_t;
public:
    StatusBar()
    {
        memset(_array, 0, sizeof(_array));
    }
    void createIcons(lv_obj_t *par)
    {
        _par = par;

        static lv_style_t barStyle;

        lv_style_init(&barStyle);
        lv_style_set_radius(&barStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_bg_color(&barStyle, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
        lv_style_set_bg_opa(&barStyle, LV_OBJ_PART_MAIN, LV_OPA_20);
        lv_style_set_border_width(&barStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_text_color(&barStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
        lv_style_set_image_recolor(&barStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);

        _bar = lv_cont_create(_par, NULL);
        lv_obj_set_size(_bar,  LV_HOR_RES, _barHeight);
        lv_obj_add_style(_bar, LV_OBJ_PART_MAIN, &barStyle);

        _array[0].icon = lv_label_create(_bar, NULL);
        lv_label_set_text(_array[0].icon, "100%");

        _array[1].icon = lv_img_create(_bar, NULL);
        lv_img_set_src(_array[1].icon, LV_SYMBOL_BATTERY_FULL);

        _array[2].icon = lv_img_create(_bar, NULL);
        lv_img_set_src(_array[2].icon, LV_SYMBOL_WIFI);
        lv_obj_set_hidden(_array[2].icon, true);

        _array[3].icon = lv_img_create(_bar, NULL);
        lv_img_set_src(_array[3].icon, LV_SYMBOL_BLUETOOTH);
        lv_obj_set_hidden(_array[3].icon, true);

        //step counter
        _array[4].icon = lv_img_create(_bar, NULL);
        lv_img_set_src(_array[4].icon, &step);
        lv_obj_align(_array[4].icon, _bar, LV_ALIGN_IN_LEFT_MID, 10, 0);

        _array[5].icon = lv_label_create(_bar, NULL);
        lv_label_set_text(_array[5].icon, "0");
        lv_obj_align(_array[5].icon, _array[4].icon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

        refresh();
    }

    void setStepCounter(uint32_t counter)
    {
        lv_label_set_text(_array[5].icon, String(counter).c_str());
        lv_obj_align(_array[5].icon, _array[4].icon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    }

    void updateLevel(int level)
    {
        lv_label_set_text(_array[0].icon, (String(level) + "%").c_str());
        refresh();
    }

    void updateBatteryIcon(lv_icon_battery_t icon)
    {
        const char *icons[6] = {LV_SYMBOL_BATTERY_EMPTY, LV_SYMBOL_BATTERY_1, LV_SYMBOL_BATTERY_2, LV_SYMBOL_BATTERY_3, LV_SYMBOL_BATTERY_FULL, LV_SYMBOL_CHARGE};
        lv_img_set_src(_array[1].icon, icons[icon]);
        refresh();
    }

    void show(lv_icon_status_bar_t icon)
    {
        lv_obj_set_hidden(_array[icon].icon, false);
        refresh();
    }

    void hidden(lv_icon_status_bar_t icon)
    {
        lv_obj_set_hidden(_array[icon].icon, true);
        refresh();
    }
    uint8_t height()
    {
        return _barHeight;
    }
    lv_obj_t *self()
    {
        return _bar;
    }
private:
    void refresh()
    {
        int prev;
        for (int i = 0; i < 4; i++) {
            if (!lv_obj_get_hidden(_array[i].icon)) {
                if (i == LV_STATUS_BAR_BATTERY_LEVEL) {
                    lv_obj_align(_array[i].icon, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
                } else {
                    lv_obj_align(_array[i].icon, _array[prev].icon, LV_ALIGN_OUT_LEFT_MID, iconOffset, 0);
                }
                prev = i;
            }
        }
    };
    lv_obj_t *_bar = nullptr;
    lv_obj_t *_par = nullptr;
    uint8_t _barHeight = 30;
    lv_status_bar_t _array[6];
    const int8_t iconOffset = -5;
};

StatusBar bar;
//---------------------- End of Local Variable-------------------------//

//-------------------- Sub Function for GUI Part-----------------------//
// Define all functions used for update watch display
static void updateTime()
{
    time_t now;
    struct tm  info;
    char buf[64];
    time(&now);
    localtime_r(&now, &info);
    strftime(buf, sizeof(buf), "%H:%M", &info);
    lv_label_set_text(timeLabel, buf);
    lv_obj_align(timeLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);
    TTGOClass *watch = TTGOClass::getWatch();
    watch->rtc->syncToRtc();
}

void updatePreviousStepCount(uint32_t counter)
{
    bar.setStepCounter(counter);
}

void updateBatteryLevel()
{
    TTGOClass *watch = TTGOClass::getWatch();
    int p = watch->power->getBattPercentage();
    bar.updateLevel(p);
}

void updateStepCount()
{
    lv_label_set_text_fmt(StepCount_Label, "Steps: %d", current_data.Step);
}

void updateDistance()
{
    lv_label_set_text_fmt(Distance_Label, "Distance: %.3f miles", current_data.Distance);
}

void updateCalories()
{
    lv_label_set_text_fmt(Calories_Label, "Calories: %d kcal", current_data.Calories);
}

void updateHikingTime()
{
    //hour    
    int hr = (current_data.Time_inSecond / ( 60 * 60)) % 24;

    // minutes
    int min = (current_data.Time_inSecond / (60)) % 60;

    // seconds
    int sec = (current_data.Time_inSecond ) % 60;

    // milliseconds
    //int mill = stopwatch_milliseconds % 1000;

    lv_label_set_text_fmt(Hiking_Time_Label, "Total Hiking Time: %02d:%02d:%02d", hr, min, sec);
}

void updateBatteryIcon(lv_icon_battery_t icon)
{
    if (icon >= LV_ICON_CALCULATION) {
        TTGOClass *watch = TTGOClass::getWatch();
        int level = watch->power->getBattPercentage();
        if (level > 95)icon = LV_ICON_BAT_FULL;
        else if (level > 80)icon = LV_ICON_BAT_3;
        else if (level > 45)icon = LV_ICON_BAT_2;
        else if (level > 20)icon = LV_ICON_BAT_1;
        else icon = LV_ICON_BAT_EMPTY;
    }
    bar.updateBatteryIcon(icon);
}

static void lv_battery_task(struct _lv_task_t *data)
{
    updateBatteryLevel();
}

// Start Stop Hiking Section Button
static void SS_button_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) 
    {
        if (lv_btn_get_state(obj) == LV_BTN_STATE_RELEASED)
          {
            //Change Button Label, change Global Variable
            lv_label_set_text(SS_Button_Label, "Stop");
            current_data.Hiking_Active = true;

            //Reset step counter,time counter assign prev-time for stopwatch
              Reset_Hiking_Data();         

            //Update display for Step and Distance
            updateStepCount();
            updateDistance();
            
          }
        else
          {
            //Change Button Label, change Global Variable
            lv_label_set_text(SS_Button_Label, "Start");
            current_data.Hiking_Active = false;
            
            //Update step to bar
            updatePreviousStepCount(current_data.Step);
            
          }  
    } 
}
//-------------------- End of Sub Function Part-------------------------//

//---------------------- Initialization Part----------------------------//
// Set up all neccessary Button, Label, Bar for Watch Display
void setupGui()
{
    lv_style_init(&settingStyle);
    lv_style_set_radius(&settingStyle, LV_OBJ_PART_MAIN, 0);
    lv_style_set_bg_color(&settingStyle, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
    lv_style_set_bg_opa(&settingStyle, LV_OBJ_PART_MAIN, LV_OPA_0);
    lv_style_set_border_width(&settingStyle, LV_OBJ_PART_MAIN, 0);
    lv_style_set_text_color(&settingStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_image_recolor(&settingStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);


    //Create wallpaper
    void *images[] = {(void *) &bg, (void *) &bg1, (void *) &bg2, (void *) &bg3 };
    lv_obj_t *scr = lv_scr_act();
    lv_obj_t *img_bin = lv_img_create(scr, NULL);  /*Create an image object*/
    srand((int)time(0));
    int r = rand() % 4;
    lv_img_set_src(img_bin, images[r]);
    lv_obj_align(img_bin, NULL, LV_ALIGN_CENTER, 0, 0);

    //! bar
    bar.createIcons(scr);
    updateBatteryLevel();
    lv_icon_battery_t icon = LV_ICON_CALCULATION;

    TTGOClass *watch = TTGOClass::getWatch();

    if (watch->power->isChargeing()) {
        icon = LV_ICON_CHARGE;
    }
    updateBatteryIcon(icon);

    //! main
    static lv_style_t mainStyle;
    lv_style_init(&mainStyle);
    lv_style_set_radius(&mainStyle, LV_OBJ_PART_MAIN, 0);
    lv_style_set_bg_color(&mainStyle, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
    lv_style_set_bg_opa(&mainStyle, LV_OBJ_PART_MAIN, LV_OPA_0);
    lv_style_set_border_width(&mainStyle, LV_OBJ_PART_MAIN, 0);
    lv_style_set_text_color(&mainStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_image_recolor(&mainStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);


    mainBar = lv_cont_create(scr, NULL);
    lv_obj_set_size(mainBar,  LV_HOR_RES, LV_VER_RES - bar.height());
    lv_obj_add_style(mainBar, LV_OBJ_PART_MAIN, &mainStyle);
    lv_obj_align(mainBar, bar.self(), LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    //! Time
    static lv_style_t timeStyle;
    lv_style_copy(&timeStyle, &mainStyle);
    lv_style_set_text_font(&timeStyle, LV_STATE_DEFAULT, &Ubuntu);

    timeLabel = lv_label_create(mainBar, NULL);
    lv_obj_add_style(timeLabel, LV_OBJ_PART_MAIN, &timeStyle);
    updateTime();

    //Step Label
    StepCount_Label = lv_label_create(mainBar, NULL);
    lv_obj_set_pos(StepCount_Label, 20, 80);
    lv_label_set_text_fmt(StepCount_Label, "Steps: %d", 0);
    

    //Distance Label
    Distance_Label = lv_label_create(mainBar, StepCount_Label);
    lv_obj_set_pos(Distance_Label, 20, 100);
    lv_label_set_text_fmt(Distance_Label, "Distance: %.3f miles", 0.000);
    
    //Calories Label
    Calories_Label = lv_label_create(mainBar, StepCount_Label);
    lv_obj_set_pos(Calories_Label, 20, 120);
    lv_label_set_text_fmt(Calories_Label, "Calories: %d kcal", 0);

    //Hiking Time Label
    Hiking_Time_Label = lv_label_create(mainBar, StepCount_Label);
    lv_obj_set_pos(Hiking_Time_Label, 20, 140);
    lv_label_set_text_fmt(Hiking_Time_Label, "Total Hiking Time: 00:00:00");
    
    //Start Stop Button
    Start_Stop_Button = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(Start_Stop_Button, SS_button_event_handler);
    lv_obj_align(Start_Stop_Button, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    
    lv_btn_set_checkable(Start_Stop_Button, true);
    lv_btn_toggle(Start_Stop_Button);
    lv_btn_set_fit2(Start_Stop_Button, LV_FIT_NONE, LV_FIT_TIGHT);

    SS_Button_Label = lv_label_create(Start_Stop_Button, NULL);
    lv_label_set_text(SS_Button_Label, "Start");
    
}

void GUI_Initialize()
{
      //Create a program that allows the required message objects and group flags
    g_event_queue_handle = xQueueCreate(20, sizeof(uint8_t));
    g_event_group = xEventGroupCreate();

    // Turn on the IRQ used
    watch->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
    watch->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
    watch->power->clearIRQ();

    //Initialize lvgl
    watch->lvgl_begin();

    // Enable BMA423 interrupt ，
    // The default interrupt configuration,
    // you need to set the acceleration parameters, please refer to the BMA423_Accel example
    watch->bma->attachInterrupt();

    //Connection interrupted to the specified pin
    pinMode(BMA423_INT1, INPUT);
    attachInterrupt(BMA423_INT1, [] {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        uint8_t data = Q_EVENT_BMA_INT;
        xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR ();
        }
    }, RISING);

    // Connection interrupted to the specified pin
    pinMode(AXP202_INT, INPUT);
    attachInterrupt(AXP202_INT, [] {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        uint8_t data = Q_EVENT_AXP_INT;
        xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR ();
        }
    }, FALLING);

    //Check if the RTC clock matches, if not, use compile time
    watch->rtc->check();

    //Synchronize time to system time
    watch->rtc->syncToSystem();

    //Execute your own GUI interface
    setupGui();

    //Clear lvgl counter
    lv_disp_trig_activity(NULL);
}
//------------------ end of Initialization Part -----------------//


//------------------ Main function of Gui Display ---------------//
// Update all Label according to real Value
void Main_GUI_Handler()
{
    uint8_t data;
    if (lv_disp_get_inactive_time(NULL) < DEFAULT_SCREEN_TIMEOUT) {
        lv_task_handler();
    } else {
        watch->startLvglTick();
        watch->rtc->syncToSystem();

        // Update Time, Battery Level, Icon        
        updateTime();
        updateBatteryLevel();
        updateBatteryIcon(LV_ICON_CALCULATION);

        // 
        lv_disp_trig_activity(NULL);
        watch->openBL();
        watch->bma->enableStepCountInterrupt();
    }
}

//------------------ End of Main Gui Display ---------------//
