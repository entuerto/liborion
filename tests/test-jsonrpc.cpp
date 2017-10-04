//  test-jsonrpc.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <sstream>

#include <orion/Test.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>
#include <orion/net/http/Utils.h>

#include <jsoncpp/json/json.h>

using namespace orion;
using namespace orion::net;
using namespace orion::net::http;
using namespace orion::unittest;

using namespace std::string_literals;

#define JSON_RPC_METHODNAME      "method"
#define JSON_RPC_LABEL           "jsonrpc"
#define JSON_RPC_ID              "id"
#define JSON_RPC_PARAMETERS      "params"
#define JSON_RPC_RESPONSE_ERROR  "error"
#define JSON_RPC_RESPONSE_RESULT "result"
#define JSON_RPC_VERSION         "2.0"

//----------------------------------------------------------------------------
// Mock classes
//----------------------------------------------------------------------------
class MockRequest : public http::Request
{
public:
   MockRequest(const std::string& data) : 
      http::Request(Method::POST, Url(), http::Version{1, 1}, http::Header()) 
      { _data = data; }

   virtual ~MockRequest() = default;

   virtual std::streambuf* rdbuf() const override { return nullptr; }

   std::string content() const { return _data; } 

private:
   std::string _data;
};

class MockResponse : public http::Response
{
public:
   MockResponse() : 
      http::Response(StatusCode::OK)
      {}

   virtual ~MockResponse() = default;

   virtual std::streambuf* rdbuf() const override 
   { 
      return const_cast<std::streambuf*>(dynamic_cast<const std::streambuf*>(&_buf)); 
   }

   std::string content() const { return _buf.str(); }

private:
   std::stringbuf _buf;
};

class MockMethod : public rpc::JsonMethod
{
public:
   MockMethod() : 
      rpc::JsonMethod("mock", "Mock method for tests.") {}

   virtual ~MockMethod() = default;

   virtual rpc::Error operator()(Json::Value& json_request, Json::Value& json_result) override
   {
      return rpc::Error();
   }
};

class MockJsonRequestHandler : public rpc::JsonRequestHandler
{
public:
   MockJsonRequestHandler() : rpc::JsonRequestHandler("") {}
   virtual ~MockJsonRequestHandler() = default;

   std::error_code send_post_request(const http::Request& request, http::Response& response)
   {
      return on_post(request, response);
   }

};

bool is_jsonrpc_2(Json::Value& json_result)
{
   return json_result.isMember(JSON_RPC_LABEL) and 
          json_result[JSON_RPC_LABEL] == JSON_RPC_VERSION;
}

bool is_response_result(Json::Value& json_result)
{
   return json_result.isMember(JSON_RPC_RESPONSE_RESULT);
}

bool is_response_error(Json::Value& json_result)
{
   return json_result.isMember(JSON_RPC_RESPONSE_ERROR);
}

bool has_valid_id(Json::Value& json_result)
{
   return json_result.isMember(JSON_RPC_ID) and
          not (json_result[JSON_RPC_ID].isArray() or json_result[JSON_RPC_ID].isObject());
}

TestSuite(OrionWebService)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
void request_creation(Test& t)
{ 
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": 1}";

   auto mr = std::make_unique<MockRequest>(data);

   t.fail_if(not mr, _src_loc);

   t.assert<std::equal_to<>>(mr->content(), data, _src_loc);
}

void invalid_json_response(Test& t)
{
   std::string data = "toto";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;
   auto err = mrl.send_post_request(request, response);

   t.fail_if(err.value() != 0, _src_loc);

   t.assert<std::equal_to<>>(StatusCode::OK, response.status_code(), _src_loc);
   t.assert<std::equal_to<>>("application/json"s, response.header("Content-Type"), _src_loc);

   Json::Reader reader;
   Json::Value  json_result;

   t.assert<true>(reader.parse(response.content(), json_result, false), _src_loc);
}

void valid_json_response(Test& t)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": \"1\"}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;
   auto err = mrl.send_post_request(request, response);

   t.fail_if(err.value() != 0, _src_loc);

   t.assert<std::equal_to<>>(StatusCode::OK, response.status_code(), _src_loc);
   t.assert<std::equal_to<>>("application/json"s, response.header("Content-Type"), _src_loc);

   Json::Reader reader;
   Json::Value  json_result;

   t.assert<true>(reader.parse(response.content(), json_result, false), _src_loc);

   t.assert<true>(is_jsonrpc_2(json_result), _src_loc);
}

void request_method_not_found(Test& t)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": \"1\"}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;

   mrl.register_method(MockMethod());
   auto err = mrl.send_post_request(request, response);

   t.fail_if(err.value() != 0, _src_loc);

   t.assert<std::equal_to<>>(StatusCode::OK, response.status_code(), _src_loc);
   t.assert<std::equal_to<>>("application/json"s, response.header("Content-Type"), _src_loc);

   Json::Reader reader;
   Json::Value  json_result;

   t.assert<true>(reader.parse(response.content(), json_result, false), _src_loc);

   t.assert<true>(is_jsonrpc_2(json_result), _src_loc);
   t.assert<true>(is_response_error(json_result), _src_loc);

   Json::Value error = json_result[JSON_RPC_RESPONSE_ERROR];

   t.assert<std::equal_to<>>(-32601, error["code"].asInt(), _src_loc); // JsonErrc::MethodNotFound

}

void request_method_found(Test& t)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": \"1\"}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;

   mrl.register_method(MockMethod());
   auto err = mrl.send_post_request(request, response);

   t.fail_if(err.value() != 0, _src_loc);

   t.assert<std::equal_to<>>(StatusCode::OK, response.status_code(), _src_loc);
   t.assert<std::equal_to<>>("application/json"s, response.header("Content-Type"), _src_loc);

   Json::Reader reader;
   Json::Value  json_result;

   t.assert<true>(reader.parse(response.content(), json_result, false), _src_loc);

   t.assert<true>(is_jsonrpc_2(json_result), _src_loc);
   t.assert<true>(is_response_result(json_result), _src_loc);

}

void id_as_string(Test& t)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": \"1\"}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;

   mrl.register_method(MockMethod());
   auto err = mrl.send_post_request(request, response);

   t.fail_if(err.value() != 0, _src_loc);

   t.assert<std::equal_to<>>(StatusCode::OK, response.status_code(), _src_loc);
   t.assert<std::equal_to<>>("application/json"s, response.header("Content-Type"), _src_loc);

   Json::Reader reader;
   Json::Value  json_result;

   t.assert<true>(reader.parse(response.content(), json_result, false), _src_loc);

   t.assert<true>(is_jsonrpc_2(json_result), _src_loc);
   t.assert<true>(is_response_result(json_result), _src_loc);
   t.assert<true>(has_valid_id(json_result), _src_loc);

}

void id_as_int(Test& t)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": 1}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;

   mrl.register_method(MockMethod());
   auto err = mrl.send_post_request(request, response);

   t.fail_if(err.value() != 0, _src_loc);

   t.assert<std::equal_to<>>(StatusCode::OK, response.status_code(), _src_loc);
   t.assert<std::equal_to<>>("application/json"s, response.header("Content-Type"), _src_loc);

   Json::Reader reader;
   Json::Value  json_result;

   t.assert<true>(reader.parse(response.content(), json_result, false), _src_loc);

   t.assert<true>(is_jsonrpc_2(json_result), _src_loc);
   t.assert<true>(is_response_result(json_result), _src_loc);
   t.assert<true>(has_valid_id(json_result), _src_loc);
}

RegisterTestCase(OrionWebService, request_creation);
RegisterTestCase(OrionWebService, invalid_json_response);
RegisterTestCase(OrionWebService, valid_json_response);
RegisterTestCase(OrionWebService, request_method_not_found);
RegisterTestCase(OrionWebService, request_method_found);
RegisterTestCase(OrionWebService, id_as_string);
RegisterTestCase(OrionWebService, id_as_int);
} // TEST_SUITE(OrionWebService)

/*
rpc call with positional parameters:

--> {"jsonrpc": "2.0", "method": "subtract", "params": [42, 23], "id": 1}
<-- {"jsonrpc": "2.0", "result": 19, "id": 1}

rpc call with named parameters:

--> {"jsonrpc": "2.0", "method": "subtract", "params": {"subtrahend": 23, "minuend": 42}, "id": 3}
<-- {"jsonrpc": "2.0", "result": 19, "id": 3}

a Notification:

--> {"jsonrpc": "2.0", "method": "update", "params": [1,2,3,4,5]}
--> {"jsonrpc": "2.0", "method": "foobar"}

rpc call of non-existent method:

--> {"jsonrpc": "2.0", "method": "foobar", "id": "1"}
<-- {"jsonrpc": "2.0", "error": {"code": -32601, "message": "Method not found"}, "id": "1"}

rpc call with invalid JSON:

--> {"jsonrpc": "2.0", "method": "foobar, "params": "bar", "baz]
<-- {"jsonrpc": "2.0", "error": {"code": -32700, "message": "Parse error"}, "id": null}

*/

