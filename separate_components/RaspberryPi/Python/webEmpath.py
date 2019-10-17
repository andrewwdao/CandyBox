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
import urllib3

# ----------------------------Configurable parameters:

# -----WebEmpath connection:
mainUrl = 'https://api.webempath.net/v2/analyzeWav'
mainApi = "bGgzUd80q853LlOHoqZyWYnrSimSqRCwg6XaYqmfY2Y"

# ----------------------------Global variable:
http = urllib3.PoolManager()  # Create an http object
current_joy = 0


def check(audio_file, joy_threshold=20):
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
        # result example: {'error': 0, 'calm': 38, 'anger': 1, 'joy': 7, 'sorrow': 2, 'energy': 6}
        print(result)
        current_joy = int(result['joy'])
        if current_joy > joy_threshold:
            return True
        else:
            return False
    else:
        print("ERROR")
        return False


def joy_now():
    return current_joy
