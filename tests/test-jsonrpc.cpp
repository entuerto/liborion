
#include <iostream>
#include <orion/TestUtils.h>
#include <orion/MemoryUtils.h>
#include <orion/ws/Request.h>
#include <orion/ws/Response.h>
#include <orion/ws/JsonRpcError.h>
#include <orion/ws/JsonRpcMethod.h>
#include <orion/ws/JsonRpcRequestListener.h>

#include <ws/json/reader.h>
#include <ws/json/value.h>

using namespace orion;
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
   DECLARE_POINTERS(MockRequest)

   MockRequest(const std::string& data) : Request() { _data = data; }
   virtual ~MockRequest()    {}

   virtual std::string method() const        { return "POST"; }
   virtual std::string uri() const           { return "";}
   virtual std::string http_version() const  { return "1.1"; }
   virtual std::string query_string() const  { return ""; }   
   virtual std::string remote_user() const   { return ""; }    
   virtual bool is_authenticated()  const    { return false; }
   virtual bool is_secure_connection() const { return false; }

   static Request::SharedPtr create(const std::string& data)
   {
      return Request::SharedPtr(new MockRequest(data));
   }

};

class MockMethod : public JsonRpcMethod
{
public:
   MockMethod() : JsonRpcMethod() {}
   virtual ~MockMethod()  {}

   virtual std::string name() const        { return "mock"; }
   virtual std::string description() const { return "Mock method for tests."; }

   virtual JsonRpcError::SharedPtr call(Json::Value& json_request, Json::Value& json_result)
   {
      return nullptr;
   }

   static RpcMethod::SharedPtr create()
   {
      MockMethod* m = new MockMethod;

      return RpcMethod::SharedPtr(m);
   }
};

class MockJsonRpcRequestListener : public JsonRpcRequestListener
{
public:
   DECLARE_POINTERS(MockJsonRpcRequestListener)
 
   MockJsonRpcRequestListener() : JsonRpcRequestListener() {}
   virtual ~MockJsonRpcRequestListener() {}

   Response::SharedPtr send_post_request(Request::SharedPtr request)
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

//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(TestRequest)
{ 
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": 1}";

   Request::SharedPtr mr = MockRequest::create(data);

   FAIL_IF(not mr);
   EXPECT(mr->content() == data);
}

TEST(TestInvalidJsonResponse)
{
   std::string data = "toto";

   Request::SharedPtr mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;
   Response::SharedPtr response = mrl.send_post_request(mr);

   FAIL_IF(not response);
   EXPECT(response->code() == 200);
   EXPECT(response->mimetype() == "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT(reader.parse(response->content(), json_result, false));
}

TEST(TestValidJsonResponse)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": \"1\"}";

   Request::SharedPtr mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;
   Response::SharedPtr response = mrl.send_post_request(mr);

   FAIL_IF(not response);
   EXPECT(response->code() == 200);
   EXPECT(response->mimetype() == "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT(reader.parse(response->content(), json_result, false));

   EXPECT(is_jsonrpc_2(json_result));
}

TEST(TestMethodNotFound)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"id\": \"1\"}";

   Request::SharedPtr mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;

   mrl.register_method("mock", MockMethod::create());
   Response::SharedPtr response = mrl.send_post_request(mr);

   FAIL_IF(not response);
   EXPECT(response->code() == 200);
   EXPECT(response->mimetype() == "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT(reader.parse(response->content(), json_result, false));

   EXPECT(is_jsonrpc_2(json_result));
   EXPECT(is_response_error(json_result));

   Json::Value error = json_result[JSON_RPC_RESPONSE_ERROR];

   EXPECT(error["code"] == -32601);

}

TEST(TestMethodFound)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": \"1\"}";

   Request::SharedPtr mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;

   mrl.register_method("mock", MockMethod::create());
   Response::SharedPtr response = mrl.send_post_request(mr);

   FAIL_IF(not response);
   EXPECT(response->code() == 200);
   EXPECT(response->mimetype() == "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT(reader.parse(response->content(), json_result, false));

   EXPECT(is_jsonrpc_2(json_result));
   EXPECT(is_response_result(json_result));


}

TEST(TestIdAsString)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": \"1\"}";

   Request::SharedPtr mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;

   mrl.register_method("mock", MockMethod::create());
   Response::SharedPtr response = mrl.send_post_request(mr);

   FAIL_IF(not response);
   EXPECT(response->code() == 200);
   EXPECT(response->mimetype() == "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT(reader.parse(response->content(), json_result, false));

   EXPECT(is_jsonrpc_2(json_result));
   EXPECT(is_response_result(json_result));
   EXPECT(has_valid_id(json_result));

}

TEST(TestIdAsInt)
{
   std::string data = "{\"jsonrpc\": \"2.0\", \"method\": \"mock\", \"id\": 1}";

   Request::SharedPtr mr = MockRequest::create(data);

   MockJsonRpcRequestListener mrl;

   mrl.register_method("mock", MockMethod::create());
   Response::SharedPtr response = mrl.send_post_request(mr);

   FAIL_IF(not response);
   EXPECT(response->code() == 200);
   EXPECT(response->mimetype() == "application/json");

   Json::Reader reader;
   Json::Value  json_result;

   EXPECT(reader.parse(response->content(), json_result, false));

   EXPECT(is_jsonrpc_2(json_result));
   EXPECT(is_response_result(json_result));
   EXPECT(has_valid_id(json_result));

}

//----------------------------------------------------------------------------
// Main functions
//----------------------------------------------------------------------------
int main(int argc, char** argv)
{
   return run_all_tests(TestStdOutput::create(std::cout));
}

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

