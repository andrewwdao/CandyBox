import pyaudio
import urllib3
import wave
import json

form_1 = pyaudio.paInt16
chans = 1
samp_rate = 48000
chunk = 4098
record_secs = 3
dev_index = 2
wave_outputs_filename =  'test.wav'

audio = pyaudio.PyAudio()

stream = audio.open(format = form_1, rate = 11025, channels = chans, input_device_index = dev_index, input = True, frames_per_buffer=chunk)
print("recording")
frames = []

for ii in range(0,int(samp_rate/chunk*record_secs)):
	data = stream.read(chunk)
	frames.append(data)

print("finish")

stream.stop_stream()
stream.close()
audio.terminate()

wavefile = wave.open(wave_outputs_filename, 'wb')
wavefile.setnchannels(chans)
wavefile.setsampwidth(audio.get_sample_size(form_1))
wavefile.setframerate(11025)
wavefile.writeframes(b''.join(frames))
wavefile.close()

file = open("test.wav", 'rb')
data = file.read()
http = urllib3.PoolManager()
res = http.request(
	method='POST',
	url='https://api.webempath.net/v2/analyzeWav',
	fields={
		'apikey':"bGgzUd80q853LlOHoqZyWYnrSimSqRCwg6XaYqmfY2Y",
		"wav": ('test.wav', data)
	}
)

if (res.status == 200):
	result = json.loads(res.data.decode('utf-8'))
	print(result)
