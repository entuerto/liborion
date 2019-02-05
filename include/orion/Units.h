//
// Units.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITS_H
#define ORION_UNITS_H

#include <orion/Config.h>

#include <iostream>
#include <limits>
#include <ratio>
#include <type_traits>

namespace orion
{
//-------------------------------------------------------------------------------------------------
// Class Unit

template<class R>
struct Unit
{
   static const char* name();
   static const char* symbol();

   using RatioNumber = R;
};

template<class R>
inline std::ostream& operator<<(std::ostream& os, const Unit<R>& u);

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

//-------------------------------------------------------------------------------------------------
// Class Size

template<class U, class T = uintmax_t>
class Size
{
public:
   static_assert(std::is_arithmetic<T>::value, "The type must be integral or floating-point");

   template<class OTHER_UNIT, class OTHER_TYPE>
   friend class Size;

   using ThisType = Size<U, T>;

   using UnitType  = U;
   using ValueType = T;

   Size() = default;

   constexpr explicit Size(const ValueType& v);

   template<class OTHER_TYPE>
   constexpr explicit Size(const Size<UnitType, OTHER_TYPE>& other);

   template<class OTHER_UNIT, class OTHER_TYPE>
   constexpr explicit Size(const Size<OTHER_UNIT, OTHER_TYPE>& other);

   constexpr explicit operator ValueType() const { return _value; }

   template<class OTHER_TYPE>
   constexpr Size& operator=(const Size<UnitType, OTHER_TYPE>& other);

   template<class OTHER_UNIT, class OTHER_TYPE>
   constexpr Size& operator=(const Size<OTHER_UNIT, OTHER_TYPE>& other);

   static constexpr Size zero() { return ValueType{0}; }
   static constexpr Size min()  { return std::numeric_limits<ValueType>::lowest(); }
   static constexpr Size max()  { return std::numeric_limits<ValueType>::max(); }

   friend std::ostream& operator<<(std::ostream& os, const Size& s)
   {
      os << s._value << " " << UnitType::symbol();
      return os;
   }

private:
   UnitType _unit;
   ValueType _value{0};
};

//-------------------------------------------------------------------------------------------------
// Relational operators

template<class U, class T>
inline constexpr bool operator==(const Size<U, T>& lhs, const int& rhs);

template<class U, class T>
inline constexpr bool operator==(const int& lhs, const Size<U, T>& rhs);

template<class U, class T, class OU, class OT>
inline constexpr bool operator==(const Size<U, T>& lhs, const Size<OU, OT>& rhs);

template<class U, class T>
inline constexpr bool operator!=(const Size<U, T>& lhs, const int& rhs);

template<class U, class T>
inline constexpr bool operator!=(const int& lhs, const Size<U, T>& rhs);

template<class U, class T, class OU, class OT>
inline constexpr bool operator!=(const Size<U, T>& lhs, const Size<OU, OT>& rhs);

//-------------------------------------------------------------------------------------------------
// Convert functions
template <class T>
struct IsSize : std::false_type {};

template <class U, class T>
struct IsSize<Size<U, T>> : std::true_type  {};

template <class U, class T>
struct IsSize<const Size<U, T>> : std::true_type {};

template<class ToSize, class U, class T>
inline ToSize size_cast(const Size<U, T>& s)
{
   static_assert(IsSize<ToSize>::value, "Must be a Size class");

   return ToSize{s};
}

//-------------------------------------------------------------------------------------------------
// literals

inline Size<si::Byte>     operator"" _b(unsigned long long s);
inline Size<si::Kilobyte> operator"" _kb(unsigned long long s);
inline Size<si::Megabyte> operator"" _mb(unsigned long long s);
inline Size<si::Gigabyte> operator"" _gb(unsigned long long s);
inline Size<si::Terabyte> operator"" _tb(unsigned long long s);
inline Size<si::Petabyte> operator"" _pb(unsigned long long s);
inline Size<si::Exabyte>  operator"" _eb(unsigned long long s);

inline Size<si::Byte,     double> operator"" _b(long double s);
inline Size<si::Kilobyte, double> operator"" _kb(long double s);
inline Size<si::Megabyte, double> operator"" _mb(long double s);
inline Size<si::Gigabyte, double> operator"" _gb(long double s);
inline Size<si::Terabyte, double> operator"" _tb(long double s);
inline Size<si::Petabyte, double> operator"" _pb(long double s);
inline Size<si::Exabyte,  double> operator"" _eb(long double s);

inline Size<iec::Byte>     operator"" _bi(unsigned long long s);
inline Size<iec::Kibibyte> operator"" _kib(unsigned long long s);
inline Size<iec::Mebibyte> operator"" _mib(unsigned long long s);
inline Size<iec::Gibibyte> operator"" _gib(unsigned long long s);
inline Size<iec::Tebibyte> operator"" _tib(unsigned long long s);
inline Size<iec::Pebibyte> operator"" _pib(unsigned long long s);
inline Size<iec::Exbibyte> operator"" _eib(unsigned long long s);

inline Size<iec::Byte,     double> operator"" _bi(long double s);
inline Size<iec::Kibibyte, double> operator"" _kib(long double s);
inline Size<iec::Mebibyte, double> operator"" _mib(long double s);
inline Size<iec::Gibibyte, double> operator"" _gib(long double s);
inline Size<iec::Tebibyte, double> operator"" _tib(long double s);
inline Size<iec::Pebibyte, double> operator"" _pib(long double s);
inline Size<iec::Exbibyte, double> operator"" _eib(long double s);

} // namespace orion

#include <orion/impl/Units.ipp>

#endif // ORION_UNITS_H
