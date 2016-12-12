//
//  MongooseRequest.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_MONGOOSEREQUEST_H
#define ORION_NET_HTTP_MONGOOSEREQUEST_H

#include <string>
#include <sstream>
 
#include <orion/Orion-Stddefs.h>
#include <orion/net/http/Request.h>

#include <mongoose/mongoose.h>

namespace orion
{
namespace net
{
namespace http
{
///
class MongooseRequest : public Request
{
public:
   MongooseRequest();
   MongooseRequest(struct http_message* hm);
   MongooseRequest(MongooseRequest&& Other);

   virtual ~MongooseRequest();

   MongooseRequest& operator=(MongooseRequest&& Rhs);

   virtual std::string content() const override;

   virtual std::streambuf* rdbuf() const override;

private:
   std::unique_ptr<std::stringbuf> _body_streambuf;
};

} // http
} // net
} // orion
#endif // ORION_NET_HTTP_MONGOOSEREQUEST_H
