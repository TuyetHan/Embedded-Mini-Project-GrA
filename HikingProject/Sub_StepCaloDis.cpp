#include "Sub_StepCaloDis.h"

bool irq = false;
BMA *sensor; 
time_t prev_time;

void StepCounter_Initialize()
{
    //Receive objects for easy writing
    sensor = watch->bma;
    
    // Accel parameter structure
    Acfg cfg;
    /*!
        Output data rate in Hz, Optional parameters:
            - BMA4_OUTPUT_DATA_RATE_0_78HZ
            - BMA4_OUTPUT_DATA_RATE_1_56HZ
            - BMA4_OUTPUT_DATA_RATE_3_12HZ
            - BMA4_OUTPUT_DATA_RATE_6_25HZ
            - BMA4_OUTPUT_DATA_RATE_12_5HZ
            - BMA4_OUTPUT_DATA_RATE_25HZ
            - BMA4_OUTPUT_DATA_RATE_50HZ
            - BMA4_OUTPUT_DATA_RATE_100HZ
            - BMA4_OUTPUT_DATA_RATE_200HZ
            - BMA4_OUTPUT_DATA_RATE_400HZ
            - BMA4_OUTPUT_DATA_RATE_800HZ
            - BMA4_OUTPUT_DATA_RATE_1600HZ
    */
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    /*!
        G-range, Optional parameters:
            - BMA4_ACCEL_RANGE_2G
            - BMA4_ACCEL_RANGE_4G
            - BMA4_ACCEL_RANGE_8G
            - BMA4_ACCEL_RANGE_16G
    */
    cfg.range = BMA4_ACCEL_RANGE_2G;
    /*!
        Bandwidth parameter, determines filter configuration, Optional parameters:
            - BMA4_ACCEL_OSR4_AVG1
            - BMA4_ACCEL_OSR2_AVG2
            - BMA4_ACCEL_NORMAL_AVG4
            - BMA4_ACCEL_CIC_AVG8
            - BMA4_ACCEL_RES_AVG16
            - BMA4_ACCEL_RES_AVG32
            - BMA4_ACCEL_RES_AVG64
            - BMA4_ACCEL_RES_AVG128
    */
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

    /*! Filter performance mode , Optional parameters:
        - BMA4_CIC_AVG_MODE
        - BMA4_CONTINUOUS_MODE
    */
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;

    // Configure the BMA423 accelerometer
    sensor->accelConfig(cfg);

    // Enable BMA423 accelerometer
    // Warning : Need to use steps, you must first enable the accelerometer
    // Warning : Need to use steps, you must first enable the accelerometer
    // Warning : Need to use steps, you must first enable the accelerometer
    sensor->enableAccel();

    pinMode(BMA423_INT1, INPUT);
    attachInterrupt(BMA423_INT1, [] {
        // Set interrupt to set irq value to 1
        irq = 1;
    }, RISING); //It must be a rising edge
  
    // Enable BMA423 step count feature
    sensor->enableFeature(BMA423_STEP_CNTR, true);

    // Reset steps
    sensor->resetStepCounter();

    // Turn on step interrupt
    sensor->enableStepCountInterrupt();
}

void Step_Distance_Count()
{
    if (irq) {
        irq = 0;
        bool  rlst;
        do {
            // Read the BMA423 interrupt status,
            // need to wait for it to return to true before continuing
            rlst =  sensor->readInterrupt();
        } while (!rlst);

        // Check if it is a step interrupt
        if (sensor->isStepCounter()) {
            //Only count if in Hiking Section
            if(current_data.Hiking_Active == true)
            {
                // Get step data from register and calculate distance based on step
                current_data.Step = sensor->getCounter();
                if (current_data.Step%10 == 0){current_data.Distance = (current_data.Step/10)*SPACE_PER_10STEP;}
                
                //Call function to update data to screen
                updateStepCount();
                updateDistance();
            }
        }
    }
}

void HikingTime_Calories_Count(){

  if(current_data.Hiking_Active == true)
  {
      time_t now = time(0);
      double dif_seconds = difftime(now,prev_time);
      current_data.Time_inSecond += dif_seconds;
      prev_time = now;
      
      // Calculate Calories
      current_data.Calories = current_data.Time_inSecond * KCAL_PER_SECOND;

      //Call function to update data to screen
      updateHikingTime();
      updateCalories();
  }
}

void Reset_Hiking_Data(){
      prev_time = time(0);
      sensor->resetStepCounter();
      
      current_data.Time_inSecond = 0;
      current_data.Distance = 0;
      current_data.Step = 0;
      current_data.id = time(0);
}
