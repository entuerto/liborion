//
// StreamOutputHandler.h
//
// Created by Tomas Palazuelos on 2017-04.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_LOG_STREAMOUTPUTHANDLER_H
#define ORION_LOG_STREAMOUTPUTHANDLER_H

#include <iosfwd>

#include <orion/Orion-Stddefs.h>
#include <orion/log/OutputHandler.h>

namespace orion
{
namespace log
{
class Formatter;
class Record;

//--------------------------------------------------------------------------
// Class StreamOutputHandler

//! Output the log records to a stream
/*!
    An output handler class which writes logging records formatted by the specified
    formatter to a stream.

    Note that this class does not close the stream, as cout or cerr may be used.
 */
class API_EXPORT StreamOutputHandler : public OutputHandler
{
public:
   NO_COPY(StreamOutputHandler)
   NO_MOVE(StreamOutputHandler)
   
   StreamOutputHandler(std::ostream& stream);
   virtual ~StreamOutputHandler();

   virtual void write(const Record& record) override;

   virtual void flush() override;

   virtual void close() override;

private:
   std::ostream& _ostream;
};

} // namespace log
} // namespace orion
#endif /* ORION_LOG_STREAMOUTPUTHANDLER_H */
