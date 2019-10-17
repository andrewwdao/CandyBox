import json
import urllib3

file = open('test1.wav','rb')
file_data = file.read()
http = urllib3.PoolManager()
res = http.request(
	method = 'POST',
	url = 'https://api.webempath.net/v2/analyzeWav',
	fields = {
		'apikey': 'bGgzUd80q853LlOHoqZyWYnrSimSqRCwg6XaYqmfY2Y',
		'wav': ('test.wav',file_data)
	}
)
if res.status==200:
	result = json.loads(res.data.decode('utf-8'))
	print(result)


