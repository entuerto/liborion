//
// Datetime.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
//-------------------------------------------------------------------------------------------------
// This Code is based on:
//   chrono-Compatible Low-Level Date Algorithms
//   2016 Howard Hinnant
//   http://howardhinnant.github.io/date_algorithms.html
// ------------------------------------------------------------------------------------------------
#ifndef ORION_DATETIME_H
#define ORION_DATETIME_H

#include <orion/Orion-Stddefs.h>

#include <orion/Chrono.h>

#include <chrono>

namespace orion
{

struct LastSpec
{
   explicit LastSpec() = default;
};

constexpr const LastSpec Last{};

//-------------------------------------------------------------------------------------------------

///
/// Represents a year in the Gregorian calendar
///
class Year
{
public:
   using value_type = int16_t;

   Year() = default;
   explicit constexpr Year(int y) noexcept;

   constexpr Year& operator++() noexcept;
   constexpr Year operator++(int) noexcept;
   constexpr Year& operator--() noexcept;
   constexpr Year operator--(int) noexcept;

   constexpr Year& operator+=(const Years& y) noexcept;
   constexpr Year& operator-=(const Years& y) noexcept;

   constexpr Year operator-() const noexcept;
   constexpr Year operator+() const noexcept;

   /// Determines if the year is a leap year.
   constexpr bool is_leap() const noexcept;

   /// Retrieves the stored year value.
   constexpr explicit operator int() const noexcept;
   /// Checks if the stored year value is valid.
   constexpr bool ok() const noexcept;

   /// Returns the smallest possible year.
   static constexpr Year min() noexcept;
   /// Returns the largest possible year.
   static constexpr Year max() noexcept;

private:
   value_type _value;
};

constexpr bool operator==(const Year& x, const Year& y) noexcept;
constexpr bool operator!=(const Year& x, const Year& y) noexcept;
constexpr bool operator<(const Year& x, const Year& y) noexcept;
constexpr bool operator>(const Year& x, const Year& y) noexcept;
constexpr bool operator<=(const Year& x, const Year& y) noexcept;
constexpr bool operator>=(const Year& x, const Year& y) noexcept;

constexpr Year operator+(const Year& x, const Years& y) noexcept;
constexpr Year operator+(const Years& x, const Year& y) noexcept;
constexpr Year operator-(const Year& x, const Years& y) noexcept;
constexpr Years operator-(const Year& x, const Year& y) noexcept;

//-------------------------------------------------------------------------------------------------

///
/// Represents a month of a year 
///
class Month
{
public:
   using value_type = uint8_t;

   Month() = default;
   explicit constexpr Month(unsigned m) noexcept;

   constexpr Month& operator++() noexcept;
   constexpr Month operator++(int) noexcept;
   constexpr Month& operator--() noexcept;
   constexpr Month operator--(int) noexcept;

   constexpr Month& operator+=(const Months& m) noexcept;
   constexpr Month& operator-=(const Months& m) noexcept;

   /// Retrieves the stored month value.
   constexpr explicit operator unsigned() const noexcept;
   /// Checks if the stored month value is valid.
   constexpr bool ok() const noexcept;

private:
   value_type _value;
};

constexpr bool operator==(const Month& lhs, const Month& rhs) noexcept;
constexpr bool operator!=(const Month& lhs, const Month& rhs) noexcept;
constexpr bool operator<(const Month& lhs, const Month& rhs) noexcept;
constexpr bool operator>(const Month& lhs, const Month& rhs) noexcept;
constexpr bool operator<=(const Month& lhs, const Month& rhs) noexcept;
constexpr bool operator>=(const Month& lhs, const Month& rhs) noexcept;

constexpr Month operator+(const Month& lhs, const Months& rhs) noexcept;
constexpr Month operator+(const Months& lhs, const Month& rhs) noexcept;
constexpr Month operator-(const Month& lhs, const Months& rhs) noexcept;
constexpr Months operator-(const Month& lhs, const Month& rhs) noexcept;

//-------------------------------------------------------------------------------------------------

///
/// Represents a day of a month 
///
class Day
{
public:
   using value_type = uint8_t;

   Day() = default;
   explicit constexpr Day(unsigned d) noexcept;

   constexpr Day& operator++() noexcept;
   constexpr Day operator++(int) noexcept;
   constexpr Day& operator--() noexcept;
   constexpr Day operator--(int) noexcept;

   constexpr Day& operator+=(const Days& d) noexcept;
   constexpr Day& operator-=(const Days& d) noexcept;

   /// Retrieves the stored day value.
   constexpr explicit operator unsigned() const noexcept;
   /// Checks if the stored day value is valid.
   constexpr bool ok() const noexcept;

private:
   value_type _value;
};

constexpr bool operator==(const Day& lhs, const Day& rhs) noexcept;
constexpr bool operator!=(const Day& lhs, const Day& rhs) noexcept;
constexpr bool operator<(const Day& lhs, const Day& rhs) noexcept;
constexpr bool operator>(const Day& lhs, const Day& rhs) noexcept;
constexpr bool operator<=(const Day& lhs, const Day& rhs) noexcept;
constexpr bool operator>=(const Day& lhs, const Day& rhs) noexcept;

constexpr Day operator+(const Day& lhs, const Days& rhs) noexcept;
constexpr Day operator+(const Days& lhs, const Day& rhs) noexcept;
constexpr Day operator-(const Day& lhs, const Days& rhs) noexcept;
constexpr Days operator-(const Day& lhs, const Day& rhs) noexcept;

//-------------------------------------------------------------------------------------------------

class WeekdayIndexed;
class WeekdayLast;

///
/// Represents a day of the week in the Gregorian calendar
///
class Weekday
{
public:
   using value_type = uint8_t;

   Weekday() = default;
   explicit constexpr Weekday(unsigned w) noexcept;
   constexpr Weekday(const SystemDays& sysdays) noexcept;
   explicit constexpr Weekday(const LocalDays& loc_days) noexcept;

   constexpr Weekday& operator++() noexcept;
   constexpr Weekday operator++(int) noexcept;
   constexpr Weekday& operator--() noexcept;
   constexpr Weekday operator--(int) noexcept;

   constexpr Weekday& operator+=(const Days& d) noexcept;
   constexpr Weekday& operator-=(const Days& d) noexcept;

   /// Retrieves the stored weekday value 
   explicit constexpr operator unsigned() const noexcept;
   /// Checks if the stored weekday value is valid 
   constexpr bool ok() const noexcept;

   constexpr WeekdayIndexed operator[](unsigned idx) const noexcept;
   constexpr WeekdayLast operator[](LastSpec) const noexcept;

private:   
   value_type _value;
};

constexpr bool operator==(const Weekday& lhs, const Weekday& rhs) noexcept;
constexpr bool operator!=(const Weekday& lhs, const Weekday& rhs) noexcept;
constexpr bool operator<(const Weekday& lhs, const Weekday& rhs) noexcept;
constexpr bool operator>(const Weekday& lhs, const Weekday& rhs) noexcept;
constexpr bool operator<=(const Weekday& lhs, const Weekday& rhs) noexcept;
constexpr bool operator>=(const Weekday& lhs, const Weekday& rhs) noexcept;

constexpr Weekday operator+(const Weekday& lhs, const Days& rhs) noexcept;
constexpr Weekday operator+(const Days& lhs, const Weekday& rhs) noexcept;
constexpr Weekday operator-(const Weekday& lhs, const Days& rhs) noexcept;
constexpr Days operator-(const Weekday& lhs, const Weekday& rhs) noexcept;

//-------------------------------------------------------------------------------------------------

///
/// The class WeekdayIndexed combines a weekday, representing a day of the week in the 
/// Gregorian calendar, with a small index n in the range [1, 5]. It represents the 
/// first, second, third, fourth, or fifth weekday of some month.
///
class WeekdayIndexed
{
public:
   WeekdayIndexed() = default;
   constexpr WeekdayIndexed(const Weekday& wd, unsigned idx) noexcept;

   /// Retrieves a copy of the Weekday object.
   constexpr Weekday weekday() const noexcept;
   /// Retrieves the stored index value.
   constexpr unsigned index() const noexcept;
   /// Checks if the weekday and index are both valid.
   constexpr bool ok() const noexcept;

private:
   Weekday _wd;
   uint8_t _idx;
};

constexpr bool operator==(const WeekdayIndexed& lhs, const WeekdayIndexed& rhs) noexcept;
constexpr bool operator!=(const WeekdayIndexed& lhs, const WeekdayIndexed& rhs) noexcept;

//-------------------------------------------------------------------------------------------------

///
/// Represents the last weekday of some month.
///
class WeekdayLast
{
public:
   explicit constexpr WeekdayLast(const Weekday& wd) noexcept;

   /// Retrieves a copy of the Weekday object.
   constexpr Weekday weekday() const noexcept;
   /// Checks if the stored weekday is valid.
   constexpr bool ok() const noexcept;

private:
   Weekday _wd;
};

constexpr bool operator==(const WeekdayLast& lhs, const WeekdayLast& rhs) noexcept;
constexpr bool operator!=(const WeekdayLast& lhs, const WeekdayLast& rhs) noexcept;

//-------------------------------------------------------------------------------------------------

///
/// The class YearMonth represents a specific month of a specific year, but with an unspecified day. 
///
class YearMonth
{

public:
   YearMonth() = default;

   constexpr YearMonth(const Year& y, const Month& m) noexcept;
   
   /// Retrieves a copy of the Year object.
   constexpr Year year() const noexcept;
   /// Retrieves a copy of the Month object.
   constexpr Month month() const noexcept;

   constexpr YearMonth& operator+=(const Months& m) noexcept;
   constexpr YearMonth& operator-=(const Months& m) noexcept;
   constexpr YearMonth& operator+=(const Years& y) noexcept;
   constexpr YearMonth& operator-=(const Years& y) noexcept;

   /// Checks whether the contained Year and Month are valid.
   constexpr bool ok() const noexcept; 

private:
   Year _year;
   Month _month;
};

constexpr bool operator==(const YearMonth& lhs, const YearMonth& rhs) noexcept;
constexpr bool operator!=(const YearMonth& lhs, const YearMonth& rhs) noexcept;
constexpr bool operator<(const YearMonth& lhs, const YearMonth& rhs) noexcept;
constexpr bool operator>(const YearMonth& lhs, const YearMonth& rhs) noexcept;
constexpr bool operator<=(const YearMonth& lhs, const YearMonth& rhs) noexcept;
constexpr bool operator>=(const YearMonth& lhs, const YearMonth& rhs) noexcept;

constexpr YearMonth operator+(const YearMonth& lhs, const Months& rhs) noexcept;
constexpr YearMonth operator+(const Months& lhs, const YearMonth& rhs) noexcept;
constexpr YearMonth operator+(const YearMonth& lhs, const Years& rhs) noexcept;
constexpr YearMonth operator+(const Years& lhs, const YearMonth& rhs) noexcept;
constexpr Months operator-(const YearMonth& lhs, const YearMonth& rhs) noexcept;
constexpr YearMonth operator-(const YearMonth& lhs, const Months& rhs) noexcept;
constexpr YearMonth operator-(const YearMonth& lhs, const Years& rhs) noexcept;

//-------------------------------------------------------------------------------------------------

///
/// The class MonthDay represents a specific day of a specific month, of some 
/// yet to be specified year.
///
class MonthDay
{
public:
   MonthDay() = default;
   constexpr MonthDay(const Month& m, const Day& d) noexcept;

   /// Retrieves a copy of the Month object.
   constexpr Month month() const noexcept;
   /// Retrieves a copy of the Day object.
   constexpr Day day() const noexcept;
   /// Checks whether the contained Month and Day are valid.
   constexpr bool ok() const noexcept;

private:
   Month _month;
   Day _day;
};

constexpr bool operator==(const MonthDay& lhs, const MonthDay& rhs) noexcept;
constexpr bool operator!=(const MonthDay& lhs, const MonthDay& rhs) noexcept;
constexpr bool operator<(const MonthDay& lhs, const MonthDay& rhs) noexcept;
constexpr bool operator>(const MonthDay& lhs, const MonthDay& rhs) noexcept;
constexpr bool operator<=(const MonthDay& lhs, const MonthDay& rhs) noexcept;
constexpr bool operator>=(const MonthDay& lhs, const MonthDay& rhs) noexcept;

//-------------------------------------------------------------------------------------------------

///
/// The class month_weekday represents the n-th weekday of a specific month, of some 
/// yet to be specified year.
///
class MonthWeekday
{
public:
   MonthWeekday() = default;

   constexpr MonthWeekday(const Month& m, const WeekdayIndexed& wd) noexcept;

   /// Retrieves a copy of the Month object.
   constexpr Month month() const noexcept;
   /// Retrieves a copy of the WeekdayIndexed object.
   constexpr WeekdayIndexed weekday_indexed() const noexcept;
   /// Checks whether the contained Month and WeekdayIndexed are valid.
   constexpr bool ok() const noexcept;

private:
   Month _month;
   WeekdayIndexed _wdi;
};

constexpr bool operator==(const MonthWeekday& lhs, const MonthWeekday& rhs) noexcept;
constexpr bool operator!=(const MonthWeekday& lhs, const MonthWeekday& rhs) noexcept;

//-------------------------------------------------------------------------------------------------

///
/// The class Date represents a specific year, month, and day. It is a field-based time point, 
/// with a resolution of Days.
///
class Date
{
public:
   Date() = default;
   constexpr Date(const Year& y, const Month& m, const Day& d) noexcept;
   constexpr Date(const Day& d, const Month& m, const Year& y) noexcept;
   constexpr Date(const Date& other) noexcept;
   constexpr Date(const SystemDays& sys_days) noexcept;
   explicit constexpr Date(const LocalDays& loc_days) noexcept;

   constexpr Date& operator+=(const Months& months) noexcept;
   constexpr Date& operator-=(const Months& months) noexcept;
   constexpr Date& operator+=(const Years& years) noexcept;
   constexpr Date& operator-=(const Years& years) noexcept;

   constexpr Year year() const noexcept;
   constexpr Month month() const noexcept;
   constexpr Day day() const noexcept;
   constexpr operator SystemDays() const noexcept;
   explicit constexpr operator LocalDays() const noexcept;

   constexpr bool ok() const noexcept;

private:
   static constexpr Date from_days(Days d) noexcept;
   constexpr Days to_days() const noexcept;

   Year _year;
   Month _month;
   Day _day;
};

constexpr bool operator==(const Date& lhs, const Date& rhs) noexcept;
constexpr bool operator!=(const Date& lhs, const Date& rhs) noexcept;
constexpr bool operator<(const Date& lhs, const Date& rhs) noexcept;
constexpr bool operator>(const Date& lhs, const Date& rhs) noexcept;
constexpr bool operator<=(const Date& lhs, const Date& rhs) noexcept;
constexpr bool operator>=(const Date& lhs, const Date& rhs) noexcept;

constexpr Date operator+(const Date& lhs, const Months& rhs) noexcept;
constexpr Date operator+(const Months& lhs, const Date& rhs) noexcept;
constexpr Date operator-(const Date& lhs, const Months& rhs) noexcept;
constexpr Date operator+(const Date& lhs, const Years& rhs) noexcept;
constexpr Date operator+(const Years& lhs, const Date& rhs) noexcept;
constexpr Date operator-(const Date& lhs, const Years& rhs) noexcept;

//-------------------------------------------------------------------------------------------------
// Date composition operators

constexpr YearMonth operator/(const Year& y, const Month& m) noexcept;
constexpr YearMonth operator/(const Year& y, int          m) noexcept;

constexpr MonthDay operator/(const Day& d, const Month& m) noexcept;
constexpr MonthDay operator/(const Day& d, int          m) noexcept;
constexpr MonthDay operator/(const Month& m, const Day& d) noexcept;
constexpr MonthDay operator/(const Month& m, int        d) noexcept;
constexpr MonthDay operator/(const Month& m,     LastSpec) noexcept;
constexpr MonthDay operator/(int m, const Day& d) noexcept;

constexpr MonthWeekday operator/(const Month& lhs, const WeekdayIndexed& rhs) noexcept;
constexpr MonthWeekday operator/(int lhs, const WeekdayIndexed& rhs) noexcept;
constexpr MonthWeekday operator/(const WeekdayIndexed& lhs, const Month& rhs) noexcept;
constexpr MonthWeekday operator/(const WeekdayIndexed& lhs, int rhs) noexcept;

constexpr Date operator/(const YearMonth& ym, const Day& d) noexcept;
constexpr Date operator/(const YearMonth& ym, int        d) noexcept;
constexpr Date operator/(const YearMonth& ym,     LastSpec) noexcept;
constexpr Date operator/(const Year& y, const MonthDay& md) noexcept;
constexpr Date operator/(int         y, const MonthDay& md) noexcept;
constexpr Date operator/(const MonthDay& md, const Year& y) noexcept;
constexpr Date operator/(const MonthDay& md, int         y) noexcept;

//-------------------------------------------------------------------------------------------------

template<class Duration, typename Tag = void>
class TimeOfDayBase;

template<class Rep, class Period>
class TimeOfDayBase<std::chrono::duration<Rep, Period>,
                    typename std::enable_if<std::is_same<std::chrono::duration<Rep, Period>,
                                                         std::chrono::hours>::value>::type>
{
public:
   using precision = std::chrono::hours;

   constexpr TimeOfDayBase() noexcept = default;

   constexpr explicit TimeOfDayBase(std::chrono::hours since_midnight) noexcept
      : _hrs(since_midnight)
   {
   }

   /// The integral number of hours since_midnight is after 00:00:00.
   constexpr std::chrono::hours hours() const noexcept { return _hrs; }

   constexpr explicit operator precision() const noexcept { return to_duration(); }
   /// Returns the stored duration.
   constexpr precision to_duration() const noexcept { return _hrs; }

private:
   std::chrono::hours _hrs;
};

template<class Rep, class Period>
class TimeOfDayBase<std::chrono::duration<Rep, Period>,
                    typename std::enable_if<std::is_same<std::chrono::duration<Rep, Period>,
                                                         std::chrono::minutes>::value>::type>
{
public:
   using precision = std::chrono::minutes;

   constexpr TimeOfDayBase() noexcept = default;

   constexpr explicit TimeOfDayBase(std::chrono::minutes since_midnight) noexcept
      : _hrs(std::chrono::duration_cast<std::chrono::hours>(since_midnight))
      , _mins(since_midnight - _hrs)
   {
   }

   constexpr explicit TimeOfDayBase(std::chrono::hours h, std::chrono::minutes m) noexcept
      : _hrs(h)
      , _mins(m)
   {
   }

   /// The integral number of hours since_midnight is after 00:00:00.
   constexpr std::chrono::hours hours() const noexcept { return _hrs; }
   /// The integral number of minutes since_midnight is after (00:00:00 + hours()).
   constexpr std::chrono::minutes minutes() const noexcept { return _mins; }

   constexpr explicit operator precision() const noexcept { return to_duration(); }
   /// Returns the stored duration.
   constexpr precision to_duration() const noexcept
   {
      auto p = _hrs + _mins;
      return std::chrono::duration_cast<precision>(p);
   }

private:
   std::chrono::hours _hrs;
   std::chrono::minutes _mins;
};

template<class Rep, class Period>
class TimeOfDayBase<std::chrono::duration<Rep, Period>,
                    typename std::enable_if<std::is_same<std::chrono::duration<Rep, Period>,
                                                         std::chrono::seconds>::value>::type>
{
public:
   using precision = std::chrono::seconds;

   constexpr TimeOfDayBase() noexcept = default;

   constexpr explicit TimeOfDayBase(std::chrono::seconds since_midnight) noexcept
      : _hrs(std::chrono::duration_cast<std::chrono::hours>(since_midnight))
      , _mins(std::chrono::duration_cast<std::chrono::minutes>(since_midnight - _hrs))
      , _secs(since_midnight - _hrs - _mins)
   {
   }

   constexpr explicit TimeOfDayBase(std::chrono::hours h,
                                    std::chrono::minutes m,
                                    std::chrono::seconds s) noexcept
      : _hrs(h)
      , _mins(m)
      , _secs(s)
   {
   }

   /// The integral number of hours since_midnight is after 00:00:00.
   constexpr std::chrono::hours hours() const noexcept { return _hrs; }
   /// The integral number of minutes since_midnight is after (00:00:00 + hours()).
   constexpr std::chrono::minutes minutes() const noexcept { return _mins; }
   /// The integral number of seconds since_midnight is after (00:00:00 + hours() + minutes()).
   constexpr std::chrono::seconds seconds() const noexcept { return _secs; }

   constexpr explicit operator precision() const noexcept { return to_duration(); }
   /// Returns the stored duration.
   constexpr precision to_duration() const noexcept
   {
      auto p = _hrs + _mins + _secs;
      return std::chrono::duration_cast<precision>(p);
   }

private:
   std::chrono::hours _hrs;
   std::chrono::minutes _mins;
   std::chrono::seconds _secs;
};

template<class Rep, class Period>
class TimeOfDayBase <std::chrono::duration<Rep, Period>,
                     typename std::enable_if<std::ratio_less<Period, std::ratio<1>>::value>::type>
{
public:
   using precision = std::chrono::duration<Rep, Period>;

   constexpr TimeOfDayBase() noexcept = default;

   constexpr explicit TimeOfDayBase(precision since_midnight) noexcept
      : _hrs(std::chrono::duration_cast<std::chrono::hours>(since_midnight))
      , _mins(std::chrono::duration_cast<std::chrono::minutes>(since_midnight - _hrs))
      , _secs(std::chrono::duration_cast<std::chrono::seconds>(since_midnight - _hrs - _mins))
      , _subs(since_midnight - _hrs - _mins - _secs)
   {
   }

   constexpr explicit TimeOfDayBase(std::chrono::hours h,
                                    std::chrono::minutes m,
                                    std::chrono::seconds s,
                                    precision sub) noexcept
      : _hrs(h)
      , _mins(m)
      , _secs(s)
      , _subs(sub)
   {
   }

   /// The integral number of hours since_midnight is after 00:00:00.
   constexpr std::chrono::hours hours() const noexcept { return _hrs; }
   /// The integral number of minutes since_midnight is after (00:00:00 + hours()).
   constexpr std::chrono::minutes minutes() const noexcept { return _mins; }
   /// The integral number of seconds since_midnight is after (00:00:00 + hours() + minutes()).
   constexpr std::chrono::seconds seconds() const noexcept { return _secs; }
   /// The integral number of fractional seconds since_midnight is after
   /// (00:00:00 + hours() + minutes() + seconds()).
   constexpr precision subseconds() const noexcept { return _subs; }

   constexpr explicit operator precision() const noexcept { return to_duration(); }
   /// Returns the stored duration.
   constexpr precision to_duration() const noexcept
   {
      auto p = _hrs + _mins + _secs + _subs;
      return std::chrono::duration_cast<precision>(p);
   }

private:
   std::chrono::hours _hrs;
   std::chrono::minutes _mins;
   std::chrono::seconds _secs;
   precision _subs;
};

template<class Duration>
class TimeOfDay : public TimeOfDayBase<Duration>
{
public:
   constexpr TimeOfDay() noexcept = default;

   constexpr explicit TimeOfDay(Duration since_midnight) noexcept
      : TimeOfDayBase<Duration>(since_midnight)
   {
   }

   template<class Arg0, class Arg1, class... Args>
   constexpr explicit TimeOfDay(Arg0&& arg0, Arg1&& arg1, Args&&... args) noexcept
      : TimeOfDayBase<Duration>(std::forward<Arg0>(arg0),
                                std::forward<Arg1>(arg1),
                                std::forward<Args>(args)...)
   {
   }
};

constexpr TimeOfDay<std::chrono::hours> make_time(const std::chrono::hours& h);

constexpr TimeOfDay<std::chrono::minutes> make_time(const std::chrono::hours& h,
                                                    const std::chrono::minutes& m);

constexpr TimeOfDay<std::chrono::seconds> make_time(const std::chrono::hours& h,
                                                    const std::chrono::minutes& m,
                                                    const std::chrono::seconds& s);

} // namespace orion

#include <orion/impl/Datetime.ipp>

//-------------------------------------------------------------------------------------------------
namespace orion
{

constexpr const Month jan{1u};
constexpr const Month feb{2u};
constexpr const Month mar{3u};
constexpr const Month apr{4u};
constexpr const Month may{5u};
constexpr const Month jun{6u};
constexpr const Month jul{7u};
constexpr const Month aug{8u};
constexpr const Month sep{9u};
constexpr const Month oct{10u};
constexpr const Month nov{11u};
constexpr const Month dec{12u};

constexpr const Month January{1u};
constexpr const Month Febuary{2u};
constexpr const Month March{3u};
constexpr const Month April{4u};
constexpr const Month May{5u};
constexpr const Month June{6u};
constexpr const Month July{7u};
constexpr const Month August{8u};
constexpr const Month September{9u};
constexpr const Month October{10u};
constexpr const Month November{11u};
constexpr const Month December{12u};

constexpr Weekday sun{0u};
constexpr Weekday mon{1u};
constexpr Weekday tue{2u};
constexpr Weekday wed{3u};
constexpr Weekday thu{4u};
constexpr Weekday fri{5u};
constexpr Weekday sat{6u};

constexpr Weekday Sunday{0u};
constexpr Weekday Monday{1u};
constexpr Weekday Tuesday{2u};
constexpr Weekday Wednesday{3u};
constexpr Weekday Thursday{4u};
constexpr Weekday Friday{5u};
constexpr Weekday Saturday{6u};

} // namespace orion

#endif // ORION_DATETIME_H