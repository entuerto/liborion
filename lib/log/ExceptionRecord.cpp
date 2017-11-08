//
// ExceptionRecord.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/log/ExceptionRecord.h>

#include <orion/log/Level.h>

namespace orion
{
namespace log
{
//----------------------------------------------------------------------------
// ExceptionRecord
/*!
   Constructor
 */
ExceptionRecord::ExceptionRecord(const std::string& msg,
                                 const SourceLocation& thrown_src_loc,
                                 const SourceLocation& caught_src_loc)
   : Record(Level::Exception, msg, thrown_src_loc)
   , _caught_src_location(caught_src_loc)
{
}

/*
   Copy constructor
 */
//ExceptionRecord::ExceptionRecord(const ExceptionRecord& rhs)
//   : Record(rhs)
//   , _caught_src_location(rhs._caught_src_location)
//{
//}

ExceptionRecord::ExceptionRecord(ExceptionRecord&& rhs)
   : Record(std::move(rhs))
   , _caught_src_location(std::move(rhs._caught_src_location))
{
}

//ExceptionRecord& ExceptionRecord::operator=(const ExceptionRecord& rhs)
//{
//   if (this == &rhs)
//      return *this;
//
//   Record::operator=(rhs);
//
//   _caught_src_location = rhs._caught_src_location;
//
//   return *this;
//}

ExceptionRecord& ExceptionRecord::operator=(ExceptionRecord&& rhs)
{
   _caught_src_location = std::move(rhs._caught_src_location);

   Record::operator=(std::move(rhs));

   return *this;
}

const SourceLocation& ExceptionRecord::thrown_source_location() const
{
   return Record::source_location();
}

const SourceLocation& ExceptionRecord::caught_source_location() const
{
   return _caught_src_location;
}

} // namespace log
} // namespace orion
