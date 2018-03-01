//
// Protocol.ipp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_RPC_PROTOCOL_IPP
#define ORION_RPC_PROTOCOL_IPP

#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace net
{
namespace rpc
{

//--------------------------------------------------------------------------------------------------
// ContentType
//--------------------------------------------------------------------------------------------------

inline std::istream& operator>>(std::istream& s, ContentType& ct)
{
   std::string value;

   s >> value;

   if (value == "Raw")
      ct = ContentType::Raw;
   else if (value == "Char")
      ct = ContentType::Char;
   else if (value == "Json")
      ct = ContentType::Json;

   return s;
}

inline std::ostream& operator<<(std::ostream& s, ContentType ct)
{
   switch (ct)
   {
      case ContentType::Raw:
         s << "Raw";
         break;
      case ContentType::Char:
         s << "Char";
         break;
      case ContentType::Json:
         s << "Json";
         break;
   }
   
   return s;
}

//--------------------------------------------------------------------------------------------------
// Message
//--------------------------------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& s, const Message& msg)
{
   s << "\n\nMessage\n"
     << msg.tag
     << " " 
     << msg.version 
     << " ";

     asio::streambuf tmp_buf;

     copy_streambuf(tmp_buf, msg.content.data);

     s << &tmp_buf << "\n";

   return s;
}

//--------------------------------------------------------------------------------------------------
// Request
//--------------------------------------------------------------------------------------------------

template<typename IdT>
Request<IdT>::Request(IdT id)
   : id(id)
   , method()
   , params()
{
}

template<typename IdT>
Request<IdT>::Request(const Request& other)
   : id(other.id)
   , method(other.method)
   , params()
{
   copy_streambuf(params, other.params);
}

template<typename IdT>
Request<IdT>& Request<IdT>::operator=(const Request& other)
{
   if (this == &other)
      return *this;

   id     = other.id;
   method = other.method;

   copy_streambuf(params, other.params);

   return *this;
}

//--------------------------------------------------------------------------------------------------
// Response
//--------------------------------------------------------------------------------------------------

template<typename IdT>
Response<IdT>::Response(IdT id)
   : id(id)
   , result()
{
}

template<typename IdT>
Response<IdT>::Response(const Response& other)
   : id(other.id)
   , result()
{
   copy_streambuf(result, other.result);
}

template<typename IdT>
Response<IdT>& Response<IdT>::operator=(const Response& other)
{
   if (this == &other)
      return *this;

   id = other.id;

   copy_streambuf(result, other.result);

   return *this;
}

//-------------------------------------------------------------------------------------------------

} // namespace rpc
} // namespace net
} // namespaceorion
#endif // ORION_RPC_PROTOCOL_IPP