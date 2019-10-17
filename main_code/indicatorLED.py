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
FAST_INTERVAL = 0.3
LONG_INTERVAL = 2
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

def wifiIsConnected():
    try:
        r = requests.get("http://www.google.com", timeout=500)
        return True
    except requests.ConnectionError:
        print("Internet Failed!!")
        return False

if __name__ == "__main__":
    # Check wifi connectivity
    while True:
        if not wifiIsConnected():
            for a in range(0,3):
                GPIO.output(LED_PIN, GPIO.HIGH)
                time.sleep(FAST_INTERVAL)
                GPIO.output(LED_PIN, GPIO.LOW)
                time.sleep(FAST_INTERVAL)
            time.sleep(LONG_INTERVAL)
        else:
            GPIO.output(LED_PIN, GPIO.HIGH)
