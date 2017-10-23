//
// Utils.h
//
//  Created by Tomas Palazuelos on 2017-10-13.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_UTILS_H
#define ORION_NET_UTILS_H

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <initializer_list>
#include <string>
#include <vector>

namespace orion
{
namespace net
{
//-------------------------------------------------------------------------------------------------

struct Parameter
{
   std::string key;
   std::string value;
};

class Parameters
{
public:
   Parameters() = default;
   Parameters(const std::initializer_list<Parameter>& p)
      : _params(p)
   {
   }

   void add(const Parameter& p) { _params.push_back(p); }

   std::vector<Parameter> _params;
};

//-------------------------------------------------------------------------------------------------

struct Timeout
{
   std::chrono::milliseconds ms;

   constexpr operator std::chrono::milliseconds() const noexcept { return ms; }
};

} // net
} // orion

#endif // ORION_NET_UTILS_H
