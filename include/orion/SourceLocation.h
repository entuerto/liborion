// SourceLocation.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_SOURCELOCATION_H
#define ORION_SOURCELOCATION_H

#include <orion/Orion-Stddefs.h>

#include <iostream>
#include <string>

namespace orion
{

#define _src_loc \
   SourceLocation { __FILE__, __LINE__, __FUNCTION__ }

struct API_EXPORT SourceLocation
{
   std::string file_name;
   int line_number;
   std::string function_name;
};

inline std::ostream& operator<<(std::ostream& out, const SourceLocation& sl)
{
   out << sl.file_name << ":" << sl.line_number;

   if (not sl.function_name.empty())
   {
      out << ":" << sl.function_name;
   }
   return out;
}

} // namespace orion

#endif /* ORION_SOURCELOCATION_H */
