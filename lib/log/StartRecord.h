//
// StartRecord.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#ifndef ORION_LOG_STARTRECORD_H
#define ORION_LOG_STARTRECORD_H

#include <orion/Chrono.h>
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
   virtual ~StartRecord() = default;

   virtual std::string message() const override;
};

//-------------------------------------------------------------------------------------------------

std::string StartRecord::message() const
{
   auto now = std::chrono::system_clock::now();

   return "\nLog Start Time - " + orion::to_string(now, "%c");
}

} // namespace log
} // namespace orion

#endif /* ORION_LOG_STARTRECORD_H */
