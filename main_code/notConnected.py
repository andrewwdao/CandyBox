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
import time

LED_PIN = 23

FAST_INTERVAL = 0.3
LONG_INTERVAL = 2

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

if __name__ == "__main__":
    try:
        for a in range(0, 3):
            GPIO.output(LED_PIN, GPIO.HIGH)
            time.sleep(FAST_INTERVAL)
            GPIO.output(LED_PIN, GPIO.LOW)
            time.sleep(FAST_INTERVAL)
        time.sleep(LONG_INTERVAL)
    except KeyboardInterrupt:
        pass
