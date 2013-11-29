
import json
import urllib2

def send_jsonrpc(url, data):
   request = urllib2.Request(url=url, data=data)
   response_data = urllib2.urlopen(request).read()
   return response_data

if __name__ == '__main__':
   add = {"jsonrpc": "2.0", "method": "add", "id": 1}

   print send_jsonrpc('http://127.0.0.1:9090', json.dumps(add))

   add = {"jsonrpc": "2.0", "method": "add", "params" : [2,2], "id": 2}

   print send_jsonrpc('http://127.0.0.1:9090', json.dumps(add))

   add = {"jsonrpc": "2.0", "method": "add", "params" : {"a1": 3, "a2": 3}, "id": 3}

   print send_jsonrpc('http://127.0.0.1:9090', json.dumps(add))

   add = [{"jsonrpc": "2.0", "method": "add", "params" : {"a1": 4, "a2": 3}, "id": 4},
          {"jsonrpc": "2.0", "method": "add", "params" : {"a1": 5, "a2": 3}, "id": 5},
          {"jsonrpc": "2.0", "method": "add", "params" : {"a1": 6, "a2": 3}, "id": 6}]

   print send_jsonrpc('http://127.0.0.1:9090', json.dumps(add))

   add = {"jsonrpc": "2.0", "method": 1, "params": "bar"}

   print send_jsonrpc('http://127.0.0.1:9090', json.dumps(add))
