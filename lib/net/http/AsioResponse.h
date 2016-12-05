//
// AsioResponse.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_ASIORESPONSE_H
#define ORION_NET_HTTP_ASIORESPONSE_H

#include <ostream>

#include <orion/Orion-Stddefs.h>
#include <orion/net/http/Response.h>

#include <asio.hpp>

namespace orion
{
namespace net
{
namespace http
{
///
class AsioResponse : public Response
{
public:
   AsioResponse(StatusCode code);
   AsioResponse(StatusCode code, const Version& version);
   AsioResponse(AsioResponse&& Other);

   virtual ~AsioResponse();

   AsioResponse& operator=(AsioResponse&& Rhs);

   virtual std::string content() const override;

   virtual std::streambuf* rdbuf() const override;

   std::vector<asio::const_buffer> buffers();

   std::size_t size() const;

   //friend std::ostream& operator<<(std::ostream& o, const AsioResponse& r);

private:
   void build_header_buffer();

   std::unique_ptr<asio::streambuf> _header_streambuf;
   std::unique_ptr<asio::streambuf> _body_streambuf;
};

//std::ostream& operator<<(std::ostream& o, const AsioResponse& r);
/*
class AsioResponseConstBuffer
{
public:
   // Implement the ConstBufferSequence requirements.
   typedef asio::const_buffer value_type;
   typedef const asio::const_buffer* const_iterator;

   explicit AsioResponseConstBuffer(const AsioResponse& data);

   const asio::const_buffer* begin() const { return &_buffer; }
   const asio::const_buffer* end()   const { return &_buffer + 1; }

   std::size_t size() const { return _size; }

private:
   std::shared_ptr<std::string> _data;
   asio::const_buffer _buffer;
   std::size_t _size;
};
*/
} // http
} // net
} // orion

#endif // ORION_NET_HTTP_ASIORESPONSE_H
