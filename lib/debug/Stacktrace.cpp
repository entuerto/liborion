//
// Stacktrace.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/debug/Stacktrace.h>

#include <fmt/format.h>

#include <sstream>

namespace orion
{
namespace debug
{

//--------------------------------------------------------------------------------------------------
// Frame

std::string to_string(const Frame& f)
{
   return fmt::format("{:#018x}, {}!{}, {}:{}",
                      f.address,
                      (not f.module_name.empty() ? f.module_name : "[unknown module]"),
                      (not f.function_name.empty() ? f.function_name : "[unknown function]"),
                      (not f.source_file.empty() ? f.source_file : "[unknown file]"),
                      f.source_line);
}

//--------------------------------------------------------------------------------------------------
// Stacktrace

Stacktrace::Stacktrace() noexcept
   : _frames()
{
}

Stacktrace::Stacktrace(std::vector<Frame> frames) noexcept
   : _frames(std::move(frames))
{
}

Stacktrace::~Stacktrace()
{
}

Stacktrace::size_type Stacktrace::size() const noexcept
{
   return _frames.size();
}

Stacktrace::const_iterator Stacktrace::begin() const noexcept
{
   return _frames.begin();
}

Stacktrace::const_iterator Stacktrace::cbegin() const noexcept
{
   return _frames.cbegin();
}

Stacktrace::const_iterator Stacktrace::end() const noexcept
{
   return _frames.end();
}

Stacktrace::const_iterator Stacktrace::cend() const noexcept
{
   return _frames.cend();
}

Stacktrace::const_reverse_iterator Stacktrace::rbegin() noexcept
{
   return _frames.rbegin();
}

Stacktrace::const_reverse_iterator Stacktrace::crbegin() noexcept
{
   return _frames.crbegin();
}

Stacktrace::const_reverse_iterator Stacktrace::rend() const noexcept
{
   return _frames.rend();
}

Stacktrace::const_reverse_iterator Stacktrace::crend() const noexcept
{
   return _frames.crend();
}

bool Stacktrace::empty() const noexcept
{
   return _frames.empty();
}

Stacktrace::const_reference Stacktrace::operator[](std::size_t idx) const noexcept
{
   return _frames.at(idx);
}

//--------------------------------------------------------------------------------------------------

std::string to_string(const Stacktrace& st)
{
   std::ostringstream ostream;

   for (auto& frame : st)
   {
      ostream << to_string(frame) << "\n";
   }

   return ostream.str();
}

} // namespace debug
} // namespace orion
