//
// Utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_UTILS_H
#define ORION_NET_UTILS_H

#include <orion/Config.h>

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

} // namespace net
} // namespace orion

#endif // ORION_NET_UTILS_H
