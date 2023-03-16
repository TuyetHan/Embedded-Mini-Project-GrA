import bluetooth

# Dirección MAC del dispositivo ESP32
esp32_mac = '08:3A:F2:69:A9:F6'

# UUID del servicio que deseas utilizar
service_uuid = '00001101-0000-1000-8000-00805f9b34fb'

connected = False

if __name__ == "__main__":
    # Conecta con el dispositivo ESP32 y el servicio deseado
    sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    sock.connect((esp32_mac, 1))
    sock.settimeout(2)
    connected = True
    sock.send(service_uuid)
    print("Connected to Watch!")

    # Recibe el dato numérico
    data = sock.recv(1024)
    print(data)
    #num = int.from_bytes(data, byteorder='big')

    # Imprime el dato numérico
    #print(num)

    # Cierra la conexión
    sock.close()
