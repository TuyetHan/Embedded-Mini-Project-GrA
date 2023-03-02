import bluetooth

# Mac Address of the device
MAC_ADDRESS = '08:3A:F2:69:A9:F6'

# UUID of service
service_uuid = '00001101-0000-1000-8000-00805f9b34fb'

class BTConnect:
    connected = False
    sock = None

    def connect(self):

        self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        self.sock.connect((MAC_ADDRESS, 1))
        self.sock.settimeout(2)
        connected = True
        self.sock.send(service_uuid)

        return connected


    def receive_data(self):
        
        data = self.sock.recv(1024)

        data = data.decode()

        steps, distance, time, calories = data.split(",")

        retrieved_data = {}
        retrieved_data['steps'] = steps
        retrieved_data['distance'] = distance
        retrieved_data['time'] = time
        retrieved_data['calories'] = calories

        # Cierra la conexión
        self.sock.close()

        return retrieved_data