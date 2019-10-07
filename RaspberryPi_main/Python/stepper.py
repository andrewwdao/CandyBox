"""------------------------------------------------------------*-
  LCD I2C python module for Raspberry Pi
  Tested on: Raspberry Pi 3 B+
  (c) Can Tho University 2019
  version 1.00 - 01/10/2019
 --------------------------------------------------------------
 * Command when using UART controller:
                           Change Speed:        S|<speed from 0 100%> (S|70)
                           Rotate Direction:    D|<0 for CW - 1 for CCW> (D|0)
                           Number of Rotation:  F|<float> (F|0.7)
                           Increase Speed:      H|<speed from 0 100%>   (H|5)
                           Decrease Speed:      L|<speed from 0 100%>   (L|3)
                           Read speed:          Read
                           Run unlimited:       Go!
                           Stop immediately:    Pause!
 --------------------------------------------------------------"""
import serial
import subprocess
import sys
import time


class StepperUart:
    # ----------------------------Configurable parameters:
    # -----UART parameters:
    turns = float()
    speed = int()

    def __init__(self, com_port='COM1', baud_rate=115200, turns=5, speed=80):
        self.turns = turns
        self.speed = speed
        # ----------------------------Class variable:
        self.slave = serial.Serial(com_port, baudrate=baud_rate,
                                   parity=serial.PARITY_NONE,
                                   stopbits=serial.STOPBITS_ONE,
                                   bytesize=serial.EIGHTBITS
                                   )
        time.sleep(1)
        # save the speed
        data = "S|" + str(speed)
        data = data.encode("utf-8")
        self.slave.write(data)
        time.sleep(1)
        print(b'Candy Box ready!')
        self.slave.write(b'Candy Box ready!')

    def move(self):
        data = "F|" + str(self.turns)
        data = data.encode("utf-8")
        self.slave.write(data)


class StepperControl:
    turns = float()
    speed = int()

    def __init__(self, turns=5, speed=80):
        self.turns = turns
        self.speed = speed

    def move(self):
        control_dir = "./stepper " + str(self.turns) + " " + str(self.speed)
        subprocess.Popen([control_dir], shell=True)
