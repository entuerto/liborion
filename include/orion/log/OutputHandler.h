//
// OutputHandler.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_OUTPUTHANDLER_H
#define ORION_LOG_OUTPUTHANDLER_H

#include <orion/Orion-Stddefs.h>
#include <orion/log/Formatter.h>

#include <memory>
#include <vector>

namespace orion
{
namespace log
{
class Record;

//--------------------------------------------------------------------------
// Class OutputHandler

/// OutputHandler instances dispatch logging events to specific destinations.
///
/// Output handlers can optionally use Formatter instances to format
/// records as desired.

/// By default, a basic one line formatter is specified.
///
class API_EXPORT OutputHandler
{
public:
   OutputHandler();
   virtual ~OutputHandler();

   /// Gets the formatter of the output handler
   Formatter* formatter() const;

   /// Sets the formatter of the output handler.
   void set_formatter(std::unique_ptr<Formatter>&& formatter);

   /// Writes a log record
   virtual void write(const Record& record) = 0;

   /// Flushes the stream
   virtual void flush() = 0;

   /// Closes the output handler
   virtual void close() = 0;

private:
   std::unique_ptr<Formatter> _formatter;
};

typedef std::vector<std::unique_ptr<OutputHandler>> OutputHandlers;

} // namespace log
} // namespace orion

#endif /* ORION_LOG_OUTPUTHANDLER_H */
