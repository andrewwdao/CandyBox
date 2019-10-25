"""------------------------------------------------------------*-
  WebEmpath interaction module for Raspberry Pi
  Tested on: Raspberry Pi 3 B+
  (c) Can Tho University 2019
  version 1.00 - 08/10/2019
 --------------------------------------------------------------
 *
 *
 --------------------------------------------------------------"""
import json
from builtins import int
import urllib3
import socketio
import time
# ----------------------------Configurable parameters:

# -----WebEmpath connection:
mainUrl = 'https://api.webempath.net/v2/analyzeWav'
mainApi = "bGgzUd80q853LlOHoqZyWYnrSimSqRCwg6XaYqmfY2Y"
# -----Limit to be considered as being funny:
JOY_THRESHOLD = 20
# ----------------------------Global variable:
http = urllib3.PoolManager()  # Create an http object
current_joy = 0

SOCKET_CONNECTED = False

#socketio config:
socket = socketio.Client()

@socket.event
def message(data):
    print("Oh! It works")

@socket.on("new_thresh")
def on_message(data):
    global JOY_THRESHOLD
    print("New threshold: ", data)
    JOY_THRESHOLD = int(data)

@socket.event
def connect():
    global SOCKET_CONNECTED
    SOCKET_CONNECTED = True
    print('socket connected!')

def socket_connect():
    # time.sleep(5)
    print('connecting to socket...')
    socket.connect("http://18.179.14.225:3000")
    # time.sleep(0.5)
    # while not SOCKET_CONNECTED:
    #     try:
    #         print('connecting to socket...')
    #         socket.connect("http://0.0.0.0:1234")
    #         time.sleep(1)
    #     except:
    #         pass


def check(audio_file):
    global current_joy
    # Open the audio file and send it to the server
    with open(audio_file, 'rb') as file:
        file_data = file.read()
    res = http.request(
        method='POST',
        url=mainUrl,
        fields={
            'apikey': mainApi,
            "wav": (audio_file, file_data)
        }
    )

    # Check response from server and execute the appropriate task
    if res.status == 200:
        result = json.loads(res.data.decode('utf-8'))  # anger, joy, calm, energy, sorrow
        socket.emit("moods", result)
        # result example: {'error': 0, 'calm': 38, 'anger': 1, 'joy': 7, 'sorrow': 2, 'energy': 6}
        result_list = [int(result['error']), int(result['calm']), int(result['anger']),
                       int(result['joy']), int(result['sorrow']), int(result['energy'])]
        print(result)
        if  max(result_list) == int(result['joy']):
            if int(result['joy']) > JOY_THRESHOLD:
                socket.emit("drop_candy", "Y")
                return True
            else:
                return False
        else:
            return  False
    else:
        print("ERROR")
        return False


def joy_now():
    return current_joy
