//
// Utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UTILS_H
#define ORION_UTILS_H

#include <iomanip>
#include <sstream>

namespace orion
{

//-------------------------------------------------------------------------------------------------
   
template <class T, std::size_t N>
inline std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr)
{
   o << "[ "
   << std::setbase(16)
   << std::showbase;
      
   for (const auto& b : arr)
      o << static_cast<uint64_t>(b) << " ";
      
   o << "]";
   return o;
}
   
//-------------------------------------------------------------------------------------------------

inline void write_to_stream(std::ostream& /* stream */)
{
}

template<typename T, typename... Args>
inline void write_to_stream(std::ostream& stream, const T& arg, const Args&... args)
{
   stream << std::boolalpha << arg;
   write_to_stream(stream, args...);
}

//-------------------------------------------------------------------------------------------------

template<typename... Args>
std::string get_as_string(const Args&... args)
{
   std::ostringstream out;

   write_to_stream(out, args...);

   return out.str();
}

//--------------------------------------------------------------------------------------------------

/// 
/// FinalAction allows you to ensure something gets run at the end of a scope
///
template<class F>
class FinalAction
{
public:
   explicit FinalAction(F action) noexcept
      : _action(std::move(action))
      , _invoke(true)
   {
   }

   FinalAction(FinalAction&& other) noexcept
      : _action(std::move(other._action))
      , _invoke(other._invoke)
   {
      other._invoke = false;
   }

   virtual ~FinalAction() noexcept
   {
      if (_invoke)
         _action();
   }

   FinalAction(const FinalAction&) = delete;
   FinalAction& operator=(const FinalAction&) = delete;
   FinalAction& operator=(FinalAction&&) = delete;

protected:
   void dismiss() noexcept { _invoke = false; }

private:
   F _action;
   bool _invoke;
};

template<class F>
inline FinalAction<F> finally(F const& action) noexcept
{
   return FinalAction<F>{action};
}

template<class F>
inline FinalAction<F> finally(F&& action) noexcept
{
   return FinalAction<F>{std::forward<F>(action)};
}

} // namespace orion

#endif /* ORION_UTILS_H */
