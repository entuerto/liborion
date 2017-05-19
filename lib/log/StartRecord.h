// StartRecord.h
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#ifndef ORION_LOG_STARTRECORD_H
#define ORION_LOG_STARTRECORD_H

#include <orion/log/Record.h>

#include <string>

namespace orion
{
namespace log
{
//! First Record sent when logging
/*!
    This Record tells the user that the logging
    process is about to start.
 */
class StartRecord : public Record
{
public:
   StartRecord();

   StartRecord(const StartRecord& rhs);
   StartRecord(StartRecord&& rhs);

   virtual ~StartRecord();

   virtual std::string message() const override;

   StartRecord& operator=(const StartRecord& rhs);
   StartRecord& operator=(StartRecord&& rhs);
};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_STARTRECORD_H */
