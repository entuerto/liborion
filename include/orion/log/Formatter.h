// Formatter.h
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_LOG_FORMATTER_H
#define ORION_LOG_FORMATTER_H

#include <orion/Orion-Stddefs.h>
#include <string>

namespace orion
{
namespace log
{
class Record;

//! A Formatter is used to convert a Record to text.
/*!
   A Formatter class is responsible for converting a Record to
   a readable form that will be interpreted by either a human or
   an external system.
 */
class API_EXPORT Formatter
{
public:
   virtual ~Formatter() = default;

   //! Formats the Record into a reable form
   virtual std::string format(const Record& record) = 0;
};

} // namespace log
} // namespace orion
#endif /* ORION_LOG_FORMATTER_H */
