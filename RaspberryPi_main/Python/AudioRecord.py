"""------------------------------------------------------------*-
  Audio Recording module for NUC
  Tested on: NUC
  (c) Can Tho University 2019
  version 1.00 - 08/10/2019
 --------------------------------------------------------------
 *
 *
 --------------------------------------------------------------"""
import pyaudio
import wave
from shutil import copy2
from datetime import datetime

# ----------------------------Configurable parameters:
# -----Sound quality parameters:
filename = "stream.wav"
chunk = 1024  # Record in chunks of 1024 samples
sample_format = pyaudio.paInt16  # 16 bits per sample
channels = 1
fs = 11025  # Record at 11025 samples per second (required)
seconds = 3
# -----laughter wav file directory:
joy_dir = "./joy_wav/"
# ----------------------------Global variable:
myAudio = pyaudio.PyAudio()  # Create an interface to PortAudio


def start():
    # Initialize array to store frames
    frames = []
    # Initialize an instance for recording audio
    stream = myAudio.open(format=sample_format,
                          channels=channels,
                          rate=fs,
                          frames_per_buffer=chunk,
                          # input_device_index=2,
                          input=True)
    # Store data in chunks for 3 second:
    for i in range(0, int(fs / chunk * seconds)):
        data = stream.read(chunk)
        frames.append(data)
    # Stop and close the stream
    stream.stop_stream()
    stream.close()

    # Create the audio file
    wf = wave.open(filename, 'wb')
    wf.setnchannels(channels)
    wf.setsampwidth(myAudio.get_sample_size(sample_format))
    wf.setframerate(fs)
    wf.writeframes(b''.join(frames))
    wf.close()


def save_joy(joy_now):
    now = datetime.now()
    copy2("./" + filename, joy_dir+now.strftime("%H.%M.%S_%d%m%Y_")+str(joy_now)+".wav")

