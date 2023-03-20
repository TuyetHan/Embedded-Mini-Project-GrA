#ifndef CONFIG_H
#define CONFIG_H
#include "Arduino.h"

//Hardware select
#define LILYGO_WATCH_2020_V2                 // To use T-Watch2020 V2, please uncomment this line
#define LILYGO_WATCH_LVGL                   //To use LVGL, you need to enable the macro LVGL

#include <LilyGoWatch.h>

#define KCAL_PER_SECOND         0.1225
#define SPACE_PER_10STEP        0.0045
#define DEFAULT_SCREEN_TIMEOUT  30*1000

struct Hiking_Data{
  uint32_t   id;
  uint32_t  Step;
  float     Distance;
  long      Time_inSecond;
  uint8_t   Calories;
  bool      Hiking_Active;
};
extern Hiking_Data current_data;

extern TTGOClass *watch;

#endif
