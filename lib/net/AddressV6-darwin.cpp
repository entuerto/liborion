//
// AddressV6-darwin.cpp
// 
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/AddressV6.h>

#include <algorithm>

#include <orion/StringUtils.h>

namespace orion
{
namespace net
{

std::string to_string(const AddressV6& addr)
{ 
   return "";
}

AddressV6 make_address_v6(const std::string& s)
{
   return AddressV6();
}

} // net
} // orion
