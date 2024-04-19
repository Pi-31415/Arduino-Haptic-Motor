import socket
import serial
import serial.tools.list_ports
import time
import threading

# Constants
BAUD_RATE = 230400
UDP_IP = "127.0.0.1"  # Localhost
UDP_PORT = 11000  # Port number for UDP

def list_serial_ports():
    """ Lists serial port names """
    ports = serial.tools.list_ports.comports()
    available_ports = [port.device for port in ports]
    return available_ports

def setup_udp_server(ip, port):
    """ Set up UDP server """
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((ip, port))
    return sock

def udp_listener(sock, ser):
    """ Listen for messages from the UDP server and send to serial port """
    while True:
        data, addr = sock.recvfrom(1024)  # Buffer size is 1024 bytes
        try:
            # Convert data to float and send to serial
            float_data = float(data.decode())
            ser.write(f"{float_data}\n".encode('utf-8'))
        except ValueError:
            pass  # Ignore invalid data

def get_float_input():
    """ Get and validate float input from the user """
    while True:
        try:
            value = float(input("Enter a float value between 0 and 5: "))
            if 0 <= value <= 5:
                return value
            else:
                print("Value must be between 0 and 5.")
        except ValueError:
            print("Invalid input. Please enter a valid float.")

def main():
    print("Listing all available serial ports...")
    ports = list_serial_ports()
    for i, port in enumerate(ports):
        print(f"{i}: {port}")

    port_index = int(input("Select the port number to connect: "))
    selected_port = ports[port_index]

    # Establish serial connection
    
    ser = serial.Serial(selected_port, BAUD_RATE, timeout=1)
    ser.flush()

    # Set up and start UDP listener thread
    sock = setup_udp_server(UDP_IP, UDP_PORT)
    udp_thread = threading.Thread(target=udp_listener, args=(sock, ser))
    udp_thread.daemon = True
    udp_thread.start()

    while True:
        value = get_float_input()
        ser.write(f"{value}\n".encode('utf-8'))
        time.sleep(0.01)

if __name__ == "__main__":
    main()
