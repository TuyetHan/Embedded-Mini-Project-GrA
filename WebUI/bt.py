import bluetooth
import time

# MAC address of the device
MAC_ADDRESS = '08:3A:F2:69:A9:F6'

# UUID of service
service_uuid = '00001101-0000-1000-8000-00805f9b34fb'

class BTConnect:
    """This class manages the Bluetooth connection to the watch and
       the retrieval of data from the watch once it is connected.

       This class allows to:
        - Connect to the watch via Bluetooth (connect)
        - Retrieve and process data from the watch (receive_data)

    Attributes:
        N/A
    """

    def connect(self):
        connected = False
        # Connects with ESP32 device and the service indicated
        self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        try:
            self.sock.connect((MAC_ADDRESS, 1))
            self.sock.settimeout(2)
            connected = True
            self.sock.send(service_uuid)
            print("Connected to Watch!")
        except bluetooth.btcommon.BluetoothError:
            time.sleep(1)
        except Exception as e:
            print(e)
            print("Hub: Error occured while trying to connect to the Watch.")

        return connected


    def receive_data(self):
        # Receives the data written in a string line
        data = self.sock.recv(1024)

        data = data.decode()

        steps, distance, time, calories, s_id = data.split(",")

        retrieved_data = {}
        retrieved_data['steps'] = steps
        retrieved_data['distance'] = distance
        retrieved_data['time'] = time
        retrieved_data['calories'] = calories
        retrieved_data['id'] = s_id

        # Closes connection
        self.sock.close()

        connected = False

        return retrieved_data