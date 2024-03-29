"""------------------------------------------------------------*-
  LED indicator for Raspberry Pi
  Tested on: Raspberry Pi 3 B+
  (c) Minh-An Dao 2019
  version 1.00 - 17/10/2019
 --------------------------------------------------------------
 *
 *
 --------------------------------------------------------------"""
import RPi.GPIO as GPIO  # default as BCM mode!
import time

LED_PIN = 23

SHORT_INTERVAL = 1.5
LONG_INTERVAL = 1

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(LED_PIN, GPIO.OUT)



if __name__ == "__main__":
    try:
        GPIO.output(LED_PIN, GPIO.HIGH)
        time.sleep(SHORT_INTERVAL)
        GPIO.output(LED_PIN, GPIO.LOW)
        # time.sleep(SHORT_INTERVAL)
    except KeyboardInterrupt:
        pass

