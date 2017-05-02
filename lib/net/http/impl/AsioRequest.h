//
// AsioRequest.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_ASIOREQUEST_H
#define ORION_NET_HTTP_ASIOREQUEST_H

#include <string>
 
#include <orion/Orion-Stddefs.h>
#include <orion/net/http/Request.h>

#include <asio.hpp>

namespace orion
{
namespace net
{
namespace http
{
///
class AsioRequest : public Request
{
public:
   AsioRequest();
   AsioRequest(const Method& method, 
               const Url& url, 
               const Version& version, 
               const Header& header);
   AsioRequest(AsioRequest&& rhs);

   virtual ~AsioRequest();

   AsioRequest& operator=(AsioRequest&& rhs);

   std::vector<asio::const_buffer> buffers();

   std::size_t size() const;

private:
   void build_header_buffer();

};

} // http
} // net
} // orion

#endif
