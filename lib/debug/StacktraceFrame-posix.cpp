//
// StacktraceFrame-posix.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/debug/Stacktrace.h>

#include <debug/ModuleCache.h>

namespace orion
{
namespace debug
{
//--------------------------------------------------------------------------------------------------
// Frame

std::string Frame::module_name() const
{
   ModuleCache& cache = ModuleCache::instance();

   Module* module = cache.find(_address);

   return (module != nullptr) ? std::string{module->file_name.begin(), module->file_name.end()}
                              : "";
}

std::string Frame::function_name() const
{
   ModuleCache& cache = ModuleCache::instance();

   Module* module = cache.find(_address);

   if (module == nullptr)
      return "";

   debug::LocationInfo info;

   module->find_symbol(_address, info);

   return info.function_name;
}

std::string Frame::source_file() const
{
   ModuleCache& cache = ModuleCache::instance();

   Module* module = cache.find(_address);

   if (module == nullptr)
      return "";

   debug::LocationInfo info;

   module->find_symbol(_address, info);

   return info.file_name;
}

uint64_t Frame::source_line() const
{
   ModuleCache& cache = ModuleCache::instance();

   Module* module = cache.find(_address);

   if (module == nullptr)
      return 0;

   debug::LocationInfo info;

   module->find_symbol(_address, info);

   return info.line;
}

} // namespace debug
} // namespace orion
