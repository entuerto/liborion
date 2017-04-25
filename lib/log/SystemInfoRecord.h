// SystemInfoRecord.h
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

#ifndef ORION_LOG_SYSTEMINFORECORD_H
#define ORION_LOG_SYSTEMINFORECORD_H

#include <string>

#include <orion/log/Record.h>

namespace orion
{
namespace log
{
//! Record that contains the system information to print
/*!
    Record that contains the system information to print.
    The record is sent when the logger is started.
 */
class SystemInfoRecord : public Record
{
public:
   SystemInfoRecord();

   SystemInfoRecord(const std::string& msg);

   SystemInfoRecord(const SystemInfoRecord& rhs);
   SystemInfoRecord(SystemInfoRecord&& rhs);

   virtual ~SystemInfoRecord();

   SystemInfoRecord& operator=(const SystemInfoRecord& rhs);
   SystemInfoRecord& operator=(SystemInfoRecord&& rhs);

};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_SYSTEMINFORECORD_H */
