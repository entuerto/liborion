//
// OnelineWithSourceInfoFormatter.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_ONELINEWITHSOURCEINFOFORMATTER_H
#define ORION_LOG_ONELINEWITHSOURCEINFOFORMATTER_H

#include <orion/Orion-Stddefs.h>
#include <orion/log/Formatter.h>

#include <string>

namespace orion
{
namespace log
{
class Record;

//! One line log formatter
/*!
    Formats the log record on a single line with source line
    information.
 */
class API_EXPORT OnelineWithSourceInfoFormatter : public Formatter
{
public:
   OnelineWithSourceInfoFormatter();
   virtual ~OnelineWithSourceInfoFormatter();

   virtual std::string format(const Record& record);
};

} // namespace log
} // namespace orion
#endif /* ORION_LOG_ONELINEWITHSOURCEINFOFORMATTER_H */
