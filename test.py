# This connects UDP to Unity and Serial to Python
import serial.tools.list_ports
import time
import serial
ports = list(serial.tools.list_ports.comports())
for p in ports:
    print(p)


# Importing Libraries
arduino = serial.Serial(port='/dev/cu.usbmodem144301',
                        baudrate=115200, timeout=.1)


def write_read(x):
    arduino.write(bytes(x+'\n', 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data


while True:
    num = input("Enter a number: ")  # Taking input from user
    value = write_read(num)
    print(value)  # printing the value
