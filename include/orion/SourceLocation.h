//
// SourceLocation.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
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

struct SourceLocation
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
