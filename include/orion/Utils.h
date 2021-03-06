//
// Utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UTILS_H
#define ORION_UTILS_H

#include <orion/Common.h>

#include <array>
#include <cstring>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

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
// Orion
namespace orion
{

//-------------------------------------------------------------------------------------------------

template<typename Container>
inline constexpr Container& operator+=(Container& lhs, const Container& rhs)
{
   std::copy(std::begin(rhs), std::end(rhs), std::back_inserter(lhs));
   return lhs;
}

template<typename Container, typename Container2>
inline constexpr Container& operator+=(Container& lhs, const Container2& rhs)
{
   std::copy(std::begin(rhs), std::end(rhs), std::back_inserter(lhs));
   return lhs;
}

//-------------------------------------------------------------------------------------------------

template<typename... T>
constexpr std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>
make_array(T&&... t)
{
   return std::array<typename std::decay<typename std::common_type<T...>::type>::type,
                     sizeof...(T)>{{std::forward<T>(t)...}};
}

//-------------------------------------------------------------------------------------------------

template<typename T, size_t N>
inline constexpr size_t array_size(T (&)[N])
{
   return N;
}

template<typename T, size_t N>
inline constexpr size_t str_size(T (&)[N])
{
   return N - 1;
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

// abseil
// 
// implicit_cast()
//
// Performs an implicit conversion between types following the language
// rules for implicit conversion; if an implicit conversion is otherwise
// allowed by the language in the given context, this function performs such an
// implicit conversion.
//
// Example:
//
//   // If the context allows implicit conversion:
//   From from;
//   To to = from;
//
//   // Such code can be replaced by:
//   implicit_cast<To>(from);
//
// An `implicit_cast()` may also be used to annotate numeric type conversions
// that, although safe, may produce compiler warnings (such as `long` to `int`).
// Additionally, an `implicit_cast()` is also useful within return statements to
// indicate a specific implicit conversion is being undertaken.
//
// Example:
//
//   return implicit_cast<double>(size_in_bytes) / capacity_;
//
template<typename To>
inline To implicit_cast(typename details::identity_t<To> to)
{
   return to;
}

namespace internal_casts
{
template<class Dest, class Source>
struct is_bitcastable
   : std::integral_constant<
        bool,
        sizeof(Dest) == sizeof(Source) and std::is_trivially_copyable<Source>::value and
           std::is_trivially_copyable<Dest>::value and std::is_default_constructible<Dest>::value>
{
};

} // namespace internal_casts

// bit_cast()
//
// Performs a bitwise cast on a type without changing the underlying bit
// representation of that type's value. The two types must be of the same size
// and both types must be trivially copyable. As with most casts, use with
// caution. A `bit_cast()` might be needed when you need to temporarily treat a
// type as some other type, such as in the following cases:
//
//    * Serialization (casting temporarily to `char *` for those purposes is
//      always allowed by the C++ standard)
//    * Managing the individual bits of a type within mathematical operations
//      that are not normally accessible through that type
//    * Casting non-pointer types to pointer types (casting the other way is
//      allowed by `reinterpret_cast()` but round-trips cannot occur the other
//      way).
//
// Example:
//
//   float f = 3.14159265358979;
//   int i = bit_cast<int32_t>(f);
//   // i = 0x40490fdb
//
// Casting non-pointer types to pointer types and then dereferencing them
// traditionally produces undefined behavior.
//
// Example:
//
//   // WRONG
//   float f = 3.14159265358979;            // WRONG
//   int i = * reinterpret_cast<int*>(&f);  // WRONG
//
// The address-casting method produces undefined behavior according to the ISO
// C++ specification section [basic.lval]. Roughly, this section says: if an
// object in memory has one type, and a program accesses it with a different
// type, the result is undefined behavior for most values of "different type".
//
// Such casting results in type punning: holding an object in memory of one type
// and reading its bits back using a different type. A `bit_cast()` avoids this
// issue by implementing its casts using `memcpy()`, which avoids introducing
// this undefined behavior.
//
template<
   typename Dest,
   typename Source,
   typename std::enable_if<internal_casts::is_bitcastable<Dest, Source>::value, int>::type = 0>
inline Dest bit_cast(const Source& source)
{
   Dest dest;
   std::memcpy(static_cast<void*>(std::addressof(dest)),
               static_cast<const void*>(std::addressof(source)),
               sizeof(dest));
   return dest;
}

} // namespace orion

#endif /* ORION_UTILS_H */
