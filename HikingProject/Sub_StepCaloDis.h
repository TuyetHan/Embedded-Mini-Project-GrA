#ifndef SUB_STEPCALODIS_H
#define SUB_STEPCALODIS_H
#include <Arduino.h>
#include "config.h"
#include "WatchDisplay.h"

#define SPACE_PER_10STEP 0.0045

extern BMA *sensor;
extern TFT_eSPI *tft;
extern TTGOClass *watch;
extern bool irq;

extern uint32_t StepCount;
extern float distance;

void StepCounter_Initialize();
void Step_Distance_Count();

#endif
