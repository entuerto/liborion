// SystemInfoRecord.cpp
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

#include <log/SystemInfoRecord.h>

namespace orion
{
namespace log
{
//----------------------------------------------------------------------------
// SystemInfoRecord
/*!
   Constructor
 */
SystemInfoRecord::SystemInfoRecord() :
   Record()
{
}

SystemInfoRecord::SystemInfoRecord(const std::string& msg) :
   Record(Level::NotSet, msg)
{
}

/*
   Copy constructor
 */
SystemInfoRecord::SystemInfoRecord(const SystemInfoRecord& rhs) :
   Record(rhs)
{
}

SystemInfoRecord::SystemInfoRecord(SystemInfoRecord&& rhs) :
   Record(std::move(rhs))
{
}

SystemInfoRecord::~SystemInfoRecord()
{
}

SystemInfoRecord& SystemInfoRecord::operator=(const SystemInfoRecord& rhs)
{
   if (this == &rhs)
      return *this;

   Record::operator=(rhs);

   return *this;
}

SystemInfoRecord& SystemInfoRecord::operator=(SystemInfoRecord&& rhs)
{
   Record::operator=(std::move(rhs));

   return *this;
}

} // namespace log
} // namespace orion
