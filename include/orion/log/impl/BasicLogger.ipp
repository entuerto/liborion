//
// BasicLogger.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_BASICLOGGER_IPP
#define ORION_LOG_BASICLOGGER_IPP

#include <orion/Common.h>

#include <orion/TypeTraits.h>
#include <orion/log/ExceptionRecord.h>

#include <sstream>

namespace orion
{
namespace log
{

namespace detail
{
struct Concatenate
{
   std::ostringstream& buffer;

   template<typename T>
   void operator()(T& value)
   {
      buffer << value;
   }

   void operator()(std::error_code& ec) 
   {
      buffer << ec.category().name() 
             << ": (Code " 
             << ec.value()
             << ") "
             << ec.message();
   }

   void operator()(const std::error_code& ec) 
   {
      buffer << ec.category().name() 
             << ": (Code " 
             << ec.value()
             << ") "
             << ec.message();
   }

   void operator()(const std::exception& e) 
   {
      buffer << e.what(); 
   }

   void operator()(SourceLocation& /*unused*/) {}
};
} // namespace detail

template<typename Service>
Level BasicLogger<Service>::level() const
{
   return _service.level(_impl);
}

template<typename Service>
void BasicLogger<Service>::level(Level value)
{
   _service.level(_impl, value);
}

template<typename Service>
bool BasicLogger<Service>::is_enabled(Level level) const
{
   return _service.is_enabled(_impl, level);
}

template<typename Service>
void BasicLogger<Service>::add_output_handler(std::unique_ptr<OutputHandler>&& hdl)
{
   _service.add_output_handler(_impl, std::move(hdl));
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::info(Args&&... args)
{
   write(Level::Info, args...);
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::message(Args&&... args)
{
   write(Level::Message, args...);
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::warning(Args&&... args)
{
   write(Level::Warning, args...);
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::error(Args&&... args)
{
   write(Level::Error, args...);
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::debug(Args&&... args)
{
   write(Level::Debug, args...);
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::exception(const std::exception& e, Args&&... args)
{
   auto t = std::make_tuple(args...);

   std::ostringstream buffer(e.what(), std::ios_base::ate);

   get_all_values(detail::Concatenate{buffer}, t);

   auto sl = get_value<SourceLocation>(t, SourceLocation{});

   _service.write(_impl, ExceptionRecord{buffer.str(), SourceLocation{}, sl});
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::exception(const orion::Exception& e, Args&&... args)
{
   auto t = std::make_tuple(args...);

   std::ostringstream buffer(e.what(), std::ios_base::ate);

   get_all_values(detail::Concatenate{buffer}, t);

   auto sl = get_value<SourceLocation>(t, SourceLocation{});

   _service.write(_impl, ExceptionRecord{buffer.str(), e.source_location(), sl});
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::fatal(Args&&... args)
{
   write(Level::Error, args...);
   std::abort();
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::write(Level level, Args&&... args)
{
   auto t = std::forward_as_tuple(args...);

   std::ostringstream buffer;

   get_all_values(detail::Concatenate{buffer}, t);

   auto sl = get_value<SourceLocation>(t, SourceLocation{});

   _service.write(_impl, Record{level, buffer.str(), sl});
}

template<typename Service>
void BasicLogger<Service>::write(const Record& record)
{
   _service.write(_impl, record);
}

template<typename Service>
void BasicLogger<Service>::start(SystemInfoFunc system_info)
{
   _service.start(_impl, system_info);
}

template<typename Service>
void BasicLogger<Service>::shutdown()
{
   _service.shutdown_logger(_impl);
}

template<typename Service>
void BasicLogger<Service>::suspend()
{
   _service.suspend(_impl);
}

template<typename Service>
void BasicLogger<Service>::resume()
{
   _service.resume(_impl);
}

template<typename Service>
void BasicLogger<Service>::push_scope()
{
   _service.push_scope(_impl);
}

template<typename Service>
void BasicLogger<Service>::pop_scope()
{
   _service.pop_scope(_impl);
}

template<typename Service>
uint32_t BasicLogger<Service>::scope_depth() const
{
   return _service.scope_depth(_impl);
}

template<typename Service>
BasicLogger<Service>& BasicLogger<Service>::operator+=(const Record& record)
{
   _service.write(_impl, record);
   return *this;
}

} // namespace log
} // namespace orion

#endif // ORION_LOG_BASICLOGGER_IPP
