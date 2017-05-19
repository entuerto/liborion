// EndRecord.h
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// Copyright 2016 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef ORION_LOG_ENDRECORD_H
#define ORION_LOG_ENDRECORD_H

#include <orion/log/Record.h>

#include <string>

namespace orion
{
namespace log
{
//! Informs the end of the log
/*!
    The last record sent when the logger is shutdown. If this
    log record is not written to the output, the application
    has probably crashed.
 */
class EndRecord : public Record
{
public:
   EndRecord();

   EndRecord(const EndRecord& rhs);
   EndRecord(EndRecord&& rhs);

   virtual ~EndRecord();

   virtual std::string message() const override;

   EndRecord& operator=(const EndRecord& rhs);
   EndRecord& operator=(EndRecord&& rhs);
};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_ENDRECORD_H */
