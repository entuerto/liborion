//
// Utils.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_UTILS_IPP
#define ORION_NET_TCP_UTILS_IPP

namespace orion
{
namespace net
{
namespace tcp
{

//-------------------------------------------------------------------------------------------------
// Handler implementation

inline constexpr Handler::State Handler::state() const
{
   return _state;
}

inline constexpr void Handler::state(Handler::State value)
{
   _state = value;
}

inline constexpr bool Handler::read_wanted() const 
{ 
   return _state == State::Read; 
}

inline constexpr bool Handler::write_wanted() const 
{ 
   return _state == State::Write; 
}

inline constexpr bool Handler::should_stop() const  
{ 
   return _state == State::Closed; 
}

inline void Handler::signal_write()
{
   _state = State::Write;
}

} // tcp
} // net
} // orion

#endif // ORION_NET_TCP_UTILS_IPP
