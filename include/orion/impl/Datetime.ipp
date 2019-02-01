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

#include <orion/Utils.h>

#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

namespace orion
{
//-------------------------------------------------------------------------------------------------

// chrono-Compatible Low-Level Date Algorithms
// 2016 Howard Hinnant
// http://howardhinnant.github.io/date_algorithms.html

// Used to shift the serial date from 0 to 1970-01-01 instead of 0000-03-01.
constexpr int32_t unix_epoch = 719468;

// Returns number of Days since civil 1970-01-01.  Negative values indicate
// Days prior to 1970-01-01.
//
// Preconditions:  y-m-d represents a date in the civil (Gregorian) calendar
//                 m is in [1, 12]
//                 d is in [1, last_day_of_month(y, m)]
//                 y is "approximately" in
//                   [numeric_limits<IntT>::min()/366, numeric_limits<IntT>::max()/366]
//                 Exact range of validity is:
//                 [civil_from_Days(numeric_limits<IntT>::min()),
//                  civil_from_Days(numeric_limits<IntT>::max()-719468)]
inline constexpr int to_days(int y, uint32_t m, uint32_t d) noexcept
{
   static_assert(std::numeric_limits<uint32_t>::digits >= 18,
                 "This algorithm has not been ported to a 16 bit uint32_t Integer");
   static_assert(std::numeric_limits<int>::digits >= 20,
                 "This algorithm has not been ported to a 16 bit signed Integer");

   y -= m <= 2;

   const int era = (y >= 0 ? y : y - 399) / 400;

   const auto year_of_era = static_cast<uint32_t>(y - era * 400); // [0, 399]

   const uint32_t day_of_year = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]

   const uint32_t day_of_era =
      year_of_era * 365 + year_of_era / 4 - year_of_era / 100 + day_of_year; // [0, 146096]

   return era * 146097 + static_cast<int>(day_of_era) - unix_epoch;
}

//
// Returns year/month/day triple in civil calendar
// Preconditions:  z is number of Days since 1970-01-01 and is in the range:
//                   [numeric_limits<IntT>::min(), numeric_limits<IntT>::max()-719468].
inline constexpr std::tuple<int, uint32_t, uint32_t> from_days(int z) noexcept
{
   static_assert(std::numeric_limits<uint32_t>::digits >= 18,
                 "This algorithm has not been ported to a 16 bit uint32_t Integer");

   static_assert(std::numeric_limits<int>::digits >= 20,
                 "This algorithm has not been ported to a 16 bit signed Integer");

   z += unix_epoch;

   const int era = (z >= 0 ? z : z - 146096) / 146097;

   const auto day_of_era = static_cast<uint32_t>(z - era * 146097); // [0, 146096]

   const uint32_t year_of_era =
      (day_of_era - day_of_era / 1460 + day_of_era / 36524 - day_of_era / 146096) / 365; // [0, 399]

   const auto y = static_cast<int>(year_of_era) + era * 400;

   const uint32_t day_of_year =
      day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100); // [0, 365]

   const uint32_t mp = (5 * day_of_year + 2) / 153; // [0, 11]

   const uint32_t d = day_of_year - (153 * mp + 2) / 5 + 1; // [1, 31]

   const uint32_t m = mp + (mp < 10 ? 3 : -9); // [1, 12]

   return std::tuple<int, uint32_t, uint32_t>(y + (m <= 2), m, d);
}

template<class IntT>
inline constexpr bool is_leap(IntT y) noexcept 
{ 
   return y % 4 == 0 and (y % 100 != 0 or y % 400 == 0); 
}

// Preconditions: m is in [1, 12]
// Returns: The number of Days in the month m of common year
// The result is always in the range [28, 31].
inline constexpr uint32_t last_day_of_month_common_year(uint32_t m) noexcept
{
   constexpr uint8_t a[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
   return at(a, m - 1);
}

// Preconditions: m is in [1, 12]
// Returns: The number of Days in the month m of year y
// The result is always in the range [28, 31].
template<class IntT>
inline constexpr uint32_t last_day_of_month(IntT y, uint32_t m) noexcept
{
   return (m != 2 or not is_leap(y)) ? last_day_of_month_common_year(m) : 29u;
}

// Returns day of week in civil calendar [0, 6] -> [Sun, Sat]
// Preconditions:  z is number of Days since 1970-01-01 and is in the range:
//                   [numeric_limits<Int>::min(), numeric_limits<Int>::max()-4].
template<class IntT>
inline constexpr uint32_t weekday_from_days(IntT z) noexcept
{
   return static_cast<uint32_t>(z >= -4 ? (z + 4) % 7 : (z + 5) % 7 + 6);
}

// Preconditions: x <= 6 && y <= 6
// Returns: The number of Days from the weekday y to the weekday x.
// The result is always in the range [0, 6].
inline constexpr uint32_t weekday_difference(uint32_t x, uint32_t y) noexcept
{
   x -= y;
   return (x <= 6) ? x : x + 7;
}

// Preconditions: wd <= 6
// Returns: The weekday following wd
// The result is always in the range [0, 6].
inline constexpr uint32_t next_weekday(uint32_t wd) noexcept
{
   return (wd < 6) ? wd + 1 : 0;
}

// Preconditions: wd <= 6
// Returns: The weekday prior to wd
// The result is always in the range [0, 6].
inline constexpr uint32_t prev_weekday(uint32_t wd) noexcept
{
   return (wd > 0) ? wd - 1 : 6;
}

//-------------------------------------------------------------------------------------------------
// Implementation
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Year
//-------------------------------------------------------------------------------------------------

inline constexpr Year::Year(int y) noexcept
   : _value(static_cast<decltype(_value)>(y))
{
}

inline constexpr Year& Year::operator++() noexcept
{
   ++_value;
   return *this;
}

inline constexpr Year Year::operator++(int)noexcept
{
   auto tmp(*this);
   ++(*this);
   return tmp;
}

inline constexpr Year& Year::operator--() noexcept
{
   --_value;
   return *this;
}

inline constexpr Year Year::operator--(int)noexcept
{
   auto tmp(*this);
   --(*this);
   return tmp;
}

inline constexpr Year& Year::operator+=(const Years& y) noexcept
{
   *this = *this + y;
   return *this;
}

inline constexpr Year& Year::operator-=(const Years& y) noexcept
{
   *this = *this - y;
   return *this;
}

inline constexpr Year Year::operator-() const noexcept
{
   return Year{-_value};
}

inline constexpr Year Year::operator+() const noexcept
{
   return *this;
}

inline constexpr bool Year::is_leap() const noexcept
{
   return _value % 4 == 0 and (_value % 100 != 0 or _value % 400 == 0);
}

inline constexpr Year::operator int() const noexcept
{
   return _value;
}

inline constexpr bool Year::ok() const noexcept
{
   return true;
}

inline constexpr Year Year::min() noexcept
{
   return Year{std::numeric_limits<Year::value_type>::min()};
}

inline constexpr Year Year::max() noexcept
{
   return Year{std::numeric_limits<Year::value_type>::max()};
}

inline constexpr bool operator==(const Year& x, const Year& y) noexcept
{
   return static_cast<int>(x) == static_cast<int>(y);
}

inline constexpr bool operator!=(const Year& x, const Year& y) noexcept
{
   return not(x == y);
}

inline constexpr bool operator<(const Year& x, const Year& y) noexcept
{
   return static_cast<int>(x) < static_cast<int>(y);
}

inline constexpr bool operator>(const Year& x, const Year& y) noexcept
{
   return y < x;
}

inline constexpr bool operator<=(const Year& x, const Year& y) noexcept
{
   return not(y < x);
}

inline constexpr bool operator>=(const Year& x, const Year& y) noexcept
{
   return not(x < y);
}

inline constexpr Year operator+(const Year& x, const Years& y) noexcept
{
   return Year{static_cast<int>(x) + y.count()};
}

inline constexpr Year operator+(const Years& x, const Year& y) noexcept
{
   return y + x;
}

inline constexpr Year operator-(const Year& x, const Years& y) noexcept
{
   return Year{static_cast<int>(x) - y.count()};
}

inline constexpr Years operator-(const Year& x, const Year& y) noexcept
{
   return Years{static_cast<int>(x) - static_cast<int>(y)};
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const Year& y)
{
   //detail::save_ostream<CharT, Traits> _(os);
   os.fill('0');
   os.flags(std::ios::dec | std::ios::internal);
   os.width(4 + (y < Year{0}));
   os << static_cast<int>(y);
   if (not y.ok())
      os << " is not a valid year";
   return os;
}

//-------------------------------------------------------------------------------------------------
// Month
//-------------------------------------------------------------------------------------------------

inline constexpr Month::Month(unsigned m) noexcept
   : _value(m)
{
}

inline constexpr Month& Month::operator++() noexcept
{
   if (++_value == 13)
      _value = 1;
   return *this;
}

inline constexpr Month Month::operator++(int)noexcept
{
   auto tmp(*this);
   ++(*this);
   return tmp;
}

inline constexpr Month& Month::operator--() noexcept
{
   if (--_value == 0)
      _value = 12;
   return *this;
}

inline constexpr Month Month::operator--(int)noexcept
{
   auto tmp(*this);
   --(*this);
   return tmp;
}

inline constexpr Month& Month::operator+=(const Months& m) noexcept
{
   *this = *this + m;
   return *this;
}

inline constexpr Month& Month::operator-=(const Months& m) noexcept
{
   *this = *this - m;
   return *this;
}

inline constexpr Month::operator unsigned() const noexcept
{
   return _value;
}

inline constexpr bool Month::ok() const noexcept
{
   return 1 <= _value and _value <= 12;
}

inline constexpr bool operator==(const Month& lhs, const Month& rhs) noexcept
{
   return static_cast<unsigned>(lhs) == static_cast<unsigned>(rhs);
}

inline constexpr bool operator!=(const Month& lhs, const Month& rhs) noexcept
{
   return not(lhs == rhs);
}

inline constexpr bool operator<(const Month& lhs, const Month& rhs) noexcept
{
   return static_cast<unsigned>(lhs) < static_cast<unsigned>(rhs);
}

inline constexpr bool operator>(const Month& lhs, const Month& rhs) noexcept
{
   return rhs < lhs;
}

inline constexpr bool operator<=(const Month& lhs, const Month& rhs) noexcept
{
   return not(rhs < lhs);
}

inline constexpr bool operator>=(const Month& lhs, const Month& rhs) noexcept
{
   return not(lhs < rhs);
}

constexpr Month inline operator+(const Month& lhs, const Months& rhs) noexcept
{
   const auto mu = static_cast<int32_t>(static_cast<unsigned>(lhs)) - 1 + rhs.count();
   const auto yr = (mu >= 0 ? mu : mu - 11) / 12;

   return Month{static_cast<unsigned>(mu - yr * 12 + 1)};
}

constexpr Month inline operator+(const Months& lhs, const Month& rhs) noexcept
{
   return rhs + lhs;
}

constexpr Month inline operator-(const Month& lhs, const Months& rhs) noexcept
{
   return lhs + -rhs;
}

constexpr Months inline operator-(const Month& lhs, const Month& rhs) noexcept
{
   const auto d = static_cast<unsigned>(lhs) - static_cast<unsigned>(rhs);

   return Months{d <= 11 ? d : d + 12};
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const Month& m)
{
   constexpr const char* text[] = {"January",
                                   "Febuary",
                                   "March",
                                   "April",
                                   "May",
                                   "June",
                                   "July",
                                   "August",
                                   "September",
                                   "October",
                                   "November",
                                   "December"};

   if (m.ok())
   {
      os << at(text, static_cast<uint32_t>(m) - 1);
   }
   else
      os << static_cast<unsigned>(m) << " Is not a valid month";
   return os;
}

//-------------------------------------------------------------------------------------------------
// Day
//-------------------------------------------------------------------------------------------------

inline constexpr Day::Day(unsigned d) noexcept
   : _value(static_cast<Day::value_type>(d))
{
}

inline constexpr Day& Day::operator++() noexcept
{
   ++_value;
   return *this;
}

inline constexpr Day Day::operator++(int)noexcept
{
   auto tmp(*this);
   ++(*this);
   return tmp;
}

inline constexpr Day& Day::operator--() noexcept
{
   --_value;
   return *this;
}

inline constexpr Day Day::operator--(int)noexcept
{
   auto tmp(*this);
   --(*this);
   return tmp;
}

inline constexpr Day& Day::operator+=(const Days& d) noexcept
{
   *this = *this + d;
   return *this;
}

inline constexpr Day& Day::operator-=(const Days& d) noexcept
{
   *this = *this - d;
   return *this;
}

inline constexpr Day::operator unsigned() const noexcept
{
   return _value;
}

inline constexpr bool Day::ok() const noexcept
{
   return 1 <= _value and _value <= 31;
}

inline constexpr bool operator==(const Day& lhs, const Day& rhs) noexcept
{
   return static_cast<unsigned>(lhs) == static_cast<unsigned>(rhs);
}

inline constexpr bool operator!=(const Day& lhs, const Day& rhs) noexcept
{
   return not(lhs == rhs);
}

inline constexpr bool operator<(const Day& lhs, const Day& rhs) noexcept
{
   return static_cast<unsigned>(lhs) < static_cast<unsigned>(rhs);
}

inline constexpr bool operator>(const Day& lhs, const Day& rhs) noexcept
{
   return rhs < lhs;
}

inline constexpr bool operator<=(const Day& lhs, const Day& rhs) noexcept
{
   return not(rhs < lhs);
}

inline constexpr bool operator>=(const Day& lhs, const Day& rhs) noexcept
{
   return not(lhs < rhs);
}

inline constexpr Day operator+(const Day& lhs, const Days& rhs) noexcept
{
   return Day{static_cast<unsigned>(lhs) + static_cast<unsigned>(rhs.count())};
}

inline constexpr Day operator+(const Days& lhs, const Day& rhs) noexcept
{
   return rhs + lhs;
}

inline constexpr Day operator-(const Day& lhs, const Days& rhs) noexcept
{
   return lhs + -rhs;
}

inline constexpr Days operator-(const Day& lhs, const Day& rhs) noexcept
{
   return Days{static_cast<int>(static_cast<unsigned>(lhs)) -
               static_cast<int>(static_cast<unsigned>(rhs))};
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const Day& d)
{
   os.fill('0');
   os.flags(std::ios::dec | std::ios::right);
   os.width(2);
   os << static_cast<unsigned>(d);
   if (not d.ok())
      os << " Is not a valid day";
   return os;
}

//-------------------------------------------------------------------------------------------------
// Weekday
//-------------------------------------------------------------------------------------------------

inline constexpr Weekday::Weekday(unsigned w) noexcept
   : _value(static_cast<Weekday::value_type>(w))
{
}

inline constexpr Weekday::Weekday(const SystemDays& sys_days) noexcept
   : _value(weekday_from_days(sys_days.time_since_epoch().count()))
{
}
inline constexpr Weekday::Weekday(const LocalDays& loc_days) noexcept
   : _value(weekday_from_days(loc_days.time_since_epoch().count()))
{
}

inline constexpr Weekday& Weekday::operator++() noexcept
{
   _value = (_value == 6 ? 0 : _value + 1);
   return *this;
}

inline constexpr Weekday Weekday::operator++(int) noexcept
{
   Weekday tmp{*this};
   ++(*this);
   return tmp;
}

inline constexpr Weekday& Weekday::operator--() noexcept
{
   _value = (_value == 0 ? 6 : _value - 1);
   return *this;
}

inline constexpr Weekday Weekday::operator--(int) noexcept
{
   Weekday tmp{*this};
   --(*this);
   return tmp;
}

inline constexpr Weekday& Weekday::operator+=(const Days& d) noexcept
{
   *this = *this + d; 
   return *this;
}

inline constexpr Weekday& Weekday::operator-=(const Days& d) noexcept
{
   *this = *this - d; 
   return *this;
}

inline constexpr Weekday::operator unsigned() const noexcept
{
   return _value;
}

inline constexpr bool Weekday::ok() const noexcept
{
   return _value <= 6;
}

inline constexpr WeekdayIndexed Weekday::operator[](unsigned idx) const noexcept
{
   return WeekdayIndexed{*this, idx};
}

inline constexpr WeekdayLast Weekday::operator[](LastSpec) const noexcept
{
   return WeekdayLast{*this};
}

inline constexpr bool operator==(const Weekday& lhs, const Weekday& rhs) noexcept
{
   return static_cast<unsigned>(lhs) == static_cast<unsigned>(rhs);
}

inline constexpr bool operator!=(const Weekday& lhs, const Weekday& rhs) noexcept
{
   return not (lhs == rhs);
}

inline constexpr bool operator<(const Weekday& lhs, const Weekday& rhs) noexcept
{
   return static_cast<unsigned>(lhs) < static_cast<unsigned>(rhs);
}

inline constexpr bool operator>(const Weekday& lhs, const Weekday& rhs) noexcept
{
   return rhs < lhs;
}

inline constexpr bool operator<=(const Weekday& lhs, const Weekday& rhs) noexcept
{
   return not (rhs < lhs);
}

inline constexpr bool operator>=(const Weekday& lhs, const Weekday& rhs) noexcept
{
   return not (lhs < rhs);
}

inline constexpr Weekday operator+(const Weekday& lhs, const Days& rhs) noexcept
{
   const auto mu = static_cast<int32_t>(static_cast<unsigned>(lhs)) + rhs.count();
   const auto yr = (mu >= 0 ? mu : mu - 6) / 7;
   return Weekday{static_cast<unsigned>(mu - yr * 7)};
}

inline constexpr Weekday operator+(const Days& lhs, const Weekday& rhs) noexcept
{
   return rhs + lhs;
}

inline constexpr Weekday operator-(const Weekday& lhs, const Days& rhs) noexcept
{
   return lhs + -rhs;
}

inline constexpr Days operator-(const Weekday& lhs, const Weekday& rhs) noexcept
{
   const int wdu = static_cast<unsigned>(lhs) - static_cast<unsigned>(rhs);
   const int wk  = (wdu >= 0 ? wdu : wdu - 6) / 7;
   return Days{wdu - wk * 7};
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const Weekday& wd)
{
   constexpr const char* text[] = {"Sunday",
                                   "Monday",
                                   "Tuesday",
                                   "Wednesday",
                                   "Thursday",
                                   "Friday",
                                   "Saturday"};
   if (wd.ok())
   {
      os << at(text, static_cast<uint32_t>(wd));
   }
   else
      os << static_cast<unsigned>(wd) << " is not a valid weekday";
   return os;
}

//-------------------------------------------------------------------------------------------------
// WeekdayIndexed
//-------------------------------------------------------------------------------------------------

inline constexpr WeekdayIndexed::WeekdayIndexed(const Weekday& wd, unsigned idx) noexcept
   : _wd(wd)
   , _idx(idx)
{
}

inline constexpr Weekday WeekdayIndexed::weekday() const noexcept
{
   return _wd;
}

inline constexpr unsigned WeekdayIndexed::index() const noexcept
{
   return _idx;
}

inline constexpr bool WeekdayIndexed::ok() const noexcept
{
   return _wd.ok() and _idx >= 1 and _idx <= 5;
}

inline constexpr bool operator==(const WeekdayIndexed& lhs, const WeekdayIndexed& rhs) noexcept
{
   return lhs.weekday() == rhs.weekday() and lhs.index() == rhs.index();
}

inline constexpr bool operator!=(const WeekdayIndexed& lhs, const WeekdayIndexed& rhs) noexcept
{
   return not (lhs == rhs);
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const WeekdayIndexed& wdi)
{
   os << wdi.weekday() << '[' << wdi.index();

   if (not (1 <= wdi.index() and wdi.index() <= 5))
      os << " is not a valid index";

   os << ']';
   return os;
}

//-------------------------------------------------------------------------------------------------
// WeekdayLast
//-------------------------------------------------------------------------------------------------

inline constexpr WeekdayLast::WeekdayLast(const Weekday& wd) noexcept
   : _wd(wd)
{
}

inline constexpr Weekday WeekdayLast::weekday() const noexcept
{
   return _wd;
}

inline constexpr bool WeekdayLast::ok() const noexcept
{
   return _wd.ok();
}

inline constexpr bool operator==(const WeekdayLast& lhs, const WeekdayLast& rhs) noexcept
{
   return lhs.weekday() == rhs.weekday();
}

inline constexpr bool operator!=(const WeekdayLast& lhs, const WeekdayLast& rhs) noexcept
{
   return not (lhs == rhs);
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const WeekdayLast& wdl)
{
   return os << wdl.weekday() << "[last]";
}

//-------------------------------------------------------------------------------------------------
// YearMonth
//-------------------------------------------------------------------------------------------------

inline constexpr YearMonth::YearMonth(const Year& y, const Month& m) noexcept
   : _year(y)
   , _month(m)
{
}

inline constexpr Year YearMonth::year() const noexcept
{
   return _year;
}

inline constexpr Month YearMonth::month() const noexcept
{
   return _month;
}

inline constexpr YearMonth& YearMonth::operator+=(const Months& m) noexcept
{
   this->_month += m;
   return *this;
}

inline constexpr YearMonth& YearMonth::operator-=(const Months& m) noexcept
{
   this->_month -= m;
   return *this;
}

inline constexpr YearMonth& YearMonth::operator+=(const Years& y) noexcept
{
   this->_year += y;
   return *this;
}

inline constexpr YearMonth& YearMonth::operator-=(const Years& y) noexcept
{
   this->_year -= y;
   return *this;
}

inline constexpr bool YearMonth::ok() const noexcept
{
   return _year.ok() and _month.ok();
}

inline constexpr bool operator==(const YearMonth& lhs, const YearMonth& rhs) noexcept
{
   return lhs.year() == rhs.year() and lhs.month() == rhs.month();
}

inline constexpr bool operator!=(const YearMonth& lhs, const YearMonth& rhs) noexcept
{
   return not (lhs == rhs);
}

inline constexpr bool operator<(const YearMonth& lhs, const YearMonth& rhs) noexcept
{
   return lhs.year() != rhs.year() ? lhs.year() < rhs.year() : lhs.month() < rhs.month();
}

inline constexpr bool operator>(const YearMonth& lhs, const YearMonth& rhs) noexcept
{
   return rhs < lhs;
}

inline constexpr bool operator<=(const YearMonth& lhs, const YearMonth& rhs) noexcept
{
   return not (rhs < lhs);
}

inline constexpr bool operator>=(const YearMonth& lhs, const YearMonth& rhs) noexcept
{
   return not (lhs < rhs);
}

inline constexpr YearMonth operator+(const YearMonth& lhs, const Months& rhs) noexcept
{
   int dm       = static_cast<int>(static_cast<unsigned>(lhs.month())) - 1 + rhs.count();
   const int dy = (dm >= 0 ? dm : dm - 11) / 12;
   dm           = dm - dy * 12 + 1;
   return (lhs.year() + Years{dy}) / Month{static_cast<unsigned>(dm)};
}

inline constexpr YearMonth operator+(const Months& lhs, const YearMonth& rhs) noexcept
{
   return rhs + lhs;
}

inline constexpr YearMonth operator+(const YearMonth& lhs, const Years& rhs) noexcept
{
   return (lhs.year() + rhs) / lhs.month();
}

inline constexpr YearMonth operator+(const Years& lhs, const YearMonth& rhs) noexcept
{
   return rhs + lhs;
}

inline constexpr Months operator-(const YearMonth& lhs, const YearMonth& rhs) noexcept
{
   return (lhs.year() - rhs.year()) +
          Months{static_cast<unsigned>(lhs.month()) - static_cast<unsigned>(rhs.month())};
}

inline constexpr YearMonth operator-(const YearMonth& lhs, const Months& rhs) noexcept
{
   return lhs + -rhs;
}

inline constexpr YearMonth operator-(const YearMonth& lhs, const Years& rhs) noexcept
{
   return lhs + -rhs;
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const YearMonth& ym)
{
   return os << ym.year() << '/' << ym.month();
}

//-------------------------------------------------------------------------------------------------
// MonthDay
//-------------------------------------------------------------------------------------------------

inline constexpr MonthDay::MonthDay(const Month& m, const Day& d) noexcept
   : _month{m}
   , _day{d}
{
}

inline constexpr Month MonthDay::month() const noexcept
{
   return _month;
}

inline constexpr Day MonthDay::day() const noexcept
{
   return _day;
}
inline constexpr bool MonthDay::ok() const noexcept
{
   constexpr const Day d[] = {Day{31},
                              Day{29},
                              Day{31},
                              Day{30},
                              Day{31},
                              Day{30},
                              Day{31},
                              Day{31},
                              Day{30},
                              Day{31},
                              Day{30},
                              Day{31}};
   return _month.ok() and Day{1} <= _day and _day <= at(d, static_cast<unsigned>(_month) - 1);
}

inline constexpr bool operator==(const MonthDay& lhs, const MonthDay& rhs) noexcept
{
   return lhs.month() == rhs.month() and lhs.day() == rhs.day();
}

inline constexpr bool operator!=(const MonthDay& lhs, const MonthDay& rhs) noexcept
{
   return not(lhs == rhs);
}

inline constexpr bool operator<(const MonthDay& lhs, const MonthDay& rhs) noexcept
{
   return lhs.month() != rhs.month() ? lhs.month() < rhs.month() : lhs.day() < rhs.day();
}

inline constexpr bool operator>(const MonthDay& lhs, const MonthDay& rhs) noexcept
{
   return rhs < lhs;
}

inline constexpr bool operator<=(const MonthDay& lhs, const MonthDay& rhs) noexcept
{
   return not(rhs < lhs);
}

inline constexpr bool operator>=(const MonthDay& lhs, const MonthDay& rhs) noexcept
{
   return not(lhs < rhs);
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const MonthDay& md)
{
   return os << md.month() << '/' << md.day();
}

//-------------------------------------------------------------------------------------------------
// MonthWeekday
//-------------------------------------------------------------------------------------------------

inline constexpr MonthWeekday::MonthWeekday(const Month& m, const WeekdayIndexed& wd) noexcept
   : _month(m)
   , _wdi(wd)
{
}

inline constexpr Month MonthWeekday::month() const noexcept
{
   return _month;
}

inline constexpr WeekdayIndexed MonthWeekday::weekday_indexed() const noexcept
{
   return _wdi;
}

inline constexpr bool MonthWeekday::ok() const noexcept
{
   return _month.ok() and _wdi.ok();
}

inline constexpr bool operator==(const MonthWeekday& lhs, const MonthWeekday& rhs) noexcept
{
   return lhs.month() == rhs.month() and lhs.weekday_indexed() == rhs.weekday_indexed();
}

inline constexpr bool operator!=(const MonthWeekday& lhs, const MonthWeekday& rhs) noexcept
{
   return not (lhs == rhs);
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const MonthWeekday& mwd)
{
   return os << mwd.month() << '/' << mwd.weekday_indexed();
}

//-------------------------------------------------------------------------------------------------
// Date
//-------------------------------------------------------------------------------------------------

inline constexpr Date::Date(const Year& y, const Month& m, const Day& d) noexcept
   : _year(y)
   , _month(m)
   , _day(d)
{
}

inline constexpr Date::Date(const Day& d, const Month& m, const Year& y) noexcept
   : _year(y)
   , _month(m)
   , _day(d)
{
}

inline constexpr Date::Date(const Date& other) noexcept
   : _year(other.year())
   , _month(other.month())
   , _day(other.day())
{
}

inline constexpr Date::Date(const SystemDays& sys_days) noexcept
   : Date(from_days(sys_days.time_since_epoch()))
{
}

inline constexpr Date::Date(const LocalDays& loc_days) noexcept
   : Date(from_days(loc_days.time_since_epoch()))
{
}

inline constexpr Date& Date::operator+=(const Months& months) noexcept
{
   *this = *this + months; 
   return *this;
}

inline constexpr Date& Date::operator-=(const Months& months) noexcept
{
   *this = *this - months; 
   return *this;
}

inline constexpr Date& Date::operator+=(const Years& years) noexcept
{
   *this = *this + years; 
   return *this;
}

inline constexpr Date& Date::operator-=(const Years& years) noexcept
{
   *this = *this - years; 
   return *this;
}

inline constexpr Year Date::year() const noexcept
{
   return _year;
}
inline constexpr Month Date::month() const noexcept
{
   return _month;
}
inline constexpr Day Date::day() const noexcept
{
   return _day;
}
inline constexpr Date::operator SystemDays() const noexcept
{
   return SystemDays{to_days()};
}
inline constexpr Date::operator LocalDays() const noexcept
{
   return LocalDays{to_days()};
}

inline constexpr bool Date::ok() const noexcept 
{
   if (not _year.ok() or not _month.ok()) 
      return false;

   return Day{1} <= _day and _day <= (_year / _month / Last).day();
}

inline constexpr bool operator==(const Date& lhs, const Date& rhs) noexcept
{
   return lhs.year() == rhs.year() and lhs.month() == rhs.month() and
          lhs.day() == rhs.day();
}

inline constexpr bool operator!=(const Date& lhs, const Date& rhs) noexcept
{
   return not (lhs == rhs);
}

inline constexpr bool operator<(const Date& lhs, const Date& rhs) noexcept
{
   if (lhs.year() < rhs.year())
      return true;
   if (lhs.year() > rhs.year())
      return false;
   if (lhs.month() < rhs.month())
      return true;
   if (lhs.month() > rhs.month())
      return false;
   return lhs.day() < rhs.day();
}

inline constexpr bool operator>(const Date& lhs, const Date& rhs) noexcept
{
   return rhs < lhs;
}

inline constexpr bool operator<=(const Date& lhs, const Date& rhs) noexcept
{
   return not (rhs < lhs);
}

inline constexpr bool operator>=(const Date& lhs, const Date& rhs) noexcept
{
   return not (lhs < rhs);
}

inline constexpr Date operator+(const Date& lhs, const Months& rhs) noexcept
{
   return {lhs.year(), lhs.month() + rhs, lhs.day()};
}

inline constexpr Date operator+(const Months& lhs, const Date& rhs) noexcept
{
   return rhs + lhs;
}

inline constexpr Date operator-(const Date& lhs, const Months& rhs) noexcept
{
   return lhs + -rhs;
}

inline constexpr Date operator+(const Date& lhs, const Years& rhs) noexcept
{
   return {(lhs.year() + rhs), lhs.month(), lhs.day()};
}

inline constexpr Date operator+(const Years& lhs, const Date& rhs) noexcept
{
   return rhs + lhs;
}

inline constexpr Date operator-(const Date& lhs, const Years& rhs) noexcept
{
   return lhs + -rhs;
}

inline constexpr Date Date::from_days(Days d) noexcept
{
   const auto ret = orion::from_days(d.count());

   return Date{Year{std::get<0>(ret)}, Month{std::get<1>(ret)}, Day{std::get<2>(ret)}};
}

inline constexpr Days Date::to_days() const noexcept
{
   auto y = static_cast<int>(_year);
   auto m = static_cast<uint32_t>(_month);
   auto d = static_cast<uint32_t>(_day);

   return Days{orion::to_days(y, m, d)};
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const Date& d)
{
   //detail::save_ostream<CharT, Traits> _(os);

   os.fill('0');
   os.flags(std::ios::dec | std::ios::right);

   os << d.year() << '/';

   os.width(2);
   
   os << static_cast<unsigned>(d.month()) << '/';
   os << d.day();
   
   if (not d.ok())
      os << " is not a valid date";
   return os;
}

//-------------------------------------------------------------------------------------------------

inline constexpr YearMonth operator/(const Year& y, const Month& m) noexcept
{
   return {y, m};
}

inline constexpr YearMonth operator/(const Year& y, int m) noexcept
{
   return {y, Month{narrow_cast<unsigned>(m)}};
}

inline constexpr MonthDay operator/(const Day& d, const Month& m) noexcept
{
   return {m, d};
}

inline constexpr MonthDay operator/(const Day& d, int m) noexcept
{
   return {Month{narrow_cast<unsigned>(m)}, d};
}

inline constexpr MonthDay operator/(const Month& m, const Day& d) noexcept
{
   return {m, d};
}

inline constexpr MonthDay operator/(const Month& m, int d) noexcept
{
   return {m, Day{narrow_cast<unsigned>(d)}};
}

inline constexpr MonthDay operator/(const Month& m, LastSpec) noexcept
{
   auto d = Day{last_day_of_month_common_year(static_cast<uint32_t>(m))};
   return MonthDay{m, d};
}

inline constexpr MonthDay operator/(int m, const Day& d) noexcept
{
   return {Month{narrow_cast<unsigned>(m)}, d};
}

inline constexpr MonthWeekday operator/(const Month& lhs, const WeekdayIndexed& rhs) noexcept
{
   return MonthWeekday{lhs, rhs};
}

inline constexpr MonthWeekday operator/(int lhs, const WeekdayIndexed& rhs) noexcept
{
   return MonthWeekday{Month{static_cast<uint32_t>(lhs)}, rhs};
}

inline constexpr MonthWeekday operator/(const WeekdayIndexed& lhs, const Month& rhs) noexcept
{
   return MonthWeekday{rhs, lhs};
}

inline constexpr MonthWeekday operator/(const WeekdayIndexed& lhs, int rhs) noexcept
{
   return MonthWeekday{Month{static_cast<uint32_t>(rhs)}, lhs};
}

inline constexpr Date operator/(const YearMonth& ym, const Day& d) noexcept
{
   return Date{ym.year(), ym.month(), d};
}

inline constexpr Date operator/(const YearMonth& ym, int d) noexcept
{
   return Date{ym.year(), ym.month(), Day{narrow_cast<unsigned>(d)}};
}

inline constexpr Date operator/(const YearMonth& ym,  LastSpec) noexcept
{
   const auto d = last_day_of_month(static_cast<int>(ym.year()), static_cast<uint32_t>(ym.month()));
   return Date{ym.year(), ym.month(), Day{d}};
}

inline constexpr Date operator/(const Year& y, const MonthDay& md) noexcept
{
   if (md.month() == Month{2u} and md.day() == Day{28})
   {
      const auto d = last_day_of_month(static_cast<int>(y), static_cast<uint32_t>(md.month()));
      return Date{y, md.month(), Day{d}};
   }
   return Date{y, md.month(), md.day()};
}

inline constexpr Date operator/(int y, const MonthDay& md) noexcept
{
   if (md.month() == Month{2u} and md.day() == Day{28})
   {
      const auto d = last_day_of_month(y, static_cast<uint32_t>(md.month()));
      return Date{Year{y}, md.month(), Day{d}};
   }
   return Date{Year{y}, md.month(), md.day()};
}

inline constexpr Date operator/(const MonthDay& md, const Year& y) noexcept
{
   if (md.month() == Month{2u} and md.day() == Day{28})
   {
      const auto d = last_day_of_month(static_cast<int>(y), static_cast<uint32_t>(md.month()));
      return Date{y, md.month(), Day{d}};
   }
   return Date{y, md.month(), md.day()};
}

inline constexpr Date operator/(const MonthDay& md, int  y) noexcept
{
   if (md.month() == Month{2u} and md.day() == Day{28})
   {
      const auto d = last_day_of_month(y, static_cast<uint32_t>(md.month()));
      return Date{Year{y}, md.month(), Day{d}};
   }
   return Date{Year{y}, md.month(), md.day()};
}

//-------------------------------------------------------------------------------------------------

template<class Rep,
         class Period,
         class = typename std::enable_if<!std::chrono::treat_as_floating_point<Rep>::value>::type>
inline constexpr TimeOfDay<std::chrono::duration<Rep, Period>> make_time(
   const std::chrono::duration<Rep, Period>& d)
{
   return TimeOfDay<std::chrono::duration<Rep, Period>>{d};
}

inline constexpr TimeOfDay<std::chrono::hours> make_time(const std::chrono::hours& h)
{
   return TimeOfDay<std::chrono::hours>{h};
}

inline constexpr TimeOfDay<std::chrono::minutes> make_time(const std::chrono::hours& h,
                                                           const std::chrono::minutes& m)
{
   return TimeOfDay<std::chrono::minutes>{h, m};
}

inline constexpr TimeOfDay<std::chrono::seconds> make_time(const std::chrono::hours& h,
                                                           const std::chrono::minutes& m,
                                                           const std::chrono::seconds& s)
{
   return TimeOfDay<std::chrono::seconds>{h, m, s};
}

template<class Rep,
         class Period,
         class = typename std::enable_if<std::ratio_less<Period, std::ratio<1>>::value>::type>
inline constexpr TimeOfDay<std::chrono::duration<Rep, Period>> make_time(
   const std::chrono::hours& h,
   const std::chrono::minutes& m,
   const std::chrono::seconds& s,
   const std::chrono::duration<Rep, Period>& sub_s)
{
   return TimeOfDay<std::chrono::duration<Rep, Period>>{h, m, s, sub_s};
}

} // namespace orion

#endif // ORION_DATETIME_IPP
