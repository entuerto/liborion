//
// Function.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_FUNCTION_H
#define ORION_LOG_FUNCTION_H

#include <orion/Common.h>

#include <orion/log/Level.h>

#include <string>

namespace orion
{
namespace log
{
/// 
/// A Function represents an function being logged.
///
class API_EXPORT Function
{
public:
   Function(Level level, const SourceLocation& src_loc);

   Function(const Function& rhs) = default;
   Function(Function&& rhs) noexcept;

   virtual ~Function();

   /// Returns the level of the log record
   Level level() const;

   /// Records the level of the log record
   void level(Level level);

   /// Returns the function name recorded
   std::string name() const;

   /// Records a function name
   void name(const std::string& value);

   Function& operator=(const Function& rhs);
   Function& operator=(Function&& rhs) noexcept;

private:
   Level _level;
   std::string _name;
};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_FUNCTION_H */
