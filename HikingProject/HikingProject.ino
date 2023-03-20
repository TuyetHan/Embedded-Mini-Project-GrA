#include "config.h"

#include "Sub_StepCaloDis.h"
#include "WatchDisplay.h"
#include "Bluetooth_Control.h"



#include <soc/rtc.h>
#include "esp_sleep.h"

// Global Variable for Hiking Related Activities 
//time_t prev_time;
Hiking_Data current_data = {0,0,0,0,0,0};

// Create variable for Step Count Sensor and Watch  
TTGOClass *watch;

void setup()
{
    Serial.begin(115200);

    // Get TTGOClass instance
    watch = TTGOClass::getWatch();

    // Initialize the hardware
    watch->begin();

    // Watch GUI initialization
    GUI_Initialize();

    // Turn on the backlight
    watch->openBL();

    //Set up BMA4 for Step Counter
    StepCounter_Initialize();

    // Set up Bluetooth
    Bluetooth_Initialize();
}

void loop()
{
    // GUI Display Process
    Main_GUI_Handler();

    // Step and Distance calculation    
    Step_Distance_Count();

    //Stop Watch time count, Calories calculate
    HikingTime_Calories_Count();

    // Send all record data to bluetooth
    Send_Hikingdata_Bluetooth();
    
    delay(100);
}
