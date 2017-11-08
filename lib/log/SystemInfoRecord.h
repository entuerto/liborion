//
// SystemInfoRecord.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#ifndef ORION_LOG_SYSTEMINFORECORD_H
#define ORION_LOG_SYSTEMINFORECORD_H

#include <orion/log/Record.h>

#include <string>

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
   SystemInfoRecord(const std::string& msg) 
      : Record(Level::NotSet, msg) {}

   virtual ~SystemInfoRecord() = default;

};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_SYSTEMINFORECORD_H */
