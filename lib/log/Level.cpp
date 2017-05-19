//
// Level.cpp
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#include <orion/log/Level.h>

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
      default:
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
