
import json
import urllib2

def send_jsonrpc(url, data):
   request = urllib2.Request(url=url, data=data)
   response_data = urllib2.urlopen(request).read()
   return response_data

if __name__ == '__main__':
   stop_server = {"jsonrpc": "2.0", "method": "shutdown", "id": 60}

   print send_jsonrpc('http://127.0.0.1:9090', json.dumps(stop_server))
