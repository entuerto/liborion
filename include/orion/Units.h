//
// Units.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITS_H
#define ORION_UNITS_H

#include <orion/Orion-Stddefs.h>
#include <iostream>
#include <limits>
#include <ratio>
#include <type_traits>

namespace orion
{
template<class R>
struct Unit
{
   static const char* name;
   static const char* symbol;

   using ratio_number = R;
};

template<class R>
std::ostream& operator<<(std::ostream& os, const Unit<R>& u)
{
   os << "Name:   " << u.name << "\t"
      << "Symbol: " << u.symbol << "\t"
      << "Number: " << Unit<R>::ratio_number::num << "\n";
   return os;
}

namespace si
{
using Byte     = Unit<std::ratio<1>>;
using Kilobyte = Unit<std::kilo>;
using Megabyte = Unit<std::mega>;
using Gigabyte = Unit<std::giga>;
using Terabyte = Unit<std::tera>;
using Petabyte = Unit<std::peta>;
using Exabyte  = Unit<std::exa>;
// using Zettabyte = Unit<std::zetta>;
// using Yottabyte = Unit<std::yotta>;
}

template<> const char* si::Byte::name       = "Byte";
template<> const char* si::Byte::symbol     = "B";
template<> const char* si::Kilobyte::name   = "Kilobyte";
template<> const char* si::Kilobyte::symbol = "KB";
template<> const char* si::Megabyte::name   = "Megabyte";
template<> const char* si::Megabyte::symbol = "MB";
template<> const char* si::Gigabyte::name   = "Gigabyte";
template<> const char* si::Gigabyte::symbol = "GB";
template<> const char* si::Terabyte::name   = "Terabyte";
template<> const char* si::Terabyte::symbol = "TB";
template<> const char* si::Petabyte::name   = "Petabyte";
template<> const char* si::Petabyte::symbol = "PB";
template<> const char* si::Exabyte::name    = "Exabyte";
template<> const char* si::Exabyte::symbol  = "EB";
/*
template<> const char* si::Zettabyte::name   = "Zettabyte";
template<> const char* si::Zettabyte::symbol = "ZB";
template<> const char* si::Yottabyte::name   = "Yottabyte";
template<> const char* si::Yottabyte::symbol = "YB";
*/

namespace iec
{
using Byte     = Unit<std::ratio<1LL>>;
using Kibibyte = Unit<std::ratio<1024LL>>;
using Mebibyte = Unit<std::ratio<1048576LL>>;
using Gibibyte = Unit<std::ratio<1073741824LL>>;
using Tebibyte = Unit<std::ratio<1099511627776LL>>;
using Pebibyte = Unit<std::ratio<1125899906842624LL>>;
using Exbibyte = Unit<std::ratio<1152921504606846976LL>>;
// using Zebibyte = Unit<std::ratio<   1180591620717411303424LL>>;
// using Yobibyte = Unit<std::ratio<1208925819614629174706176LL>>;
}

template<> const char* iec::Kibibyte::name   = "Kibibyte";
template<> const char* iec::Kibibyte::symbol = "KiB";
template<> const char* iec::Mebibyte::name   = "Mebibyte";
template<> const char* iec::Mebibyte::symbol = "MiB";
template<> const char* iec::Gibibyte::name   = "Gibibyte";
template<> const char* iec::Gibibyte::symbol = "GiB";
template<> const char* iec::Tebibyte::name   = "Tebibyte";
template<> const char* iec::Tebibyte::symbol = "TiB";
template<> const char* iec::Pebibyte::name   = "Pebibyte";
template<> const char* iec::Pebibyte::symbol = "PiB";
template<> const char* iec::Exbibyte::name   = "Exbibyte";
template<> const char* iec::Exbibyte::symbol = "EiB";
/*
template<> const char* iec::Zebibyte::name   = "Zebibyte";
template<> const char* iec::Zebibyte::symbol = "ZiB";
template<> const char* iec::Yobibyte::name   = "Yobibyte";
template<> const char* iec::Yobibyte::symbol = "YiB";
*/

template<class U, class T = uintmax_t>
class Value
{
public:
   static_assert(std::is_arithmetic<T>::value, "The type must be integral or floating-point");

   template<class OTHER_UNIT, class OTHER_TYPE>
   friend class Value;

   using this_type = Value<U, T>;

   using unit_type  = U;
   using value_type = T;

   Value()
      : _value(T(0))
   {
   }

   explicit Value(const value_type& v)
      : _value(v)
   {
   }

   template<class OTHER_TYPE>
   Value(const Value<unit_type, OTHER_TYPE>& other)
      : _value(static_cast<value_type>(other._value))
   {
   }

   template<class OTHER_UNIT, class OTHER_TYPE>
   Value(const Value<OTHER_UNIT, OTHER_TYPE>& other)
      : _value(static_cast<value_type>(other._value))
   {
      _value *= OTHER_UNIT::ratio_number::num;
      _value /= unit_type::ratio_number::num;
   }

   operator value_type() const { return _value; }

   explicit operator value_type&() const { return _value; }

   template<class OTHER_TYPE>
   this_type operator=(const Value<unit_type, OTHER_TYPE>& other)
   {
      _value = static_cast<value_type>(other._value);
   }

   template<class OTHER_UNIT, class OTHER_TYPE>
   this_type operator=(const Value<OTHER_UNIT, OTHER_TYPE>& other)
   {
      *this = this_type(other);
      return *this;
   }

   static constexpr Value zero() { return value_type(0); }
   static constexpr Value min()  { return std::numeric_limits<value_type>::lowest(); }
   static constexpr Value max()  { return std::numeric_limits<value_type>::max(); }

   friend std::ostream& operator<<(std::ostream& os, const Value& value)
   {
      os << value._value << " " << value._unit.symbol;
      return os;
   }

private:
   unit_type _unit;
   value_type _value;
};

//-------------------------------------------------------------------------------------------------
// Relational operators

template<class U, class T>
inline bool operator==(const Value<U, T>& lhs, const int& rhs)
{
   T value1 = lhs * U::ratio_number::num;

   return value1 == static_cast<T>(rhs);
}

template<class U, class T>
inline bool operator!=(const Value<U, T>& lhs, const int& rhs)
{
   return not(lhs == rhs);
}

template<class U, class T, class OU, class OT>
inline bool operator==(const Value<U, T>& lhs, const Value<OU, OT>& rhs)
{
   T value1  = lhs * U::ratio_number::num;
   OT value2 = rhs * OU::ratio_number::num;

   return value1 == value2;
}

template<class U, class T, class OU, class OT>
inline bool operator!=(const Value<U, T>& lhs, const Value<OU, OT>& rhs)
{
   return not(lhs == rhs);
}

//-------------------------------------------------------------------------------------------------
// Convert functions

template<class N, class U, class T>
Value<N, T> convert(const Value<U, T>& value)
{
   return (value * U::ratio_number::num) / N::ratio_number::num;
}

template<class N, class U>
Value<N, double> convert(const Value<U, uintmax_t>& value)
{
   return double(value * U::ratio_number::num) / N::ratio_number::num;
}

//-------------------------------------------------------------------------------------------------
// literals

Value<si::Byte> operator"" _b(unsigned long long value)
{
   return Value<si::Byte>(value);
}
Value<si::Kilobyte> operator"" _kb(unsigned long long value)
{
   return Value<si::Kilobyte>(value);
}
Value<si::Megabyte> operator"" _mb(unsigned long long value)
{
   return Value<si::Megabyte>(value);
}
Value<si::Gigabyte> operator"" _gb(unsigned long long value)
{
   return Value<si::Gigabyte>(value);
}
Value<si::Terabyte> operator"" _tb(unsigned long long value)
{
   return Value<si::Terabyte>(value);
}
Value<si::Petabyte> operator"" _pb(unsigned long long value)
{
   return Value<si::Petabyte>(value);
}
Value<si::Exabyte> operator"" _eb(unsigned long long value)
{
   return Value<si::Exabyte>(value);
}

Value<si::Byte, double> operator"" _b(long double value)
{
   return Value<si::Byte, double>(value);
}
Value<si::Kilobyte, double> operator"" _kb(long double value)
{
   return Value<si::Kilobyte, double>(value);
}
Value<si::Megabyte, double> operator"" _mb(long double value)
{
   return Value<si::Megabyte, double>(value);
}
Value<si::Gigabyte, double> operator"" _gb(long double value)
{
   return Value<si::Gigabyte, double>(value);
}
Value<si::Terabyte, double> operator"" _tb(long double value)
{
   return Value<si::Terabyte, double>(value);
}
Value<si::Petabyte, double> operator"" _pb(long double value)
{
   return Value<si::Petabyte, double>(value);
}
Value<si::Exabyte, double> operator"" _eb(long double value)
{
   return Value<si::Exabyte, double>(value);
}

Value<iec::Byte> operator"" _bi(unsigned long long value)
{
   return Value<iec::Byte>(value);
}
Value<iec::Kibibyte> operator"" _kib(unsigned long long value)
{
   return Value<iec::Kibibyte>(value);
}
Value<iec::Mebibyte> operator"" _mib(unsigned long long value)
{
   return Value<iec::Mebibyte>(value);
}
Value<iec::Gibibyte> operator"" _gib(unsigned long long value)
{
   return Value<iec::Gibibyte>(value);
}
Value<iec::Tebibyte> operator"" _tib(unsigned long long value)
{
   return Value<iec::Tebibyte>(value);
}
Value<iec::Pebibyte> operator"" _pib(unsigned long long value)
{
   return Value<iec::Pebibyte>(value);
}
Value<iec::Exbibyte> operator"" _eib(unsigned long long value)
{
   return Value<iec::Exbibyte>(value);
}

Value<iec::Byte, double> operator"" _bi(long double value)
{
   return Value<iec::Byte, double>(value);
}
Value<iec::Kibibyte, double> operator"" _kib(long double value)
{
   return Value<iec::Kibibyte, double>(value);
}
Value<iec::Mebibyte, double> operator"" _mib(long double value)
{
   return Value<iec::Mebibyte, double>(value);
}
Value<iec::Gibibyte, double> operator"" _gib(long double value)
{
   return Value<iec::Gibibyte, double>(value);
}
Value<iec::Tebibyte, double> operator"" _tib(long double value)
{
   return Value<iec::Tebibyte, double>(value);
}
Value<iec::Pebibyte, double> operator"" _pib(long double value)
{
   return Value<iec::Pebibyte, double>(value);
}
Value<iec::Exbibyte, double> operator"" _eib(long double value)
{
   return Value<iec::Exbibyte, double>(value);
}

} // namespace orion

#endif // ORION_UNITS_H
