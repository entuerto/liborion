//
// BasicLogger.ipp
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_LOG_BASICLOGGER_IPP
#define ORION_LOG_BASICLOGGER_IPP

#include <orion/Orion-Stddefs.h>

#include <orion/log/ExceptionRecord.h>
#include <orion/Utils.h>

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

   void operator()(SourceLocation&) {}
};
} // namespace detail

template<typename Service>
Level BasicLogger<Service>::level() const
{
   return this->get_service().level(this->get_implementation());
}

template<typename Service>
void BasicLogger<Service>::level(Level value)
{
   this->get_service().level(this->get_implementation(), value);
}

template<typename Service>
bool BasicLogger<Service>::is_enabled(Level level) const
{
   return this->get_service().is_enabled(this->get_implementation(), level);
}

template<typename Service>
void BasicLogger<Service>::add_output_handler(std::unique_ptr<OutputHandler>&& hdl)
{
   this->get_service().add_output_handler(this->get_implementation(), std::move(hdl));
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

   this->get_service().write(this->get_implementation(),
                             ExceptionRecord{buffer.str(), SourceLocation{}, sl});
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::exception(const orion::Exception& e, Args&&... args)
{
   auto t = std::make_tuple(args...);

   std::ostringstream buffer(e.what(), std::ios_base::ate);

   get_all_values(detail::Concatenate{buffer}, t);

   auto sl = get_value<SourceLocation>(t, SourceLocation{});

   this->get_service().write(this->get_implementation(),
                             ExceptionRecord{buffer.str(), e.source_location(), sl});
}

template<typename Service>
template<typename... Args>
void BasicLogger<Service>::write(Level level, Args&&... args)
{
   auto t = std::forward_as_tuple(args...);

   std::ostringstream buffer;

   get_all_values(detail::Concatenate{buffer}, t);

   auto sl = get_value<SourceLocation>(t, SourceLocation{});

   this->get_service().write(this->get_implementation(), Record{level, buffer.str(), sl});
}

template<typename Service>
void BasicLogger<Service>::write(const Record& record)
{
   this->get_service().write(this->get_implementation(), record);
}

template<typename Service>
void BasicLogger<Service>::start(SystemInfoFunc system_info)
{
   this->get_service().start(this->get_implementation(), system_info);
}

template<typename Service>
void BasicLogger<Service>::shutdown()
{
   this->get_service().shutdown(this->get_implementation());
}

template<typename Service>
void BasicLogger<Service>::suspend()
{
   this->get_service().suspend(this->get_implementation());
}

template<typename Service>
void BasicLogger<Service>::resume()
{
   this->get_service().resume(this->get_implementation());
}

template<typename Service>
void BasicLogger<Service>::push_scope()
{
   this->get_service().push_scope(this->get_implementation());
}

template<typename Service>
void BasicLogger<Service>::pop_scope()
{
   this->get_service().pop_scope(this->get_implementation());
}

template<typename Service>
uint32_t BasicLogger<Service>::scope_depth() const
{
   return this->get_service().scope_depth(this->get_implementation());
}

template<typename Service>
BasicLogger<Service>& BasicLogger<Service>::operator+=(const Record& record)
{
   this->get_service().write(this->get_implementation(), record);
   return *this;
}

} // namespace log
} // namespace orion

#endif // ORION_LOG_BASICLOGGER_IPP
