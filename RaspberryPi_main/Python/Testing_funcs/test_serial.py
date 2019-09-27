import serial
from datetime import date

print(date.today())
arduino = serial.Serial('COM4', baudrate = 115200, timeout=1)

arduino.write(b'Candy Box ready!')
#arduino.write(b'Pause!')