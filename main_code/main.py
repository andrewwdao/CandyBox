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
import RPi.GPIO as GPIO  # default as BCM mode!
import webEmpath
from stepper import StepperUart,StepperControl
import subprocess as subpro
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

LED_PIN = 23
ON_PIN = 3
OFF_PIN = 4

DEBOUNCE=10
READY = False

def onISR(channel):
    global READY
    READY = True

def offISR(channel):
    global READY
    READY = False

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.setup(ON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(OFF_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.add_event_detect(ON_PIN, GPIO.FALLING, callback=onISR, bouncetime=DEBOUNCE)
GPIO.add_event_detect(OFF_PIN, GPIO.FALLING, callback=offISR, bouncetime=DEBOUNCE)

def wifiIsConnected():
    try:
        r = requests.get("http://www.google.com", timeout=500)
        return True
    except requests.ConnectionError:
        print("Internet Failed!!")
        return False
# ------------------------------------------------------------------------------

if __name__ == "__main__":
    try:
        # ----------------------------Setup
        if UART_CONTROL:
            stepper = StepperUart(COM_PORT, BAUD_RATE, TURNS, SPEED)
        else:
            stepper = StepperControl(TURNS, SPEED)
        # ----------------------------Loop
        # Check wifi connectivity
        while True:
            if not wifiIsConnected():
                print('not connected')
                subpro.Popen(["python3 notConnected.py"], shell=True)
            else:
                if True:
                    print('connected: ON')
                    subpro.Popen(["python3 ready.py"], shell=True)
                    PiAudioRecord.start()
                    if webEmpath.check(PiAudioRecord.des_wav, JOY_THRESHOLD):
                        PiAudioRecord.save_joy(webEmpath.joy_now())
                        stepper.move()
                        print("Candy Drop!")
                else:
                    print('connected: OFF')
                    GPIO.output(LED_PIN, GPIO.HIGH)
    except KeyboardInterrupt:
        pass
