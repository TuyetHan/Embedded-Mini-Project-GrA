#include "config.h"

#include "Sub_StepCaloDis.h"
#include "WatchDisplay.h"

#include <soc/rtc.h>
#include "esp_sleep.h"


// Global Variable
QueueHandle_t g_event_queue_handle = NULL;
EventGroupHandle_t g_event_group = NULL;

uint32_t StepCount;
float distance;

BMA *sensor;
TTGOClass *watch;
TFT_eSPI *tft;

//bool irq = false;


void setup()
{
    Serial.begin(115200);

    // Get TTGOClass instance
    watch = TTGOClass::getWatch();

    // Initialize the hardware, the BMA423 sensor has been initialized internally
    watch->begin();

    GUI_Initialize();

    // Turn on the backlight
    watch->openBL();

    //Set up BMA4 for Step Counter
    StepCounter_Initialize();
    
}


void loop()
{
    Main_GUI_Handler();
    Step_Distance_Count();
    delay(20);
}
