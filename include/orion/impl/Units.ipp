//
// Units.ipp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITS_IPP
#define ORION_UNITS_IPP

namespace orion
{
   
template<class R>
inline std::ostream& operator<<(std::ostream& os, const Unit<R>& u)
{
   os << "Name:   " << Unit<R>::name() << "\t"
      << "Symbol: " << Unit<R>::symbol() << "\t"
      << "Number: " << Unit<R>::RatioNumber::num << "\n";
   return os;
}

//-------------------------------------------------------------------------------------------------
// SI units

template<>
inline const char* si::Byte::name()
{
   return "Byte";
}
template<>
inline const char* si::Byte::symbol()
{
   return "B";
}
template<>
inline const char* si::Kilobyte::name()
{
   return "Kilobyte";
}
template<>
inline const char* si::Kilobyte::symbol()
{
   return "KB";
}
template<>
inline const char* si::Megabyte::name()
{
   return "Megabyte";
}
template<>
inline const char* si::Megabyte::symbol()
{
   return "MB";
}
template<>
inline const char* si::Gigabyte::name()
{
   return "Gigabyte";
}
template<>
inline const char* si::Gigabyte::symbol()
{
   return "GB";
}
template<>
inline const char* si::Terabyte::name()
{
   return "Terabyte";
}
template<>
inline const char* si::Terabyte::symbol()
{
   return "TB";
}
template<>
inline const char* si::Petabyte::name()
{
   return "Petabyte";
}
template<>
inline const char* si::Petabyte::symbol()
{
   return "PB";
}
template<>
inline const char* si::Exabyte::name()
{
   return "Exabyte";
}
template<>
inline const char* si::Exabyte::symbol()
{
   return "EB";
}
/*
template<> const char* si::Zettabyte::name   = "Zettabyte";
template<> const char* si::Zettabyte::symbol = "ZB";
template<> const char* si::Yottabyte::name   = "Yottabyte";
template<> const char* si::Yottabyte::symbol = "YB";
*/

//-------------------------------------------------------------------------------------------------
// IEC units

template<>
inline const char* iec::Kibibyte::name()
{
   return "Kibibyte";
};
template<>
inline const char* iec::Kibibyte::symbol()
{
   return "KiB";
};
template<>
inline const char* iec::Mebibyte::name()
{
   return "Mebibyte";
};
template<>
inline const char* iec::Mebibyte::symbol()
{
   return "MiB";
};
template<>
inline const char* iec::Gibibyte::name()
{
   return "Gibibyte";
};
template<>
inline const char* iec::Gibibyte::symbol()
{
   return "GiB";
};
template<>
inline const char* iec::Tebibyte::name()
{
   return "Tebibyte";
};
template<>
inline const char* iec::Tebibyte::symbol()
{
   return "TiB";
};
template<>
inline const char* iec::Pebibyte::name()
{
   return "Pebibyte";
};
template<>
inline const char* iec::Pebibyte::symbol()
{
   return "PiB";
};
template<>
inline const char* iec::Exbibyte::name()
{
   return "Exbibyte";
};
template<>
inline const char* iec::Exbibyte::symbol()
{
   return "EiB";
};
/*
template<> const char* iec::Zebibyte::name   = "Zebibyte";
template<> const char* iec::Zebibyte::symbol = "ZiB";
template<> const char* iec::Yobibyte::name   = "Yobibyte";
template<> const char* iec::Yobibyte::symbol = "YiB";
*/

//-------------------------------------------------------------------------------------------------
// Size

template<class U, class T>
inline constexpr Size<U, T>::Size(const Size<U, T>::ValueType& v)
   : _value(v)
{
}

template<class U, class T>
template<class OTHER_TYPE>
inline constexpr Size<U, T>::Size(const Size<Size<U, T>::UnitType, OTHER_TYPE>& other)
   : _value(static_cast<ValueType>(other._value))
{
}

template<class U, class T>
template<class OTHER_UNIT, class OTHER_TYPE>
inline constexpr Size<U, T>::Size(const Size<OTHER_UNIT, OTHER_TYPE>& other)
   : _value(static_cast<Size<U, T>::ValueType>(other._value))
{
   _value *= OTHER_UNIT::RatioNumber::num;
   _value /= UnitType::RatioNumber::num;
}

template<class U, class T>
template<class OTHER_TYPE>
inline constexpr Size<U, T>& Size<U, T>::operator=(const Size<Size<U, T>::UnitType, OTHER_TYPE>& other)
{
   _value = static_cast<ValueType>(other._value);
   return *this;
}

template<class U, class T>
template<class OTHER_UNIT, class OTHER_TYPE>
inline constexpr Size<U, T>& Size<U, T>::operator=(const Size<OTHER_UNIT, OTHER_TYPE>& other)
{
   *this = ThisType(other);
   return *this;
}

//-------------------------------------------------------------------------------------------------
// Relational operators

template<class U, class T>
inline constexpr bool operator==(const Size<U, T>& lhs, const int& rhs)
{
   T Size1 = static_cast<T>(lhs) * U::RatioNumber::num;

   return Size1 == static_cast<T>(rhs);
}

template<class U, class T>
inline constexpr bool operator==(const int& lhs, const Size<U, T>& rhs)
{
   T Size1 = static_cast<T>(rhs) * U::RatioNumber::num;

   return Size1 == static_cast<T>(lhs);
}

template<class U, class T, class OU, class OT>
inline constexpr bool operator==(const Size<U, T>& lhs, const Size<OU, OT>& rhs)
{
   T Size1  = static_cast<T>(lhs) * U::RatioNumber::num;
   OT Size2 = static_cast<OT>(rhs) * OU::RatioNumber::num;

   return Size1 == Size2;
}

template<class U, class T>
inline constexpr bool operator!=(const Size<U, T>& lhs, const int& rhs)
{
   return not(lhs == rhs);
}

template<class U, class T>
inline constexpr bool operator!=(const int& lhs, const Size<U, T>& rhs)
{
   return not(lhs == rhs);
}

template<class U, class T, class OU, class OT>
inline constexpr bool operator!=(const Size<U, T>& lhs, const Size<OU, OT>& rhs)
{
   return not(lhs == rhs);
}

//-------------------------------------------------------------------------------------------------
// literals

inline Size<si::Byte> operator"" _b(unsigned long long s)
{
   return Size<si::Byte>{s};
}
inline Size<si::Kilobyte> operator"" _kb(unsigned long long s)
{
   return Size<si::Kilobyte>{s};
}
inline Size<si::Megabyte> operator"" _mb(unsigned long long s)
{
   return Size<si::Megabyte>{s};
}
inline Size<si::Gigabyte> operator"" _gb(unsigned long long s)
{
   return Size<si::Gigabyte>{s};
}
inline Size<si::Terabyte> operator"" _tb(unsigned long long s)
{
   return Size<si::Terabyte>{s};
}
inline Size<si::Petabyte> operator"" _pb(unsigned long long s)
{
   return Size<si::Petabyte>{s};
}
inline Size<si::Exabyte> operator"" _eb(unsigned long long s)
{
   return Size<si::Exabyte>{s};
}

inline Size<si::Byte, double> operator"" _b(long double s)
{
   return Size<si::Byte, double>{s};
}
inline Size<si::Kilobyte, double> operator"" _kb(long double s)
{
   return Size<si::Kilobyte, double>{s};
}
inline Size<si::Megabyte, double> operator"" _mb(long double s)
{
   return Size<si::Megabyte, double>{s};
}
inline Size<si::Gigabyte, double> operator"" _gb(long double s)
{
   return Size<si::Gigabyte, double>{s};
}
inline Size<si::Terabyte, double> operator"" _tb(long double s)
{
   return Size<si::Terabyte, double>{s};
}
inline Size<si::Petabyte, double> operator"" _pb(long double s)
{
   return Size<si::Petabyte, double>{s};
}
inline Size<si::Exabyte, double> operator"" _eb(long double s)
{
   return Size<si::Exabyte, double>{s};
}

inline Size<iec::Byte> operator"" _bi(unsigned long long s)
{
   return Size<iec::Byte>{s};
}
inline Size<iec::Kibibyte> operator"" _kib(unsigned long long s)
{
   return Size<iec::Kibibyte>{s};
}
inline Size<iec::Mebibyte> operator"" _mib(unsigned long long s)
{
   return Size<iec::Mebibyte>{s};
}
inline Size<iec::Gibibyte> operator"" _gib(unsigned long long s)
{
   return Size<iec::Gibibyte>{s};
}
inline Size<iec::Tebibyte> operator"" _tib(unsigned long long s)
{
   return Size<iec::Tebibyte>{s};
}
inline Size<iec::Pebibyte> operator"" _pib(unsigned long long s)
{
   return Size<iec::Pebibyte>{s};
}
inline Size<iec::Exbibyte> operator"" _eib(unsigned long long s)
{
   return Size<iec::Exbibyte>{s};
}

inline Size<iec::Byte, double> operator"" _bi(long double s)
{
   return Size<iec::Byte, double>{s};
}
inline Size<iec::Kibibyte, double> operator"" _kib(long double s)
{
   return Size<iec::Kibibyte, double>{s};
}
inline Size<iec::Mebibyte, double> operator"" _mib(long double s)
{
   return Size<iec::Mebibyte, double>{s};
}
inline Size<iec::Gibibyte, double> operator"" _gib(long double s)
{
   return Size<iec::Gibibyte, double>{s};
}
inline Size<iec::Tebibyte, double> operator"" _tib(long double s)
{
   return Size<iec::Tebibyte, double>{s};
}
inline Size<iec::Pebibyte, double> operator"" _pib(long double s)
{
   return Size<iec::Pebibyte, double>{s};
}
inline Size<iec::Exbibyte, double> operator"" _eib(long double s)
{
   return Size<iec::Exbibyte, double>{s};
}

} // namespace orion

#endif // ORION_UNITS_IPP
