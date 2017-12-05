//
// OutputHandler.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#include <orion/log/OnelineFormatter.h>
#include <orion/log/OutputHandler.h>

namespace orion
{
namespace log
{
//--------------------------------------------------------------------------
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

} // namespace log
} // namespace orion
