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

LED_PIN = 4
ON_PIN = 9
OFF_PIN = 7

DEBOUNCE=10
FAST_INTERVAL = 0.3
LONG_INTERVAL = 2

READY = False
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.setup(ON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(OFF_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

def onISR(channel):
    global READY
    READY = True

def offISR(channel):
    global READY
    READY = False

def wifiIsConnected():
    try:
        r = requests.get("http://www.google.com", timeout=500)
        return True
    except requests.ConnectionError:
        print("Internet Failed!!")
        return False

if __name__ == "__main__":
    try:
        GPIO.add_event_detect(ON_PIN, GPIO.FALLING, callback=onISR, bouncetime=DEBOUNCE)
        GPIO.add_event_detect(OFF_PIN, GPIO.FALLING, callback=offISR, bouncetime=DEBOUNCE)
        # Check wifi connectivity
        while True:
            if not wifiIsConnected():
                print('not connected')
                for a in range(0, 3):
                    GPIO.output(LED_PIN, GPIO.HIGH)
                    time.sleep(FAST_INTERVAL)
                    GPIO.output(LED_PIN, GPIO.LOW)
                    time.sleep(FAST_INTERVAL)
                time.sleep(LONG_INTERVAL)
            else:
                if READY:
                    print('connected: ON')
                    GPIO.output(LED_PIN, GPIO.HIGH)
                    time.sleep(LONG_INTERVAL)
                    GPIO.output(LED_PIN, GPIO.LOW)
                    time.sleep(LONG_INTERVAL)
                else:
                    print('connected: OFF')
                    GPIO.output(LED_PIN, GPIO.HIGH)
    except KeyboardInterrupt:
        GPIO.close()
        pass

