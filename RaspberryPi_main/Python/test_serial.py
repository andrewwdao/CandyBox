import serial
import time
arduino = serial.Serial('COM1', baudrate = 115200, timeout=1)

arduino.write(b'S|100')
arduino.write(b'Pause!')