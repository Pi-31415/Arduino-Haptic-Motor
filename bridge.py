#!C:\\Users\\aimlab\\AppData\\Local\\Programs\\Python\\Python311\\python.exe
"""
Rubber Hand Hardware Bridge

Author: Pi Ko
Email: pi.ko@nyu.edu

Description:
This software connects to a hardware device via a serial port.
It allows you to control the device by entering float values between 0 and 5.
You can also test the motors. The program listens for UDP messages.

Usage Manual:
1. Choose a serial port from the list of available ports.
2. Test the motors by entering values between 0 and 5.
3. Leave the program open to continue listening for inputs.
4. Press Ctrl+C to exit the program gracefully.
"""

import socket
import serial
import serial.tools.list_ports
import time
import threading
import os  # For clearing the terminal


# Constants
BAUD_RATE = 230400
UDP_IP = "127.0.0.1"  # Localhost
UDP_PORT = 11000  # Port number for UDP


def display_info():
    """Displays program information and usage manual."""
    # Clear the terminal
    os.system('cls' if os.name == 'nt' else 'clear')

    # Display title and author info
    print("==============================================")
    print("Rubber Hand Hardware Bridge")
    print("Author: Pi Ko")
    print("Email: pi.ko@nyu.edu")
    print("==============================================\n")

    # Software description and usage manual
    print("Description:")
    print("This software connects to Rubber Hand Experiment hardware device via a serial port.")
    print("It allows you to control the device by entering float values between 0 and 5.")
    print("You can also test the motors. The program listens for UDP messages, and forwards it to VR simulation.")
    print("\n\n⚠️⚠️ Leave this app open while running the simulation. ⚠️⚠️")
    print("\nUsage Manual:")
    print("1. Choose a serial port from the list of available ports.")
    print("2. Test the motors by entering values between 0 and 5.")
    print("3. Leave the program open to continue listening for inputs.")
    print("4. Press Ctrl+C to exit the program gracefully.")

    print("\nPress Enter to continue...")
    input()




def list_serial_ports():
    """Lists serial port names."""
    ports = serial.tools.list_ports.comports()
    available_ports = [port.device for port in ports]
    return available_ports

def get_port_descriptions():
    """Gets descriptions for serial ports."""
    ports = serial.tools.list_ports.comports()
    descriptions = [port.description for port in ports]
    return descriptions

def setup_udp_server(ip, port):
    """Sets up UDP server."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((ip, port))
    return sock

def udp_listener(sock, ser, stop_event):
    """Listens for messages from the UDP server and sends them to the serial port."""
    while not stop_event.is_set():
        try:
            data, addr = sock.recvfrom(1024)  # Buffer size is 1024 bytes
            # Convert data to float and send to serial
            float_data = float(data.decode())
            ser.write(f"{float_data}\n".encode('utf-8'))
        except ValueError:
            pass  # Ignore invalid data
        except (socket.error, OSError):
            # Socket has been closed, exit the loop
            break

def get_float_input():
    """Gets and validates float input from the user."""
    while True:
        try:
            value = float(input("\nEnter a float value between 0 and 5: "))
            if 0 <= value <= 5:
                return value
            else:
                print("Value must be between 0 and 5.")
        except ValueError:
            print("Invalid input. Please enter a valid float.")

def main():
    display_info() 
    print("Listing all available serial ports...")
    ports = list_serial_ports()
    descriptions = get_port_descriptions()
    if not ports:
        print("No hardware is connected.")
        return  # Exit the program
    for i, (port, description) in enumerate(zip(ports, descriptions)):
        print(f"[{i}]: {port} - {description}")

    try:
        port_index = int(input("\n\nEnter the port number (0,1,2 .etc) to connect: "))
        if port_index < 0 or port_index >= len(ports):
            print("Invalid port number selected.")
            return  # Exit the program
    except ValueError:
        print("Invalid input. Please enter a valid number.")
        return  # Exit the programselected_port = ports[port_index]

    selected_port = ports[port_index]
    # Establish serial connection
    ser = serial.Serial(selected_port, BAUD_RATE, timeout=1)
    ser.flush()

    # Send "0" first, then send "5" after connecting
    ser.write("0\n".encode('utf-8'))
    time.sleep(0.01)
    ser.write("5\n".encode('utf-8'))
    time.sleep(0.01)

    # Set up and start UDP listener thread
    sock = setup_udp_server(UDP_IP, UDP_PORT)
    stop_event = threading.Event()
    udp_thread = threading.Thread(target=udp_listener, args=(sock, ser, stop_event))
    udp_thread.start()

    try:
        while True:
            value = get_float_input()
            ser.write(f"{value}\n".encode('utf-8'))
            time.sleep(0.01)
    except KeyboardInterrupt:
        # Send "5" before quitting
        ser.write("5\n".encode('utf-8'))
        print("\nExiting...")
        # Signal the udp_listener thread to stop
        stop_event.set()
        # Close the UDP socket
        sock.close()
        # Wait for the udp_listener thread to finish
        udp_thread.join()
        # Close the serial connection
        ser.close()
        exit(0)

if __name__ == "__main__":
    main()
