import pyaudio
import wave
import sox
from shutil import copy2
from datetime import datetime

# ----------------------------Configurable parameters:
# -----Sound quality parameters:
recording_time = 3 # seconds
# ---Destination quality:
target_rate = 11025 # 11kHz sampling rate
des_wav = "./audio11.wav"
# ---Original quality:
original_rate = 44100 # 44.1kHz sampling rate
org_wav = "./audio44.wav"
chunk = 4096  #2^12 samples for buffer
sample_format = pyaudio.paInt16  # 16 bits per sample/ resolution
channels = 1
dev_index = 2 # device index found by p.get_device_info_by_index(ii)
# -----laughter wav file directory:
joy_dir = "./wav_files/joy/"
# ----------------------------Global variable:
myAudio = pyaudio.PyAudio()  # Create an interface to PortAudio


def start():
    # Initialize array to store frames
    frames = []
    # Initialize an instance for recording audio
    stream = myAudio.open(format=sample_format,
                          channels=channels,
                          rate=original_rate,
                          frames_per_buffer=chunk,
                          input_device_index=dev_index,
                          input=True
                          )
    # Store data in chunks in recording_time second:
    for i in range(0, int(original_rate / chunk * recording_time)):
        data = stream.read(chunk)
        frames.append(data)
    # Stop and close the stream
    stream.stop_stream()
    stream.close()
    myAudio.terminate()

    # Create the original audio file
    wf = wave.open(org_wav, 'wb')
    wf.setnchannels(channels)
    wf.setsampwidth(myAudio.get_sample_size(sample_format))
    wf.setframerate(original_rate)
    wf.writeframes(b''.join(frames))
    wf.close()

    # convert it to appropriate freq
    #converter = sox.Transformer()
    #converter.convert(samplerate=target_rate)
    #converter.build(org_wav,des_wav)
    # equivalent to "sox -r target_rate org_wav des_wav" in command line
    # equivalent to "sox org_wav des_wav rate target_rate" in command line
    args = [org_wav, des_wav,'rate', str(target_rate)]
    sox.core.sox(args)

def save_joy(joy_now):
    now = datetime.now()
    copy2(des_wav, joy_dir+now.strftime("%H.%M.%S_%d%m%Y_")+str(joy_now)+".wav")

if __name__=="__main__":
    start()

