
import json
from urllib.parse import quote_plus
from urllib.parse import urlencode
from urllib.request import Request
from urllib.request import urlopen

def send_jsonrpc(url, data):
   #d = quote_plus(data)
   d = urlencode(data).encode("utf-8")
   request = Request(url=url, data=d)
   response_data = urlopen(request).read()
   return response_data

if __name__ == '__main__':
   add = {"jsonrpc": "2.0", "method": "add", "id": 1}

   print(send_jsonrpc('http://127.0.0.1:9090', add))

   add = {"jsonrpc": "2.0", "method": "add", "params" : [2,2], "id": 2}

   print(send_jsonrpc('http://127.0.0.1:9090', add))

   add = {"jsonrpc": "2.0", "method": "add", "params" : {"a1": 3, "a2": 3}, "id": 3}

   print(send_jsonrpc('http://127.0.0.1:9090', add))

   add = [{"jsonrpc": "2.0", "method": "add", "params" : {"a1": 4, "a2": 3}, "id": 4},
          {"jsonrpc": "2.0", "method": "add", "params" : {"a1": 5, "a2": 3}, "id": 5},
          {"jsonrpc": "2.0", "method": "add", "params" : {"a1": 6, "a2": 3}, "id": 6}]

   print(send_jsonrpc('http://127.0.0.1:9090', add))

   add = {"jsonrpc": "2.0", "method": 1, "params": "bar"}

   print(send_jsonrpc('http://127.0.0.1:9090', add))

   substract = {"jsonrpc": "2.0", "method": "substract", "params": [3,2], "id": 40}

   print(send_jsonrpc('http://127.0.0.1:9090', substract))

   answer = {"jsonrpc": "2.0", "method": "answer", "id": 50}

   print(send_jsonrpc('http://127.0.0.1:9090', answer))
