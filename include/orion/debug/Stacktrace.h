//
// Stacktrace.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_DEBUG_STACKTRACE_H
#define ORION_DEBUG_STACKTRACE_H

#include <orion/Orion-Stddefs.h>

// #include <iosfwd>
#include <string>
#include <vector>

namespace orion
{
namespace debug
{

//--------------------------------------------------------------------------------------------------

struct API_EXPORT Frame
{
   uintptr_t address;
   std::string module_name;
   std::string function_name;
   std::string source_file;
   uint64_t source_line;
};

API_EXPORT std::string to_string(const Frame& f);

//--------------------------------------------------------------------------------------------------

class API_EXPORT Stacktrace
{
public:
   using value_type             = std::vector<Frame>::value_type;
   using reference              = std::vector<Frame>::const_reference;
   using const_reference        = std::vector<Frame>::const_reference;
   using size_type              = std::vector<Frame>::size_type;
   using iterator               = std::vector<Frame>::const_iterator;
   using const_iterator         = std::vector<Frame>::const_iterator;
   using reverse_iterator       = std::vector<Frame>::const_reverse_iterator;
   using const_reverse_iterator = std::vector<Frame>::const_reverse_iterator;

   constexpr static const int DEFAULT_MAX_DEPTH = 64;

   /// Default constructor
   Stacktrace() noexcept;

   explicit Stacktrace(std::vector<Frame> frames) noexcept;

   /// Copy constructor
   Stacktrace(const Stacktrace& rhs) = default;

   /// Move constructor
   Stacktrace(Stacktrace&& rhs) noexcept = default;

   ~Stacktrace();

   /// Copy operator
   Stacktrace& operator=(const Stacktrace& rhs) = default;

   /// Move operator
   Stacktrace& operator=(Stacktrace&& rhs) noexcept = default;

   size_type size() const noexcept;

   const_iterator begin() const noexcept;
   const_iterator cbegin() const noexcept;

   const_iterator end() const noexcept;
   const_iterator cend() const noexcept;

   const_reverse_iterator rbegin() noexcept;
   const_reverse_iterator crbegin() noexcept;

   const_reverse_iterator rend() const noexcept;
   const_reverse_iterator crend() const noexcept;

   bool empty() const noexcept;

   const_reference operator[](std::size_t idx) const noexcept;

private:
   std::vector<Frame> _frames;
};

API_EXPORT Stacktrace make_stacktrace(int skip = 0, int max_depth = Stacktrace::DEFAULT_MAX_DEPTH);

API_EXPORT std::string to_string(const Stacktrace& st);

} // namespace debug
} // namespace orion

#endif // ORION_DEBUG_STACKTRACE_H