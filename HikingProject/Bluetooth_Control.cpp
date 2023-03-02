#include "Bluetooth_Control.h"

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void Send_Hikingdata_Bluetooth()
{

  /*struct Hiking_Data{
  uint32_t  Step;
  float     Distance;
  long      Time_inSecond;
  uint8_t   Calories;
  bool      Hiking_Active; 
};*/
          /*byte bytes_step[4];
          memcpy(bytes_step, &current_data.Step, 4);

          byte bytes_distance[4];
          memcpy(bytes_distance, &current_data.Distance, 4);

          byte bytes_time[4];
          memcpy(bytes_time, &current_data.Time_inSecond, 4);
          
          byte bytes_calories[1];
          bytes_calories[0] = current_data.Calories;
          
          SerialBT.write(bytes_step, 4);

          SerialBT.write(bytes_distance, 4);

          SerialBT.write(bytes_time, 4);

          SerialBT.write(bytes_calories, 1);*/

          String data = String(current_data.Step) + "," + String(current_data.Distance) + "," + String(current_data.Time_inSecond) + "," + String(current_data.Calories);

          SerialBT.print(data);
          
          /*SerialBT.print(",");
          SerialBT.print(current_data.Distance);
          SerialBT.print(",");
          SerialBT.print(current_data.Time_inSecond);
          SerialBT.print(",");
          SerialBT.print(current_data.Calories);
          SerialBT.print(";");*/


          //delay(1000);

}

void Bluetooth_Initialize()
{
    SerialBT.begin("LilyGoGA"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
}
