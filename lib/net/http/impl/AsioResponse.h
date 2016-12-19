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
   AsioResponse(StatusCode code, const Version& version, const Header& header);
   AsioResponse(AsioResponse&& rhs);

   virtual ~AsioResponse();

   AsioResponse& operator=(AsioResponse&& rhs);

   std::vector<asio::const_buffer> buffers();

   std::size_t size() const;

   //friend std::ostream& operator<<(std::ostream& o, const AsioResponse& r);

private:
   void build_header_buffer();

};

//std::ostream& operator<<(std::ostream& o, const AsioResponse& r);

} // http
} // net
} // orion

#endif // ORION_NET_HTTP_ASIORESPONSE_H
