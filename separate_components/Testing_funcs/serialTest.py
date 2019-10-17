import time
import serial

print("Starting program")

ser = serial.Serial('/dev/ttyACM0', baudrate=115200,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS
                    )
time.sleep(0.5)

try:
    ser.write(b'S|80')
    time.sleep(1)
    ser.write(b'F|5')

except KeyboardInterrupt:
    print("Exiting Program")
except:
    print("Error occurs!")
finally:
    ser.close()
    pass