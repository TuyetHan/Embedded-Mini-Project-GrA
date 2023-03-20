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

          String data = String(current_data.Step) + "," + String(current_data.Distance) + "," + String(current_data.Time_inSecond) + 
                        "," + String(current_data.Calories) + "," + String(current_data.id);

          SerialBT.print(data);


}

void Bluetooth_Initialize()
{
    SerialBT.begin("LilyGoGA"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
}
