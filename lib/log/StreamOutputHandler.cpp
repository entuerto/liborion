//
// StreamOutputHandler.cpp
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#include <iostream>

#include <orion/log/Formatter.h>
#include <orion/log/StreamOutputHandler.h>

namespace orion
{
namespace log
{

StreamOutputHandler::StreamOutputHandler(std::ostream& stream) :
   OutputHandler(),
   _ostream(stream)
{
}

StreamOutputHandler::~StreamOutputHandler() = default;

void StreamOutputHandler::write(const Record& record)
{
   _ostream << formatter()->format(record)
            << std::endl;
}

void StreamOutputHandler::flush()
{
   _ostream << std::flush;;
}

void StreamOutputHandler::close()
{
   // Do not close stream in case cout or cerr is used.
}

} // namespace log
} // namespace orion
