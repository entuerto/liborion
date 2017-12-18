//
// OutputHandler.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#include <orion/log/OutputHandler.h>

#include <orion/log/Formatter.h>

#include <iostream>

namespace orion
{
namespace log
{
//--------------------------------------------------------------------------------------------------
// Class OutputHandler

OutputHandler::OutputHandler()
   : _formatter(std::make_unique<OnelineFormatter>())
{
}

OutputHandler::~OutputHandler() = default;

Formatter* OutputHandler::formatter() const
{
   return _formatter.get();
}

///
/// the output handler takes ownership of the formatter.
/// @param formatter  Formatter for the output
/// 
void OutputHandler::set_formatter(std::unique_ptr<Formatter>&& formatter)
{
   _formatter = std::move(formatter);
}

//--------------------------------------------------------------------------------------------------
// Class StreamOutputHandler

StreamOutputHandler::StreamOutputHandler(std::ostream& stream)
   : _ostream(stream)
{
}

StreamOutputHandler::~StreamOutputHandler() = default;

void StreamOutputHandler::write(const Record& record)
{
   _ostream << formatter()->format(record) << std::endl;
}

void StreamOutputHandler::flush()
{
   _ostream << std::flush;
}

void StreamOutputHandler::close()
{
   // Do not close stream in case cout or cerr is used.
}

std::unique_ptr<StreamOutputHandler> make_stream_output_handler(std::ostream& stream)
{
   return std::make_unique<StreamOutputHandler>(stream);
}

} // namespace log
} // namespace orion
