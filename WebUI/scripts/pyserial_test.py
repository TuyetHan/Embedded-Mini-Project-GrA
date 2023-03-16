import serial

# mac address: 08:3A:F2:69:A9:F6
# UUID: 00001101-0000-1000-8000-00805f9b34fb Serial Port
# tty1 ejecuta

if __name__ == "__main__":
    # Establecer la conexión BluetoothSerial
    ser = serial.Serial('/dev/tty1', 9600)

    print("Parece que se ha hecho la conexión")

    # Leer los datos enviados desde el dispositivo ESP32
    while True:
        print("Ejecuta el bucle de lectura")
        data = ser.read()
        #print(data.decode().strip())
        print(data)