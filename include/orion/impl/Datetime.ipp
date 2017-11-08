//
// Datetime.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_DATETIME_IPP
#define ORION_DATETIME_IPP

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <string>

namespace orion
{
//---------------------------------------------------------------------------------------
// Implementation
//---------------------------------------------------------------------------------------

// Year
//---------------------------------------------------------------------------------------

constexpr inline Year::Year(int y) noexcept
   : _value(static_cast<decltype(_value)>(y))
{
}

constexpr inline Year& Year::operator++() noexcept
{
   ++_value;
   return *this;
}

constexpr inline Year Year::operator++(int)noexcept
{
   auto tmp(*this);
   ++(*this);
   return tmp;
}

constexpr inline Year& Year::operator--() noexcept
{
   --_value;
   return *this;
}

constexpr inline Year Year::operator--(int)noexcept
{
   auto tmp(*this);
   --(*this);
   return tmp;
}

constexpr inline Year& Year::operator+=(const years& y) noexcept
{
   *this = *this + y;
   return *this;
}

constexpr inline Year& Year::operator-=(const years& y) noexcept
{
   *this = *this - y;
   return *this;
}

constexpr inline Year Year::operator-() const noexcept
{
   return Year{-_value};
}

constexpr inline Year Year::operator+() const noexcept
{
   return *this;
}

constexpr inline bool Year::is_leap() const noexcept
{
   return _value % 4 == 0 and (_value % 100 != 0 or _value % 400 == 0);
}

constexpr inline Year::operator Year::value_type() const noexcept
{
   return _value;
}

constexpr inline bool Year::ok() const noexcept
{
   return true;
}

constexpr inline Year Year::min() noexcept
{
   return Year{std::numeric_limits<Year::value_type>::min()};
}

constexpr inline Year Year::max() noexcept
{
   return Year{std::numeric_limits<Year::value_type>::max()};
}

constexpr inline bool operator==(const Year& x, const Year& y) noexcept
{
   return static_cast<Year::value_type>(x) == static_cast<Year::value_type>(y);
}

constexpr inline bool operator!=(const Year& x, const Year& y) noexcept
{
   return not(x == y);
}

constexpr inline bool operator<(const Year& x, const Year& y) noexcept
{
   return static_cast<Year::value_type>(x) < static_cast<Year::value_type>(y);
}

constexpr inline bool operator>(const Year& x, const Year& y) noexcept
{
   return y < x;
}

constexpr inline bool operator<=(const Year& x, const Year& y) noexcept
{
   return not(y < x);
}

constexpr inline bool operator>=(const Year& x, const Year& y) noexcept
{
   return not(x < y);
}

constexpr inline Year operator+(const Year& x, const years& y) noexcept
{
   return Year{static_cast<Year::value_type>(x) + y.count()};
}

constexpr inline Year operator+(const years& x, const Year& y) noexcept
{
   return y + x;
}

constexpr inline Year operator-(const Year& x, const years& y) noexcept
{
   return Year{static_cast<Year::value_type>(x) - y.count()};
}

constexpr inline years operator-(const Year& x, const Year& y) noexcept
{
   return years{static_cast<Year::value_type>(x) - static_cast<Year::value_type>(y)};
}

// Month
//---------------------------------------------------------------------------------------

constexpr inline Month::Month(Month::value_type m) noexcept
   : _value(m)
{
}

constexpr inline Month& Month::operator++() noexcept
{
   if (++_value == 13)
      _value = 1;
   return *this;
}

constexpr inline Month Month::operator++(int)noexcept
{
   auto tmp(*this);
   ++(*this);
   return tmp;
}

constexpr inline Month& Month::operator--() noexcept
{
   if (--_value == 0)
      _value = 12;
   return *this;
}

constexpr inline Month Month::operator--(int)noexcept
{
   auto tmp(*this);
   --(*this);
   return tmp;
}

constexpr inline Month& Month::operator+=(const months& m) noexcept
{
   *this = *this + m;
   return *this;
}

constexpr inline Month& Month::operator-=(const months& m) noexcept
{
   *this = *this - m;
   return *this;
}

constexpr inline Month::operator Month::value_type() const noexcept
{
   return _value;
}

constexpr inline bool Month::ok() const noexcept
{
   return 1 <= _value and _value <= 12;
}

constexpr inline bool operator==(const Month& lhs, const Month& rhs) noexcept
{
   return static_cast<Month::value_type>(lhs) == static_cast<Month::value_type>(rhs);
}

constexpr inline bool operator!=(const Month& lhs, const Month& rhs) noexcept
{
   return not(lhs == rhs);
}

constexpr inline bool operator<(const Month& lhs, const Month& rhs) noexcept
{
   return static_cast<Month::value_type>(lhs) < static_cast<Month::value_type>(rhs);
}

constexpr inline bool operator>(const Month& lhs, const Month& rhs) noexcept
{
   return rhs < lhs;
}

constexpr inline bool operator<=(const Month& lhs, const Month& rhs) noexcept
{
   return not(rhs < lhs);
}

constexpr inline bool operator>=(const Month& lhs, const Month& rhs) noexcept
{
   return not(lhs < rhs);
}

constexpr Month inline operator+(const Month& lhs, const months& rhs) noexcept
{
   const auto mu = static_cast<int32_t>(static_cast<Month::value_type>(lhs)) - 1 + rhs.count();
   const auto yr = (mu >= 0 ? mu : mu - 11) / 12;

   return Month{static_cast<Month::value_type>(mu - yr * 12 + 1)};
}

constexpr Month inline operator+(const months& lhs, const Month& rhs) noexcept
{
   return rhs + lhs;
}

constexpr Month inline operator-(const Month& lhs, const months& rhs) noexcept
{
   return lhs + -rhs;
}

constexpr months inline operator-(const Month& lhs, const Month& rhs) noexcept
{
   const auto d = static_cast<Month::value_type>(lhs) - static_cast<Month::value_type>(rhs);

   return months{d <= 11 ? d : d + 12};
}

// Day
//---------------------------------------------------------------------------------------

constexpr inline Day::Day(uint32_t d) noexcept
   : _value(static_cast<Day::value_type>(d))
{
}

constexpr inline Day& Day::operator++() noexcept
{
   ++_value;
   return *this;
}

constexpr inline Day Day::operator++(int)noexcept
{
   auto tmp(*this);
   ++(*this);
   return tmp;
}

constexpr inline Day& Day::operator--() noexcept
{
   --_value;
   return *this;
}

constexpr inline Day Day::operator--(int)noexcept
{
   auto tmp(*this);
   --(*this);
   return tmp;
}

constexpr inline Day& Day::operator+=(const days& d) noexcept
{
   *this = *this + d;
   return *this;
}

constexpr inline Day& Day::operator-=(const days& d) noexcept
{
   *this = *this - d;
   return *this;
}

constexpr inline Day::operator Day::value_type() const noexcept
{
   return _value;
}

constexpr inline Day::operator uint32_t() const noexcept
{
   return _value;
}

constexpr inline bool Day::ok() const noexcept
{
   return 1 <= _value and _value <= 31;
}

constexpr inline bool operator==(const Day& lhs, const Day& rhs) noexcept
{
   return static_cast<Day::value_type>(lhs) == static_cast<Day::value_type>(rhs);
}

constexpr inline bool operator!=(const Day& lhs, const Day& rhs) noexcept
{
   return not(lhs == rhs);
}

constexpr inline bool operator<(const Day& lhs, const Day& rhs) noexcept
{
   return static_cast<Day::value_type>(lhs) < static_cast<Day::value_type>(rhs);
}

constexpr inline bool operator>(const Day& lhs, const Day& rhs) noexcept
{
   return rhs < lhs;
}

constexpr inline bool operator<=(const Day& lhs, const Day& rhs) noexcept
{
   return not(rhs < lhs);
}

constexpr inline bool operator>=(const Day& lhs, const Day& rhs) noexcept
{
   return not(lhs < rhs);
}

constexpr inline Day operator+(const Day& lhs, const days& rhs) noexcept
{
   return Day{static_cast<uint32_t>(lhs) + static_cast<uint32_t>(rhs.count())};
}

constexpr inline Day operator+(const days& lhs, const Day& rhs) noexcept
{
   return rhs + lhs;
}

constexpr inline Day operator-(const Day& lhs, const days& rhs) noexcept
{
   return lhs + -rhs;
}

constexpr inline days operator-(const Day& lhs, const Day& rhs) noexcept
{
   return days{static_cast<days::rep>(static_cast<Day::value_type>(lhs) -
                                      static_cast<Day::value_type>(rhs))};
}

//---------------------------------------------------------------------------------------

// chrono-Compatible Low-Level Date Algorithms
// 2016 Howard Hinnant
// http://howardhinnant.github.io/date_algorithms.html

// Returns number of days since civil 1970-01-01.  Negative values indicate
// days prior to 1970-01-01.
//
// Preconditions:  y-m-d represents a date in the civil (Gregorian) calendar
//                 m is in [1, 12]
//                 d is in [1, last_day_of_month(y, m)]
//                 y is "approximately" in
//                   [numeric_limits<IntT>::min()/366, numeric_limits<IntT>::max()/366]
//                 Exact range of validity is:
//                 [civil_from_days(numeric_limits<IntT>::min()),
//                  civil_from_days(numeric_limits<IntT>::max()-719468)]
template<class IntT>
constexpr IntT from_days(IntT y, unsigned m, unsigned d) noexcept
{
   static_assert(std::numeric_limits<unsigned>::digits >= 18,
                 "This algorithm has not been ported to a 16 bit unsigned Integer");
   static_assert(std::numeric_limits<IntT>::digits >= 20,
                 "This algorithm has not been ported to a 16 bit signed Integer");

   y -= m <= 2;

   const IntT era = (y >= 0 ? y : y - 399) / 400;

   const unsigned year_of_era = static_cast<unsigned>(y - era * 400); // [0, 399]

   const unsigned day_of_year = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]

   const unsigned day_of_era =
      year_of_era * 365 + year_of_era / 4 - year_of_era / 100 + day_of_year; // [0, 146096]

   return era * 146097 + static_cast<IntT>(day_of_era) - unix_epoch;
}

//
// Returns year/month/day triple in civil calendar
// Preconditions:  z is number of days since 1970-01-01 and is in the range:
//                   [numeric_limits<IntT>::min(), numeric_limits<IntT>::max()-719468].
template<class IntT>
constexpr std::tuple<IntT, unsigned, unsigned> to_days(IntT z) noexcept
{
   static_assert(std::numeric_limits<unsigned>::digits >= 18,
                 "This algorithm has not been ported to a 16 bit unsigned Integer");

   static_assert(std::numeric_limits<IntT>::digits >= 20,
                 "This algorithm has not been ported to a 16 bit signed Integer");

   z += unix_epoch;

   const IntT era = (z >= 0 ? z : z - 146096) / 146097;

   const unsigned day_of_era = static_cast<unsigned>(z - era * 146097); // [0, 146096]

   const unsigned year_of_era =
      (day_of_era - day_of_era / 1460 + day_of_era / 36524 - day_of_era / 146096) / 365; // [0, 399]

   const IntT y = static_cast<IntT>(year_of_era) + era * 400;

   const unsigned day_of_year =
      day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100); // [0, 365]

   const unsigned mp = (5 * day_of_year + 2) / 153; // [0, 11]

   const unsigned d = day_of_year - (153 * mp + 2) / 5 + 1; // [1, 31]

   const unsigned m = mp + (mp < 10 ? 3 : -9); // [1, 12]

   return std::tuple<IntT, unsigned, unsigned>(y + (m <= 2), m, d);
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of common year
// The result is always in the range [28, 31].
constexpr uint32_t last_day_of_month_common_year(uint32_t m) noexcept
{
   constexpr uint8_t a[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
   return a[m - 1];
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of year y
// The result is always in the range [28, 31].
template<class IntT>
constexpr uint32_t last_day_of_month(IntT y, uint32_t m) noexcept
{
   return (m != 2 or not is_leap(y)) ? last_day_of_month_common_year(m) : 29u;
}

// Returns day of week in civil calendar [0, 6] -> [Sun, Sat]
// Preconditions:  z is number of days since 1970-01-01 and is in the range:
//                   [numeric_limits<Int>::min(), numeric_limits<Int>::max()-4].
template<class IntT>
constexpr uint32_t weekday_from_days(IntT z) noexcept
{
   return static_cast<uint32_t>(z >= -4 ? (z + 4) % 7 : (z + 5) % 7 + 6);
}

// Preconditions: x <= 6 && y <= 6
// Returns: The number of days from the weekday y to the weekday x.
// The result is always in the range [0, 6].
constexpr uint32_t weekday_difference(uint32_t x, uint32_t y) noexcept
{
   x -= y;
   return (x <= 6) ? x : x + 7;
}

// Preconditions: wd <= 6
// Returns: The weekday following wd
// The result is always in the range [0, 6].
constexpr uint32_t next_weekday(uint32_t wd) noexcept
{
   return (wd < 6) ? wd + 1 : 0;
}

// Preconditions: wd <= 6
// Returns: The weekday prior to wd
// The result is always in the range [0, 6].
constexpr uint32_t prev_weekday(uint32_t wd) noexcept
{
   return (wd > 0) ? wd - 1 : 6;
}

//---------------------------------------------------------------------------------------

constexpr std::pair<Year, Month> operator/(const Year& y, const Month& m) noexcept
{
   return {y, m};
}

constexpr std::pair<Month, Day> operator/(const Month& m, const Day& d) noexcept
{
   return {m, d};
}

constexpr std::pair<Day, Month> operator/(const Day& d, const Month& m) noexcept
{
   return {d, m};
}

constexpr days operator/(const std::pair<Year, Month>& ym, const Day& d) noexcept
{
   return days(from_days(static_cast<Year::value_type>(ym.first),
                         static_cast<Month::value_type>(ym.second),
                         static_cast<Day::value_type>(d)));
}

constexpr days operator/(const std::pair<Month, Day>& md, const Year& y) noexcept
{
   return days(from_days(static_cast<Year::value_type>(y),
                         static_cast<Month::value_type>(md.first),
                         static_cast<Day::value_type>(md.second)));
}

constexpr days operator/(const std::pair<Day, Month>& dm, const Year& y) noexcept
{
   return days(from_days(static_cast<Year::value_type>(y),
                         static_cast<Month::value_type>(dm.second),
                         static_cast<Day::value_type>(dm.first)));
}

//---------------------------------------------------------------------------------------
//
template<class CharT, class Traits, class Rep, class Period>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                              const std::chrono::duration<Rep, Period>& d)
{
   using namespace std::chrono;
   return os << d.count() << unit_names[std::type_index(typeid(typename Period::type))];
}

} // namespace orion

#endif // ORION_DATETIME_IPP
