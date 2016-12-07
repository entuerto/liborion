//  test-jsonrpc.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <sstream>

#include <orion/TestUtils.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>
#include <orion/net/http/Utils.h>
#include <orion/net/rpc/JsonError.h>
#include <orion/net/rpc/JsonMethod.h>
#include <orion/net/rpc/JsonRequestHandler.h>

#include <jsoncpp/json/json.h>

using namespace orion;
using namespace orion::net;
using namespace orion::net::http;
using namespace orion::net::rpc;
using namespace orion::unittest;

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
      http::Request("POST", Url(), http::Version{1, 1}, http::Header()) 
      { _data = data; }

   virtual ~MockRequest() = default;

   virtual std::string content() const override { return _data; }

   virtual std::streambuf* rdbuf() const override { return nullptr; }

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

   virtual std::string content() const override { return _buf.str(); }

   virtual std::streambuf* rdbuf() const override 
   { 
      return const_cast<std::streambuf*>(dynamic_cast<const std::streambuf*>(&_buf)); 
   }

private:
   std::stringbuf _buf;
};

class MockMethod : public JsonMethod
{
public:
   MockMethod() : 
      JsonMethod("mock", "Mock method for tests.") {}

   virtual ~MockMethod() = default;

   virtual Error operator()(Json::Value& json_request, Json::Value& json_result) override
   {
      return Error();
   }
};

class MockJsonRequestHandler : public JsonRequestHandler
{
public:
   MockJsonRequestHandler() : JsonRequestHandler("") {}
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

TEST_SUITE(OrionWebService)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(Request, Creation)
{ 
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": 1}";

   auto mr = std::make_unique<MockRequest>(data);

   FAIL_IF(not mr);
   EXPECT_EQ(mr->content(), data);
}

TEST(Invalid, JsonResponse)
{
   std::string data = "toto";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;
   auto err = mrl.send_post_request(request, response);

   FAIL_IF(err.value() != 0);
   EXPECT_EQ(response.status_code(), StatusCode::OK);
   EXPECT_EQ(response.header("Content-Type"), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response.content(), json_result, false));
}

TEST(Valid, JsonResponse)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": \"1\"}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;
   auto err = mrl.send_post_request(request, response);

   FAIL_IF(err.value() != 0);
   EXPECT_EQ(response.status_code(), StatusCode::OK);
   EXPECT_EQ(response.header("Content-Type"), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response.content(), json_result, false));

   EXPECT_TRUE(is_jsonrpc_2(json_result));
}

TEST(Request, MethodNotFound)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": \"1\"}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;

   mrl.register_method(MockMethod());
   auto err = mrl.send_post_request(request, response);

   FAIL_IF(err.value() != 0);
   EXPECT_EQ(response.status_code(), StatusCode::OK);
   EXPECT_EQ(response.header("Content-Type"), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response.content(), json_result, false));

   EXPECT_TRUE(is_jsonrpc_2(json_result));
   EXPECT_TRUE(is_response_error(json_result));

   Json::Value error = json_result[JSON_RPC_RESPONSE_ERROR];

   EXPECT_EQ(error["code"], -32601); // JsonErrc::MethodNotFound

}

TEST(Request, MethodFound)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": \"1\"}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;

   mrl.register_method(MockMethod());
   auto err = mrl.send_post_request(request, response);

   FAIL_IF(err.value() != 0);
   EXPECT_EQ(response.status_code(), StatusCode::OK);
   EXPECT_EQ(response.header("Content-Type"), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response.content(), json_result, false));

   EXPECT_TRUE(is_jsonrpc_2(json_result));
   EXPECT_TRUE(is_response_result(json_result));

}

TEST(Id, AsString)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": \"1\"}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;

   mrl.register_method(MockMethod());
   auto err = mrl.send_post_request(request, response);

   FAIL_IF(err.value() != 0);
   EXPECT_EQ(response.status_code(), StatusCode::OK);
   EXPECT_EQ(response.header("Content-Type"), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response.content(), json_result, false));

   EXPECT_TRUE(is_jsonrpc_2(json_result));
   EXPECT_TRUE(is_response_result(json_result));
   EXPECT_TRUE(has_valid_id(json_result));

}

TEST(Id, AsInt)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": 1}";

   MockRequest  request(data);
   MockResponse response;

   MockJsonRequestHandler mrl;

   mrl.register_method(MockMethod());
   auto err = mrl.send_post_request(request, response);

   FAIL_IF(err.value() != 0);
   EXPECT_EQ(response.status_code(), StatusCode::OK);
   EXPECT_EQ(response.header("Content-Type"), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response.content(), json_result, false));

   EXPECT_TRUE(is_jsonrpc_2(json_result));
   EXPECT_TRUE(is_response_result(json_result));
   EXPECT_TRUE(has_valid_id(json_result));

}

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

