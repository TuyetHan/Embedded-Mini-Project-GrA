#include "Bluetooth_Control.h"

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

void Send_Hikingdata_Bluetooth()
{
//      if (Serial.available()) 
//      {
          SerialBT.print("StepCount:");
          SerialBT.print(current_data.Step);
          SerialBT.print("   Distance:");
          SerialBT.print(current_data.Distance);
          SerialBT.print("   Hiking Time: ");
          SerialBT.print(current_data.Time_inSecond);
          SerialBT.print("   Calo: ");
          SerialBT.print(current_data.Calories);
//      }
}

void Bluetooth_Initialize()
{
    SerialBT.begin("ESP32test"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
}
