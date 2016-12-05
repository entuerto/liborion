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
   AsioRequest(const std::string& method, 
               const Url& url, 
               const Version& version, 
               const Header& header,
                     bool should_keep_alive = false,
                     bool upgrade = false);
   AsioRequest(AsioRequest&& Other);

   virtual ~AsioRequest();

   AsioRequest& operator=(AsioRequest&& Rhs);

   virtual std::string content() const override;

private:
};

} // http
} // net
} // orion

#endif
