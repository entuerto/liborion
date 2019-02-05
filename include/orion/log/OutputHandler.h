//
// OutputHandler.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_OUTPUTHANDLER_H
#define ORION_LOG_OUTPUTHANDLER_H

#include <orion/Common.h>
#include <orion/log/Formatter.h>

#include <iosfwd>
#include <memory>
#include <vector>

namespace orion
{
namespace log
{
class Record;

//--------------------------------------------------------------------------------------------------
// Class OutputHandler

/// OutputHandler instances dispatch logging events to specific destinations.
///
/// Output handlers can optionally use Formatter instances to format
/// records as desired.
///
/// By default, a basic one line formatter is specified.
///
class API_EXPORT OutputHandler
{
public:
   NO_COPY(OutputHandler)
   NO_MOVE(OutputHandler)
   
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

//--------------------------------------------------------------------------------------------------
// OutputHandlers

using OutputHandlers = std::vector<std::unique_ptr<OutputHandler>>;

//--------------------------------------------------------------------------------------------------
// Class StreamOutputHandler

/// Output the log records to a stream
///
/// An output handler class which writes logging records formatted by the specified
/// formatter to a stream.
///
/// Note that this class does not close the stream, as cout or cerr may be used.
///
class API_EXPORT StreamOutputHandler : public OutputHandler
{
public:
   NO_COPY(StreamOutputHandler)
   NO_MOVE(StreamOutputHandler)

   explicit StreamOutputHandler(std::ostream& stream);
   ~StreamOutputHandler() override;

   void write(const Record& record) override;

   void flush() override;

   void close() override;

private:
   std::ostream& _ostream;
};

API_EXPORT std::unique_ptr<StreamOutputHandler> make_stream_output_handler(std::ostream& stream);


} // namespace log
} // namespace orion

#endif /* ORION_LOG_OUTPUTHANDLER_H */
