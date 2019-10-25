"""------------------------------------------------------------*-
  LED indicator for Raspberry Pi
  Tested on: Raspberry Pi 3 B+
  (c) Minh-An Dao 2019
  version 1.00 - 17/10/2019
 --------------------------------------------------------------
 *
 *
 --------------------------------------------------------------"""
from builtins import range

import RPi.GPIO as GPIO  # default as BCM mode!
import requests
import time

IR_PIN = 2

DEBOUNCE=10
IR_FLAG = False
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(IR_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)


def IR_ISR(channel):
    global IR_FLAG
    IR_FLAG = True

if __name__ == "__main__":
    try:
        GPIO.add_event_detect(IR_PIN, GPIO.RISING, callback=IR_ISR, bouncetime=DEBOUNCE)
        ir_counter=0
        while True:
            if IR_FLAG:
                ir_counter+=1
                print("Candy Fall!")
    except KeyboardInterrupt:
        GPIO.cleanup()
        pass

