import socket
import serial
import time
import re

arduino = serial.Serial('COM1', baudrate = 115200, timeout=1)

UDP_IP = "127.0.0.1"
UDP_PORT = 1234

# STATE: NEUTRAI, TALKING and LAUGHING

state = 'NEUTRAI'
last_state = 'NEUTRAI'

speed = 70


sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

def data_parsing(data):
    # Split the data by the newlines:
    data = data.split('\n')
    # Remove blank spaces for every data:
    for i in range(data.__len__()):
        data[i] = data[i].strip()
    return data
noise = 0
voice = 0
male = 0
female = 0
laugh = 0
interval = 0
max_time = 1

while True:
    xml_data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    xml_data = xml_data.decode("utf-8")
    data = data_parsing(xml_data)
    for d in data:
        if "noise" in d:
            # noise = ''.join([c for c in d if c in '1234567890.'])
            noise = re.findall("\d+\.\d+", d)
        elif "voice" in d:
            # voice = ''.join([c for c in d if c in '1234567890.'])
            voice = re.findall("\d+\.\d+", d)
        elif "male" in d:
            # male = ''.join([c for c in d if c in '1234567890.'])
            male = re.findall("\d+\.\d+", d)
        elif "female" in d:
            # female = ''.join([c for c in d if c in '1234567890.'])
            female = re.findall("\d+\.\d+", d)
        elif "laugh" in d:
            # laugh = ''.join([c for c in d if c in '1234567890.'])
            laugh = float(re.findall("\d+\.\d+", d)[0])

    # print("Noise: " + str(noise))
    # print("Voice: " + str(voice))
    # print("Male: " + str(male))
    # print("Female: " + str(female))
    # print("Laugh: " + str(laugh))
    # print("\n")
    print(laugh)
    if noise > voice:
        state = 'NEUTRAL'
    elif voice > noise:
        if laugh < 0.5:
            state = 'TALKING'
        elif laugh > 0.5:
            state = 'LAUGHING'

    # state checking:
    if state == last_state and state == 'NEUTRAL':
        pass
    else:
        if(state == 'LAUGHING'):

            if time.time() - interval > max_time:
                interval = time.time()
                arduino.write(b'F|1')

        elif(state == 'NEUTRAL'):
            arduino.write(b'Pause!')
            interval = time.time()

    last_state = state
