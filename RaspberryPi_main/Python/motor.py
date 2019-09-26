'''Command:
                           Change Speed:        S|<speed from 0 100%> (S|70)
                           Rotate Direction:    D|<0 for CW - 1 for CCW> (D|0)
                           Number of Rotation:  F|<float> (F|0.7)
                           Increase Speed:      H|<speed from 0 100%>   (H|5)
                           Decrease Speed:      L|<speed from 0 100%>   (L|3)
                           Read speed:          Read
                           Run unlimited:       Go!
                           Stop immediately:    Pause!
'''
import serial
import subprocess
import sys


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
        # save the speed
        data = "S|" + str(speed)
        data = data.encode("utf-8")
        self.slave.write(data)

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
        control_dir = "./main" + str(self.turns) + str(self.speed)
        try:
            retcode = subprocess.call(control_dir, shell=True)
            if retcode < 0:
                print("Child was terminated by signal", -retcode, file=sys.stderr)
            else:
                print("Child returned", retcode, file=sys.stderr)
        except OSError as e:
            print("Execution failed:", e, file=sys.stderr)
