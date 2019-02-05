//
// ExceptionRecord.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_EXCEPTIONRECORD_H
#define ORION_LOG_EXCEPTIONRECORD_H

#include <orion/Common.h>

#include <orion/log/Record.h>

namespace orion
{
namespace log
{

class API_EXPORT ExceptionRecord : public Record
{
public:
   NO_COPY(ExceptionRecord)

   ExceptionRecord(const std::string& msg,
                   const SourceLocation& thrown_src_loc,
                   const SourceLocation& caught_src_loc);

   ExceptionRecord(ExceptionRecord&& rhs) noexcept;

   ~ExceptionRecord() override = default;

   ExceptionRecord& operator=(ExceptionRecord&& rhs) noexcept;

   /// Returns the source location recorded when the exception is thrown
   const SourceLocation& thrown_source_location() const;

   /// Returns the source location recorded when the exception is caught
   const SourceLocation& caught_source_location() const;

private:
   SourceLocation _caught_src_location;
};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_EXCEPTIONRECORD_H */
