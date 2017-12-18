//
// Datetime.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_DATETIME_H
#define ORION_DATETIME_H

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <string>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace orion
{

///
/// Durations
///
using days = std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
using weeks = std::chrono::duration<int, std::ratio_multiply<std::ratio<7>, days::period>>;
using years = std::chrono::duration<int, std::ratio_multiply<std::ratio<146097, 400>, days::period>>;
using months = std::chrono::duration<int, std::ratio_divide<years::period, std::ratio<12>>>;

///
/// Year
///
// template<typename T = int16_t>
class API_EXPORT Year
{
public:
   // using value_type = T;
   // using value_type = int16_t;
   using value_type = int32_t;

   Year() = default;
   explicit constexpr Year(int y) noexcept;

   constexpr Year& operator++() noexcept;
   constexpr Year operator++(int) noexcept;
   constexpr Year& operator--() noexcept;
   constexpr Year operator--(int) noexcept;

   constexpr Year& operator+=(const years& y) noexcept;
   constexpr Year& operator-=(const years& y) noexcept;

   constexpr Year operator-() const noexcept;
   constexpr Year operator+() const noexcept;

   constexpr bool is_leap() const noexcept;

   constexpr explicit operator value_type() const noexcept;
   constexpr bool ok() const noexcept;

   static constexpr Year min() noexcept;
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

constexpr Year operator+(const Year& x, const years& y) noexcept;
constexpr Year operator+(const years& x, const Year& y) noexcept;
constexpr Year operator-(const Year& x, const years& y) noexcept;
constexpr years operator-(const Year& x, const Year& y) noexcept;

///
/// Month
///
// template<typename T = uint32_t>
class API_EXPORT Month
{
public:
   // using value_type = T;
   using value_type = uint32_t;

   Month() = default;
   explicit constexpr Month(value_type m) noexcept;

   constexpr Month& operator++() noexcept;
   constexpr Month operator++(int) noexcept;
   constexpr Month& operator--() noexcept;
   constexpr Month operator--(int) noexcept;

   constexpr Month& operator+=(const months& m) noexcept;
   constexpr Month& operator-=(const months& m) noexcept;

   constexpr explicit operator value_type() const noexcept;
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

constexpr Month operator+(const Month& lhs, const months& rhs) noexcept;
constexpr Month operator+(const months& lhs, const Month& rhs) noexcept;
constexpr Month operator-(const Month& lhs, const months& rhs) noexcept;
constexpr months operator-(const Month& lhs, const Month& rhs) noexcept;

///
/// Day
///
// template<typename T = uint8_t>
class API_EXPORT Day
{
public:
   // using value_type = T;
   using value_type = uint8_t;

   Day() = default;
   explicit constexpr Day(uint32_t d) noexcept;

   constexpr Day& operator++() noexcept;
   constexpr Day operator++(int) noexcept;
   constexpr Day& operator--() noexcept;
   constexpr Day operator--(int) noexcept;

   constexpr Day& operator+=(const days& d) noexcept;
   constexpr Day& operator-=(const days& d) noexcept;

   constexpr explicit operator value_type() const noexcept;
   constexpr explicit operator uint32_t() const noexcept;
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

constexpr Day operator+(const Day& lhs, const days& rhs) noexcept;
constexpr Day operator+(const days& lhs, const Day& rhs) noexcept;
constexpr Day operator-(const Day& lhs, const days& rhs) noexcept;
constexpr days operator-(const Day& lhs, const Day& rhs) noexcept;

// chrono-Compatible Low-Level Date Algorithms
// 2016 Howard Hinnant
// http://howardhinnant.github.io/date_algorithms.html

// Used to shift the serial date from 0 to 1970-01-01 instead of 0000-03-01.
constexpr int32_t unix_epoch = 719468;

template<class IntT>
constexpr IntT from_days(IntT y, unsigned m, unsigned d) noexcept;

template<class IntT>
constexpr std::tuple<IntT, unsigned, unsigned> to_days(IntT z) noexcept;

constexpr uint32_t last_day_of_month_common_year(uint32_t m) noexcept;

template<class IntT>
constexpr uint32_t last_day_of_month(IntT y, uint32_t m) noexcept;

template<class IntT>
constexpr uint32_t weekday_from_days(IntT z) noexcept;

constexpr uint32_t weekday_difference(uint32_t x, uint32_t y) noexcept;

constexpr uint32_t next_weekday(uint32_t wd) noexcept;

constexpr uint32_t prev_weekday(uint32_t wd) noexcept;

//---------------------------------------------------------------------------------------

constexpr std::pair<Year, Month> operator/(const Year& y, const Month& m) noexcept;

constexpr std::pair<Month, Day> operator/(const Month& m, const Day& d) noexcept;

constexpr std::pair<Day, Month> operator/(const Day& d, const Month& m) noexcept;

constexpr days operator/(const std::pair<Year, Month>& ym, const Day& d) noexcept;

constexpr days operator/(const std::pair<Month, Day>& md, const Year& y) noexcept;

constexpr days operator/(const std::pair<Day, Month>& dm, const Year& y) noexcept;

//---------------------------------------------------------------------------------------
//

template<class CharT, class Traits, class Rep, class Period>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                              const std::chrono::duration<Rep, Period>& d);

} // namespace orion

#include <orion/impl/Datetime.ipp>

//---------------------------------------------------------------------------------------
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

} // namespace orion

#endif // ORION_DATETIME_H