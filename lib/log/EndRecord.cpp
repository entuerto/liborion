// EndRecord.cpp
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// Copyright 2016 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <log/EndRecord.h>

#include <orion/DateUtils.h>

namespace orion
{
namespace log
{
//----------------------------------------------------------------------------
// EndRecord
/*!
   Constructor
 */
EndRecord::EndRecord()
   : Record()
{
}

/*
   Copy constructor
 */
EndRecord::EndRecord(const EndRecord& rhs)
   : Record(rhs)
{
}

EndRecord::EndRecord(EndRecord&& rhs)
   : Record(std::move(rhs))
{
}

EndRecord::~EndRecord()
{
}

/*!
   Returns the message recorded
 */
std::string EndRecord::message() const
{
   auto now = std::chrono::system_clock::now();

   return "\nLog End Time - " + orion::to_string(now, "%c");
}

EndRecord& EndRecord::operator=(const EndRecord& rhs)
{
   if (this == &rhs)
      return *this;

   Record::operator=(rhs);

   return *this;
}

EndRecord& EndRecord::operator=(EndRecord&& rhs)
{
   Record::operator=(std::move(rhs));

   return *this;
}

} // namespace log
} // namespace orion
