//
// Types.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_TYPES_H
#define ORION_TYPES_H

#include <orion/Config.h>

#include <ostream>

namespace orion
{

template<typename T>
using IsNotReference = typename std::enable_if<!std::is_reference<T>::value, void>::type;

template<typename T, typename Tag>
class BaseType
{
public:
   explicit BaseType(const T& value)
      : _value(value)
   {
   }

   template<typename T_ = T, typename = IsNotReference<T_>>
   explicit BaseType(T&& value)
      : _value(std::move(value))
   {
   }

   T& get()             { return _value; }
   const T& get() const { return _value; }

   explicit operator const T&() const  { return get(); }
   explicit operator T&()              { return get(); }

   const T* operator->() const { return std::addressof(get()); }
   T* operator->()             { return std::addressof(get()); }

private:
   T _value;
};

//------------------------------------------------------------------------------------------------

template<typename T>
struct TypeName;

/// TypeNameSequence is a utility for rendering sequences of types to a stream
template<typename... ArgTs>
struct TypeNameSequence
{
};

/// Render an empty TypeNameSequence to an ostream.
inline std::ostream& operator<<(std::ostream& os, const TypeNameSequence<>& /* value */)
{
   return os;
}

/// Render a TypeNameSequence of a single type to an ostream.
template<typename ArgT>
inline std::ostream& operator<<(std::ostream& os, const TypeNameSequence<ArgT>& value)
{
   os << TypeName<ArgT>::name();
   return os;
}

/// Render a TypeNameSequence of more than one type to an ostream.
template<typename ArgT1, typename ArgT2, typename... ArgTs>
inline std::ostream& operator<<(std::ostream& os, const TypeNameSequence<ArgT1, ArgT2, ArgTs...>& value)
{
   os << TypeName<ArgT1>::name() << ", " << TypeNameSequence<ArgT2, ArgTs...>();
   return os;
}

template<>
struct TypeName<void>
{
   static std::string name() { return "void"; }
};

template<>
struct TypeName<int8_t>
{
   static std::string name() { return "int8_t"; }
};

template<>
struct TypeName<uint8_t>
{
   static std::string name() { return "uint8_t"; }
};

template<>
struct TypeName<int16_t>
{
   static std::string name() { return "int16_t"; }
};

template<>
struct TypeName<uint16_t>
{
   static std::string name() { return "uint16_t"; }
};

template<>
struct TypeName<int32_t>
{
   static std::string name() { return "int32_t"; }
};

template<>
struct TypeName<uint32_t>
{
   static std::string name() { return "uint32_t"; }
};

template<>
struct TypeName<int64_t>
{
   static std::string name() { return "int64_t"; }
};

template<>
struct TypeName<uint64_t>
{
   static std::string name() { return "uint64_t"; }
};

template<>
struct TypeName<bool>
{
   static std::string name() { return "bool"; }
};

template<>
struct TypeName<std::string>
{
   static std::string name() { return "std::string"; }
};

} // namespace orion

namespace std
{
template<typename T, typename Tag>
struct hash<orion::BaseType<T, Tag>>
{
   using BaseType        = orion::BaseType<T, Tag>;
   using CheckIfHashable = typename std::enable_if<BaseType::is_hashable, void>::type;

   size_t operator()(const orion::BaseType<T, Tag>& x) const 
   { 
      return std::hash<T>()(x.get()); 
   }
};
}

#endif // ORION_TYPES_H
