//
// Function.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_FUNCTION_H
#define ORION_LOG_FUNCTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/SourceLocation.h>
#include <orion/log/Level.h>
#include <string>

namespace orion
{
namespace log
{
//! A Function represents an function being logged.
/*!
 */
class API_EXPORT Function
{
public:
   Function(Level level, const SourceLocation& src_loc);

   Function(const Function& rhs);
   Function(Function&& rhs);

   virtual ~Function();

   //! Returns the level of the log record
   virtual Level level() const;

   //! Records the level of the log record
   virtual void level(Level level);

   //! Returns the function name recorded
   virtual std::string name() const;

   //! Records a function name
   virtual void name(const std::string& value);

   Function& operator=(const Function& rhs);
   Function& operator=(Function&& rhs);

private:
   Level _level;
   std::string _name;
};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_FUNCTION_H */
