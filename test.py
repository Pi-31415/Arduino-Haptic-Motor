# This connects UDP to Unity and Serial to Python
import serial.tools.list_ports
import time
import serial
import socket


localIP = "127.0.0.1"
localPort = 5500
bufferSize = 1024

msgFromServer = "Hello UDP Client"
bytesToSend = str.encode(msgFromServer)

# Arduino Setup


ports = list(serial.tools.list_ports.comports())
arduino_port_name = ""
for p in ports:
    if("Arduino" in str(p)):
        arduino_port_name = str(p).split(" -")[0]

# If Arduino is not found, don't do any actions
if(arduino_port_name == ""):
    print("\n\nError : Haptic Glove not found. Please connect.")
else:
    print("\n\nConnected to Haptic Glove on port "+arduino_port_name)


# Importing Libraries
arduino = serial.Serial(port=arduino_port_name,
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
