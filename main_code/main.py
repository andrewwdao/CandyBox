"""------------------------------------------------------------*-
  CandyBox main module for Raspberry Pi
  Tested on: Raspberry Pi 3 B+
  (c) Can Tho University 2019
  version 1.00 - 08/10/2019
 --------------------------------------------------------------
 *
 *
 --------------------------------------------------------------"""
import PiAudioRecord #use for Raspberry Pi
#import AudioRecord #use for NUC
import requests
import webEmpath
from stepper import StepperUart,StepperControl
import sys

#sys.settrace()
# ---------------------------- Configurable parameters -------------------------
# -----Choose the method to control the stepper:
UART_CONTROL = False
# -----Limit to be considered as being funny:
JOY_THRESHOLD = 5
# -----Stepper Motor parameters:
TURNS = 1
SPEED = 100
# -----UART parameters: (ignore if don't use UART to control the system)
COM_PORT = 'COM4'
BAUD_RATE = 115200

def wifiIsConnected():
    try:
        r = requests.get("http://www.google.com", timeout=500)
        return True
    except requests.ConnectionError:
        print("Internet Failed!!")
        return False
# ------------------------------------------------------------------------------

if __name__ == "__main__":

    # Check wifi connectivity
    while not wifiIsConnected():
        pass
    # ----------------------------Setup
    if UART_CONTROL:
        stepper = StepperUart(COM_PORT, BAUD_RATE, TURNS, SPEED)
    else:
        stepper = StepperControl(TURNS, SPEED)
    # ----------------------------Loop
    while True:
        PiAudioRecord.start()
        if webEmpath.check(PiAudioRecord.des_wav, JOY_THRESHOLD):
            PiAudioRecord.save_joy(webEmpath.joy_now())
            stepper.move()
            print("Candy Drop!")
