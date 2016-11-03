//  test-jsonrpc.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/TestUtils.h>
#include <orion/MemoryUtils.h>
#include <orion/net/Request.h>
#include <orion/net/Response.h>
#include <orion/ws/JsonRpcError.h>
#include <orion/ws/JsonRpcMethod.h>
#include <orion/ws/JsonRpcRequestListener.h>

#include <jsoncpp/json/json.h>

using namespace orion;
using namespace orion::net;
using namespace orion::ws;
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
class MockRequest : public Request
{
public:
   MockRequest(const std::string& data) : Request() { _data = data; }
   virtual ~MockRequest() = default;

   virtual std::string method() const        { return "POST"; }
   virtual std::string uri() const           { return "";}
   virtual std::string http_version() const  { return "1.1"; }
   virtual std::string query_string() const  { return ""; }   
   virtual std::string remote_user() const   { return ""; }    
   virtual bool is_authenticated()  const    { return false; }
   virtual bool is_secure_connection() const { return false; }

   static std::unique_ptr<Request> create(const std::string& data)
   {
      return std::make_unique<MockRequest>(data);
   }

};

class MockMethod : public JsonRpcMethod
{
public:
   MockMethod() : JsonRpcMethod() {}
   virtual ~MockMethod() = default;

   virtual std::string name() const        { return "mock"; }
   virtual std::string description() const { return "Mock method for tests."; }

   virtual std::unique_ptr<JsonRpcError> call(Json::Value& json_request, Json::Value& json_result)
   {
      return nullptr;
   }

   static std::unique_ptr<RpcMethod> create()
   {
      return std::make_unique<MockMethod>();
   }
};

class MockJsonRpcRequestListener : public JsonRpcRequestListener
{
public:
   MockJsonRpcRequestListener() : JsonRpcRequestListener("") {}
   virtual ~MockJsonRpcRequestListener() = default;

   std::unique_ptr<Response> send_post_request(const Request* request)
   {
      return on_post(request);
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

   auto mr = MockRequest::create(data);

   FAIL_IF(not mr);
   EXPECT_EQ(mr->content(), data);
}

TEST(Invalid, JsonResponse)
{
   std::string data = "toto";

   auto mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;
   auto response = mrl.send_post_request(mr.get());

   FAIL_IF(not response);
   EXPECT_EQ(response->code(), 200);
   EXPECT_EQ(response->mimetype(), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response->content(), json_result, false));
}

TEST(Valid, JsonResponse)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": \"1\"}";

   auto mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;
   auto response = mrl.send_post_request(mr.get());

   FAIL_IF(not response);
   EXPECT_EQ(response->code(), 200);
   EXPECT_EQ(response->mimetype(), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response->content(), json_result, false));

   EXPECT_TRUE(is_jsonrpc_2(json_result));
}

TEST(Request, MethodNotFound)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": \"1\"}";

   auto mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;

   mrl.register_method("mock", MockMethod::create());
   auto response = mrl.send_post_request(mr.get());

   FAIL_IF(not response);
   EXPECT_EQ(response->code(), 200);
   EXPECT_EQ(response->mimetype(), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response->content(), json_result, false));

   EXPECT_TRUE(is_jsonrpc_2(json_result));
   EXPECT_TRUE(is_response_error(json_result));

   Json::Value error = json_result[JSON_RPC_RESPONSE_ERROR];

   EXPECT_EQ(error["code"], -32601);

}

TEST(Request, MethodFound)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": \"1\"}";

   auto mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;

   mrl.register_method("mock", MockMethod::create());
   auto response = mrl.send_post_request(mr.get());

   FAIL_IF(not response);
   EXPECT_EQ(response->code(), 200);
   EXPECT_EQ(response->mimetype(), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response->content(), json_result, false));

   EXPECT_TRUE(is_jsonrpc_2(json_result));
   EXPECT_TRUE(is_response_result(json_result));


}

TEST(Id, AsString)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": \"1\"}";

   auto mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;

   mrl.register_method("mock", MockMethod::create());
   auto response = mrl.send_post_request(mr.get());

   FAIL_IF(not response);
   EXPECT_EQ(response->code(), 200);
   EXPECT_EQ(response->mimetype(), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response->content(), json_result, false));

   EXPECT_TRUE(is_jsonrpc_2(json_result));
   EXPECT_TRUE(is_response_result(json_result));
   EXPECT_TRUE(has_valid_id(json_result));

}

TEST(Id, AsInt)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": 1}";

   auto mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;

   mrl.register_method("mock", MockMethod::create());
   auto response = mrl.send_post_request(mr.get());

   FAIL_IF(not response);
   EXPECT_EQ(response->code(), 200);
   EXPECT_EQ(response->mimetype(), "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT_TRUE(reader.parse(response->content(), json_result, false));

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

