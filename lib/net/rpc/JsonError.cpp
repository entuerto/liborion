//
// JsonError.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <net/rpc/JsonError.h>

#include <jsoncpp/json/json.h>

namespace orion
{
namespace net
{
namespace rpc
{

static const std::map<JsonErrc, std::string> JsonErrorText{
   {JsonErrc::ParseError, "An error occurred on the server while parsing the JSON text."},
   {JsonErrc::InvalidRequest, "The JSON sent is not a valid Request object."},
   {JsonErrc::MethodNotFound, "The method does not exist / is not available."},
   {JsonErrc::InvalidParams, "Invalid method parameter(s)."},
   {JsonErrc::InternalError, "Internal JSON-RPC error."}};

Error make_error(JsonErrc code) noexcept
{
   auto text = JsonErrorText.find(code);
   if (text != JsonErrorText.end())
      return Error(static_cast<int32_t>(code), text->second);

   return Error(static_cast<int32_t>(code));
}

} // namespace rpc
} // namespace net
} // namespace orion
