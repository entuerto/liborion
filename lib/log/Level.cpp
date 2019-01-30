//
// Level.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/log/Level.h>

#include <orion/Assert.h>

#include <iostream>

namespace orion
{
namespace log
{

//--------------------------------------------------------------------------
//
std::string to_string(Level level)
{
   switch (level)
   {
      case Level::NotSet:
         return "Not Set   ";
      case Level::Debug3:
         return "Debug3    ";
      case Level::Debug2:
         return "Debug2    ";
      case Level::Debug:
         return "Debug     ";
      case Level::Info:
         return "Info      ";
      case Level::Message:
         return "Message   ";
      case Level::Warning:
         return "Warning   ";
      case Level::Error:
         return "Error     ";
      case Level::Exception:
         return "Exception ";
      case Level::Fatal:
         return "Fatal     ";
      default:
         AssertUnreachable("Invalid logger level");
         break;
   }
   return "";
}

std::ostream& operator<<(std::ostream& o, Level level)
{
   o << to_string(level);
   return o;
}

} // namespace log
} // namespace orion
