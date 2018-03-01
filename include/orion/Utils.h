//
// Utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UTILS_H
#define ORION_UTILS_H

#include <fmt/format.h>

#include <iomanip>
#include <sstream>
#include <tuple>
#include <typeinfo>

#undef assert

namespace orion
{
//-------------------------------------------------------------------------------------------------

struct Version
{
   int major;
   int minor;
};

constexpr bool operator==(const Version& v1, const Version& v2) noexcept
{
   return v1.major == v2.major and v1.minor == v2.minor;
}

constexpr bool operator!=(const Version& v1, const Version& v2) noexcept
{
   return not operator==(v1, v2);
}

constexpr bool operator<(const Version& v1, const Version& v2) noexcept
{
   if (v1.major != v2.major)
      return v1.major < v2.major;

   if (v1.minor != v2.minor)
      return v1.minor < v2.minor;

   return false;
}

constexpr bool operator>(const Version& v1, const Version& v2) noexcept
{
   if (v1.major != v2.major)
      return v1.major > v2.major;

   if (v1.minor != v2.minor)
      return v1.minor > v2.minor;

   return false;
}

constexpr bool operator<=(const Version& v1, const Version& v2) noexcept
{
   if (v1.major != v2.major)
      return v1.major < v2.major;

   if (v1.minor != v2.minor)
      return v1.minor < v2.minor;

   return true;
}

constexpr bool operator>=(const Version& v1, const Version& v2) noexcept
{
   if (v1.major != v2.major)
      return v1.major > v2.major;

   if (v1.minor != v2.minor)
      return v1.minor > v2.minor;

   return true;
}

inline void format_arg(fmt::BasicFormatter<char>& f, const char*& /*fmt_str*/, const Version& v)
{
   f.writer().write("{}.{}", v.major, v.minor);
}

inline std::istream& operator>>(std::istream& s, Version& v)
{
   float tmp;

   s >> tmp;

   v.major = static_cast<int>(tmp);
   v.minor = (tmp - v.major) * 100;

   return s;
}

inline std::ostream& operator<<(std::ostream& s, const Version& v)
{
   s << v.major
     << "."
     << v.minor;

   return s;
}

//-------------------------------------------------------------------------------------------------

template<typename... Ts>
struct List
{
   using Type = List<Ts...>;

   enum
   {
      size = sizeof...(Ts)
   };
};

template<int I>
using IntType = std::integral_constant<int, I>;

namespace detail
{
template<int I, typename T, typename... Ts>
struct Find : IntType<-1>
{
};

template<int I, typename T, typename U, typename... Ts>
struct Find<I, T, U, Ts...> : Find<I + 1, T, Ts...>
{
};

template<int I, typename T, typename... Ts>
struct Find<I, T, T, Ts...> : IntType<I>
{
};

template<typename T, typename U>
struct Max : IntType<(U::value > T::value) ? U::value : T::value>
{
};

template<int I, typename T, typename... Ts>
struct FindLast : IntType<-1>
{
};

template<int I, typename T, typename U, typename... Ts>
struct FindLast<I, T, U, Ts...> : FindLast<I + 1, T, Ts...>
{
};

template<int I, typename T, typename... Ts>
struct FindLast<I, T, T, Ts...> : Max<IntType<I>, FindLast<I + 1, T, Ts...>>
{
};
} // namespace detail

//-------------------------------------------------------------------------------------------------

template<typename T, typename U>
struct Find;

template<typename T, typename... Ts>
struct Find<T, List<Ts...>> : detail::Find<0, T, Ts...>
{
};

template<typename T, typename U>
struct FindLast;

template<typename T, typename... Ts>
struct FindLast<T, List<Ts...>> : detail::FindLast<0, T, Ts...>
{
};

//-------------------------------------------------------------------------------------------------

template<class T,
         class... Args,
         typename std::enable_if<Find<T, List<Args...>>() == -1, bool>::type = true>
bool has_type(std::tuple<Args...>& /* tup */) noexcept
{
   return false;
}

template<class T,
         class... Args,
         typename std::enable_if<Find<T, List<Args...>>() != -1, bool>::type = true>
bool has_type(std::tuple<Args...>& /* tup */) noexcept
{
   constexpr int idx = Find<T, List<Args...>>();

   return idx >= 0;
}

//-------------------------------------------------------------------------------------------------

template<class T,
         class... Args,
         typename std::enable_if<Find<T, List<Args...>>() == -1, bool>::type = true>
const T& get_value(std::tuple<Args...>& /* tup */, const T& def) noexcept
{
   return def;
}

template<class T,
         class... Args,
         typename std::enable_if<Find<T, List<Args...>>() != -1, bool>::type = true>
const T& get_value(std::tuple<Args...>& tup, const T& /* def */) noexcept
{
   constexpr int idx = Find<T, List<Args...>>();

   return std::get<idx>(tup);
}

//-------------------------------------------------------------------------------------------------

template<class T,
         class... Args,
         typename std::enable_if<FindLast<T, List<Args...>>() == -1, bool>::type = true>
const T& get_last_value(std::tuple<Args...>& /* tup */, const T& def) noexcept
{
   return def;
}

template<class T,
         class... Args,
         typename std::enable_if<FindLast<T, List<Args...>>() != -1, bool>::type = true>
const T& get_last_value(std::tuple<Args...>& tup, const T& /* def */) noexcept
{
   constexpr int idx = FindLast<T, List<Args...>>();

   return std::get<idx>(tup);
}

//-------------------------------------------------------------------------------------------------

template<class... Args, typename Function, std::size_t... Indices>
constexpr void get_all_impl(Function&& f,
                            std::tuple<Args...>& t,
                            std::index_sequence<Indices...> /*unused*/)
{
   using swallow = int[];
   static_cast<void>(swallow{0, (std::forward<Function>(f)(std::get<Indices>(t)), void(), 0)...});
}

template<typename Function, class... Args>
constexpr void get_all_values(Function&& f, std::tuple<Args...>& t)
{
   return get_all_impl(std::forward<Function>(f),
                       t,
                       std::make_index_sequence<std::tuple_size<std::tuple<Args...>>::value>{});
}

//-------------------------------------------------------------------------------------------------
   
template <class T, std::size_t N>
std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr)
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

//-------------------------------------------------------------------------------------------------

/// The name for a given type
///
template<typename T>
std::string type_name()
{
   return typeid(T).name();
}

/// The name for a given object based on its type
///
template<typename T>
std::string type_name(const T& ob)
{
   return typeid(ob).name();
}

/// The name for a given object based on its type
///
inline std::string type_name(std::exception_ptr eptr)
{
   try
   {
      if (eptr)
      {
         std::rethrow_exception(eptr);
      }
   }
   catch (const std::exception& e)
   {
      return typeid(e).name();
   }
   catch (...)
   {
   }

   return typeid(eptr).name();
}

} // namespace orion

#endif /* ORION_UTILS_H */
