//
// Level.h
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_LOG_LEVEL_H
#define ORION_LOG_LEVEL_H

#include <orion/Orion-Stddefs.h>

#include <iosfwd>
#include <string>

namespace orion
{
namespace log
{

enum class Level : uint16_t
{
   NotSet    = 0,
   Debug3    = 10,
   Debug2    = 20,
   Debug     = 30,
   Info      = 40,
   Message   = 50,
   Warning   = 60,
   Error     = 70,
   Exception = 80
};

API_EXPORT std::string to_string(Level level);

API_EXPORT std::ostream& operator<<(std::ostream& o, Level level);

} // namespace log
} // namespace orion

#endif // ORION_LOG_LEVEL_H
