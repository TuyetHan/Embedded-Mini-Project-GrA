from bluepy.btle import Peripheral, UUID

# MAC address del dispositivo ESP32
mac_address = "08:3A:F2:69:A9:F6"

# UUID del servicio y característica que deseas leer
service_uuid = UUID("00001101-0000-1000-8000-00805f9b34fb")
char_uuid = UUID("00001101-0000-1000-8000-00805f9b34fb")

# Conectarse al dispositivo ESP32
peripheral = Peripheral(mac_address)

# Obtener el servicio y la característica deseada
service = peripheral.getServiceByUUID(service_uuid)
char = service.getCharacteristics(char_uuid)[0]

# Leer los 4 datos numéricos
data = char.read()
value1 = int.from_bytes(data[0:2], byteorder="little")
value2 = int.from_bytes(data[2:4], byteorder="little")
value3 = int.from_bytes(data[4:6], byteorder="little")
value4 = int.from_bytes(data[6:8], byteorder="little")

# Imprimir los 4 datos numéricos
print("Valor 1: ", value1)
print("Valor 2: ", value2)
print("Valor 3: ", value3)
print("Valor 4: ", value4)

# Desconectarse del dispositivo ESP32
peripheral.disconnect()