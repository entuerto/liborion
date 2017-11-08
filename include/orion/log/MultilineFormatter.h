//
// MultilineFormatter.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_MULTILINEFORMATTER_H
#define ORION_LOG_MULTILINEFORMATTER_H

#include <orion/Orion-Stddefs.h>
#include <orion/SourceLocation.h>
#include <orion/log/Formatter.h>

#include <string>

namespace orion
{
namespace log
{
class Record;

//! Multi line formatter
/*!
    Formats the information on more than one line.
 */
class API_EXPORT MultilineFormatter : public Formatter
{
public:
   MultilineFormatter();
   virtual ~MultilineFormatter();

   virtual std::string format(const Record& record);

private:
   std::string format_source_info(const std::string& prefix, const SourceLocation& src_loc);
};

} // namespace log
} // namespace orion
#endif /* ORION_LOG_MULTILINEFORMATTER_H */
