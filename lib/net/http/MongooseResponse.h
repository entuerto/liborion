//
// MongooseResponse.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_MONGOOSERESPONSE_H
#define ORION_NET_HTTP_MONGOOSERESPONSE_H

#include <sstream>

#include <orion/Orion-Stddefs.h>
#include <orion/net/http/Response.h>

#include <mongoose/mongoose.h>

namespace orion
{
namespace net
{
namespace http
{
///
class MongooseResponse : public Response
{
public:
   MongooseResponse(StatusCode code);
   MongooseResponse(StatusCode code, const Version& version);
   MongooseResponse(MongooseResponse&& Other);

   virtual ~MongooseResponse();

   MongooseResponse& operator=(MongooseResponse&& Rhs);

   virtual std::string content() const override;

   virtual std::streambuf* rdbuf() const override;

   std::string buffer();
   
private:
   void build_header_buffer();

   std::unique_ptr<std::stringbuf> _header_streambuf;
   std::unique_ptr<std::stringbuf> _body_streambuf;
};

} // http
} // net
} // orion

#endif // ORION_NET_HTTP_MONGOOSERESPONSE_H
