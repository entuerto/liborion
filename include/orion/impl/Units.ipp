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
      << "Number: " << Unit<R>::ratio_number::num << "\n";
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
// literals

inline Value<si::Byte> operator"" _b(unsigned long long value)
{
   return Value<si::Byte>(value);
}
inline Value<si::Kilobyte> operator"" _kb(unsigned long long value)
{
   return Value<si::Kilobyte>(value);
}
inline Value<si::Megabyte> operator"" _mb(unsigned long long value)
{
   return Value<si::Megabyte>(value);
}
inline Value<si::Gigabyte> operator"" _gb(unsigned long long value)
{
   return Value<si::Gigabyte>(value);
}
inline Value<si::Terabyte> operator"" _tb(unsigned long long value)
{
   return Value<si::Terabyte>(value);
}
inline Value<si::Petabyte> operator"" _pb(unsigned long long value)
{
   return Value<si::Petabyte>(value);
}
inline Value<si::Exabyte> operator"" _eb(unsigned long long value)
{
   return Value<si::Exabyte>(value);
}

inline Value<si::Byte, double> operator"" _b(long double value)
{
   return Value<si::Byte, double>(value);
}
inline Value<si::Kilobyte, double> operator"" _kb(long double value)
{
   return Value<si::Kilobyte, double>(value);
}
inline Value<si::Megabyte, double> operator"" _mb(long double value)
{
   return Value<si::Megabyte, double>(value);
}
inline Value<si::Gigabyte, double> operator"" _gb(long double value)
{
   return Value<si::Gigabyte, double>(value);
}
inline Value<si::Terabyte, double> operator"" _tb(long double value)
{
   return Value<si::Terabyte, double>(value);
}
inline Value<si::Petabyte, double> operator"" _pb(long double value)
{
   return Value<si::Petabyte, double>(value);
}
inline Value<si::Exabyte, double> operator"" _eb(long double value)
{
   return Value<si::Exabyte, double>(value);
}

inline Value<iec::Byte> operator"" _bi(unsigned long long value)
{
   return Value<iec::Byte>(value);
}
inline Value<iec::Kibibyte> operator"" _kib(unsigned long long value)
{
   return Value<iec::Kibibyte>(value);
}
inline Value<iec::Mebibyte> operator"" _mib(unsigned long long value)
{
   return Value<iec::Mebibyte>(value);
}
inline Value<iec::Gibibyte> operator"" _gib(unsigned long long value)
{
   return Value<iec::Gibibyte>(value);
}
inline Value<iec::Tebibyte> operator"" _tib(unsigned long long value)
{
   return Value<iec::Tebibyte>(value);
}
inline Value<iec::Pebibyte> operator"" _pib(unsigned long long value)
{
   return Value<iec::Pebibyte>(value);
}
inline Value<iec::Exbibyte> operator"" _eib(unsigned long long value)
{
   return Value<iec::Exbibyte>(value);
}

inline Value<iec::Byte, double> operator"" _bi(long double value)
{
   return Value<iec::Byte, double>(value);
}
inline Value<iec::Kibibyte, double> operator"" _kib(long double value)
{
   return Value<iec::Kibibyte, double>(value);
}
inline Value<iec::Mebibyte, double> operator"" _mib(long double value)
{
   return Value<iec::Mebibyte, double>(value);
}
inline Value<iec::Gibibyte, double> operator"" _gib(long double value)
{
   return Value<iec::Gibibyte, double>(value);
}
inline Value<iec::Tebibyte, double> operator"" _tib(long double value)
{
   return Value<iec::Tebibyte, double>(value);
}
inline Value<iec::Pebibyte, double> operator"" _pib(long double value)
{
   return Value<iec::Pebibyte, double>(value);
}
inline Value<iec::Exbibyte, double> operator"" _eib(long double value)
{
   return Value<iec::Exbibyte, double>(value);
}

} // namespace orion

#endif // ORION_UNITS_IPP
