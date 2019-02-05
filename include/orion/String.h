//
// String.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_STRING_H
#define ORION_STRING_H

#include <orion/Config.h>

#include <orion/string/Compare.h>
#include <orion/string/Finder.h>
#include <orion/string/Join.h>
#include <orion/string/Split.h>
#include <orion/string/Transform.h>
#include <orion/string/Trim.h>

namespace orion
{

template<typename StringT>
inline StringT plural(std::size_t n, const StringT& s, const StringT& p)
{
   return (n > 1) ? p : s;
}

} // namespace orion
#endif // ORION_STRING_H
