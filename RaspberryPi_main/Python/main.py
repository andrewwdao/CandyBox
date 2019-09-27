import requests
import webEmpath
import AudioRecord
from motor import StepperUart,StepperControl

# ---------------------------- Configurable parameters -------------------------
# -----Choose the method to control the stepper:
UART_CONTROL = True
# -----Limit to be considered as being funny:
JOY_THRESHOLD = 21
# -----Stepper Motor parameters:
TURNS = 1
SPEED = 100
# -----UART parameters: (ignore if don't use UART to control the system)
COM_PORT = 'COM4'
BAUD_RATE = 115200
# Check connection"
connection = False
def check_network():
    try:
        r = requests.get("http://www.google.com", timeout=5)
        return True
    except requests.ConnectionError:
        print("Internet Failed!!")
        return False
# ------------------------------------------------------------------------------

if __name__ == "__main__":
    while True:
        if connection == False:
            connection = check_network()
        else:
            break
    # ----------------------------Setup
    if UART_CONTROL:
        stepper = StepperUart(COM_PORT, BAUD_RATE, TURNS, SPEED)
    else:
        stepper = StepperControl(TURNS, SPEED)
    # ----------------------------Loop
    while True:
        AudioRecord.start()
        if webEmpath.check(AudioRecord.filename, JOY_THRESHOLD):
            AudioRecord.save_joy(webEmpath.joy_now())
            stepper.move()
            print("Candy Drop!")
