// ExceptionRecord.h
//
// Copyright 2017 Tomas Palazuelos <tomas.plzls@gmail.com>
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//

#ifndef ORION_LOG_EXCEPTIONRECORD_H
#define ORION_LOG_EXCEPTIONRECORD_H

#include <orion/Orion-Stddefs.h>

#include <orion/SourceLocation.h>
#include <orion/log/Record.h>

namespace orion
{
namespace log
{

class API_EXPORT ExceptionRecord : public Record
{
public:
   ExceptionRecord(const std::string& msg,
                   const SourceLocation& thrown_src_loc,
                   const SourceLocation& caught_src_loc);

   ExceptionRecord(const ExceptionRecord& rhs) = delete;
   ExceptionRecord(ExceptionRecord&& rhs);

   virtual ~ExceptionRecord() = default;

   ExceptionRecord& operator=(const ExceptionRecord& rhs) = delete;
   ExceptionRecord& operator=(ExceptionRecord&& rhs);

   //! Returns the source location recorded when the exception is thrown
   const SourceLocation& thrown_source_location() const;

   //! Returns the source location recorded when the exception is caught
   const SourceLocation& caught_source_location() const;

private:
   SourceLocation _caught_src_location;
};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_EXCEPTIONRECORD_H */
