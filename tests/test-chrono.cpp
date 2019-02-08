//
//  test-chrono.cpp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Test.h>

#include <orion/Chrono.h>
#include <orion/Datetime.h>
#include <orion/Log.h>

using namespace orion;
using namespace orion::unittest;

Section(OrionCore_Chrono, Label{"Chrono"})
{

TestCase("Day")
{
   check_false(Day{0}.ok());
   check_true(Day{1}.ok());
   check_true(Day{2}.ok());
   check_true(Day{3}.ok());
   check_true(Day{29}.ok());
   check_true(Day{30}.ok());
   check_true(Day{31}.ok());
   check_false(Day{32}.ok());

   auto d = Day{1};

   check_eq(++d, Day{2});
   check_eq(d++, Day{2});
   check_eq(d, Day{3});
   check_eq(d--, Day{3});
   check_eq(d, Day{2});
   check_eq(--d, Day{1});
   check_eq((d += Days{2}), Day{3});
   check_eq((d -= Days{2}), Day{1});
}

TestCase("Month")
{
   check_false(Month{0}.ok());
   check_true(Month{1}.ok());
   check_true(Month{2}.ok());
   check_true(Month{3}.ok());
   check_true(Month{4}.ok());
   check_true(Month{5}.ok());
   check_true(Month{6}.ok());
   check_true(Month{7}.ok());
   check_true(Month{8}.ok());
   check_true(Month{9}.ok());
   check_true(Month{10}.ok());
   check_true(Month{11}.ok());
   check_true(Month{12}.ok());
   check_false(Month{13}.ok());

   check_eq(jan, Month{1});
   check_eq(feb, Month{2});
   check_eq(mar, Month{3});
   check_eq(apr, Month{4});
   check_eq(may, Month{5});
   check_eq(jun, Month{6});
   check_eq(jul, Month{7});
   check_eq(aug, Month{8});
   check_eq(sep, Month{9});
   check_eq(oct, Month{10});
   check_eq(nov, Month{11});
   check_eq(dec, Month{12});

   check_false(jan != jan);
   check(jan != feb);
   check(feb != jan);

   check_false(jan < jan);
   check_true( jan < feb);
   check_false(feb < jan);

   check_true(jan <= jan);
   check_true(jan <= feb);
   check_false(feb <= jan);

   check_false(jan > jan);
   check_false(jan > feb);
   check_true(feb > jan);

   check_true(jan >= jan);
   check_false(jan >= feb);
   check_true(feb >= jan);

   check_eq(mar + Months{7}, oct);
   check_eq(mar + Months{27}, jun);
   check_eq(Months{7} + mar, oct);
   check_eq(Months{27} + mar, jun);

   check_eq(mar - Months{7}, aug);
   check_eq(mar - Months{27}, dec);

   check_eq(mar - feb, Months{1});
   check_eq(feb - mar, Months{11});

   auto m = dec;

   check_eq(++m, jan);
   check_eq(m++, jan);
   check_eq(m, feb);
   check_eq(m--, feb);
   check_eq(m, jan);
   check_eq(--m, dec);
   check_eq((m += Months{2}), feb);
   check_eq((m -= Months{2}), dec);
}

TestCase("Year")
{
   check_false(Year{2015}.is_leap());
   check_true(Year{2016}.is_leap());

   check_true(Year::min().ok());
   check_true(Year{2015}.ok());
   check_true(Year{2016}.ok());
   check_true(Year::max().ok());

   check_eq(Year{2015} - Year{2010}, Years{5});
   check_eq(Year{2015} - Years{5}, Year{2010});
   check_eq(Year{2015}, Years{5} + Year{2010});
   check_eq(Year{2015}, Year{2010} + Years{5});
}

TestCase("Weekday")
{
   check_true(Weekday{0u}.ok());
   check_true(Weekday{1u}.ok());
   check_true(Weekday{2u}.ok());
   check_true(Weekday{3u}.ok());
   check_true(Weekday{4u}.ok());
   check_true(Weekday{5u}.ok());
   check_true(Weekday{6u}.ok());
   check_false(Weekday{7u}.ok());
   check_false(Weekday{8u}.ok());

   check_eq(sun, Weekday{0u});
   check_eq(mon, Weekday{1u});
   check_eq(tue, Weekday{2u});
   check_eq(wed, Weekday{3u});
   check_eq(thu, Weekday{4u});
   check_eq(fri, Weekday{5u});
   check_eq(sat, Weekday{6u});

   check_false(sun != sun);
   check_true(sun != mon);
   check_true(mon != sun);

#if 0
   constexpr unsigned a[7][7] =
   {// -    Sun Mon Tue Wed Thu Fri Sat
    /*Sun*/ {0,  6,  5,  4,  3,  2,  1},
    /*Mon*/ {1,  0,  6,  5,  4,  3,  2},
    /*Tue*/ {2,  1,  0,  6,  5,  4,  3},
    /*Wed*/ {3,  2,  1,  0,  6,  5,  4},
    /*Thu*/ {4,  3,  2,  1,  0,  6,  5},
    /*Fri*/ {5,  4,  3,  2,  1,  0,  6},
    /*Sat*/ {6,  5,  4,  3,  2,  1,  0}
   };

   for (unsigned x = 0; x < 7; ++x)
   {
      for (unsigned y = 0; y < 7; ++y)
      {
         check_eq(Weekday{x} - Weekday{y}, Days{a[x][y]});
         check_eq(Weekday{x} - Days{a[x][y]}, Weekday{y});
         check_eq(Weekday{x}, Weekday{y} + Days{a[x][y]});
         check_eq(Weekday{x}, Days{a[x][y]} + Weekday{y});
      }
   }
   for (unsigned x = 0; x < 7; ++x)
   {
      for (int y = -21; y < 21; ++y)
      {
         Weekday wx{x};
         Days dy{y};
         wx += dy;
         Weekday wz = Weekday{x} + Days{y};

         check_true(wx.ok());
         check_true(wz.ok());
         check_eq(wx, wz);
         check_eq(wx - Weekday{x}, Days{y % 7 + (y % 7 < 0 ? 7 : 0)});
      }
   }
   for (unsigned x = 0; x < 7; ++x)
   {
      for (int y = -21; y < 21; ++y)
      {
         Weekday wx{x};
         Days dy{y};
         wx -= dy;

         check_eq(wx, Weekday{x} + Days{-y});
      }
   }
   for (unsigned x = 0; x < 7; ++x)
   {
      Weekday wx{x};
      check_eq(++wx - Weekday{x}, Days{1});
      check_eq(wx++ - Weekday{x}, Days{1});
      check_eq(wx - Weekday{x}, Days{2});
      check_eq(wx-- - Weekday{x}, Days{2});
      check_eq(wx - Weekday{x}, Days{1});
      check_eq(--wx - Weekday{x}, Days{0});
   }
#endif
}

TestCase("WeekdayIndexed")
{
   WeekdayIndexed wdi = sun[1];
   
   check_eq(wdi.weekday(), sun);
   check_eq(wdi.index(), 1u);

   check_true(wdi.ok());

   check_eq(wdi, (WeekdayIndexed{sun, 1}));
   check_ne(wdi, (WeekdayIndexed{sun, 2u}));
   check_ne(wdi, (WeekdayIndexed{mon, 1u}));
}

TestCase("WeekdayLast")
{
   constexpr WeekdayLast wdl = sun[Last];

   check_eq(wdl.weekday(), sun);

   check_true(wdl.ok());

   check(wdl == WeekdayLast{sun});
   check(wdl != WeekdayLast{mon});
}

TestCase("YearMonth")
{
   YearMonth ym1 = {Year{2015}, jun};

   check_eq(ym1.year(), Year{2015});
   check_eq(ym1.month(), jun);
   check_true(ym1.ok());

   YearMonth ym2 = {Year{2016}, may};

   check_eq(ym2.year(), Year{2016});
   check_eq(ym2.month(), may);
   check_true(ym2.ok());

   check_eq(ym1,  ym1);
   check_ne(ym1,  ym2);
   check_lt(ym1, ym2);
   check_le(ym1,  ym2);
   check_gt(ym2, ym1);
   check_ge(ym2,  ym2);

   check_eq(ym2 - ym1, Months{11});
   check_eq(ym1 - ym2, -Months{11});
}

TestCase("MonthDay")
{
   constexpr MonthDay md1 = {feb, Day{28}};
   constexpr MonthDay md2 = {mar, Day{1}};

   check_true(md1.ok());
   check_true(md2.ok());
   check_false((MonthDay{feb, Day{32}}.ok()));
   check_false((MonthDay{Month{0}, Day{1}}.ok()));

   check_eq(md1.month(), feb);
   check_eq(md1.day(), Day{28});
   check_eq(md2.month(), mar);
   check_eq(md2.day(), Day{1});
   check_eq(md1, md1);
   check_ne(md1, md2);
   check_lt(md1, md2);
}

TestCase("MonthWeekday")
{
   constexpr MonthWeekday mwd1 = {feb, sat[4]};
   constexpr MonthWeekday mwd2 = {mar, mon[1]};

   check_true(mwd1.ok());
   check_true(mwd2.ok());
   check_false((MonthWeekday{feb, sat[0]}.ok()));
   check_false((MonthWeekday{Month{0}, sun[1]}.ok()));

   check_eq(mwd1.month(), feb);
   check(mwd1.weekday_indexed() == sat[4]);
   check_eq(mwd2.month(), mar);
   check(mwd2.weekday_indexed() == mon[1]);
   check(mwd1 == mwd1);
   check(mwd1 != mwd2);
}

TestCase("Date")
{
   constexpr Date ymd1 = {Year{2015}, aug, Day{9}};

   check_true(ymd1.ok());

   check_eq(ymd1.year(), Year{2015});
   check_eq(ymd1.month(), aug);
   check_eq(ymd1.day(), Day{9});

   constexpr SystemDays dp = ymd1;

   check_eq(dp.time_since_epoch(), Days{16656});

   constexpr Date ymd2 = dp;

   check_eq(ymd1, ymd2);

   constexpr Date ymd3 = {Year{1969}, dec, Day{31}};

   check_true(ymd3.ok());

   check_eq(ymd3.year(), Year{1969});
   check_eq(ymd3.month(), dec);
   check_eq(ymd3.day(), Day{31});

   constexpr SystemDays dp3 = ymd3;

   check_eq(dp3.time_since_epoch(), Days{-1});

   constexpr Date ymd4 = dp3;

   check_eq(ymd3, ymd4);

   check_ne(ymd1, ymd3);
   check_gt(ymd1, ymd3);
   check_lt(ymd3, ymd1);
   check_ge(ymd1, ymd1);
   check_le(ymd3, ymd1);

   check_true((Year{2000}/feb/29).ok());
   check_false((Year{2000}/feb/30).ok());
   check_true((Year{2100}/feb/28).ok());
   check_false((Year{2100}/feb/29).ok());

   check_eq((SystemDays(Year{2100}/feb/28) + Days{1}), SystemDays(Year{2100}/mar/1));
   check_eq((SystemDays(Year{2000}/mar/1) - SystemDays(Year{2000}/feb/28)), Days{2});
   check_eq((SystemDays(Year{2100}/mar/1) - SystemDays(Year{2100}/feb/28)), Days{1});

   check_eq(jan/31/2015, jan/Last/2015);
   check_eq(feb/28/2015, feb/Last/2015);
   check_eq(mar/31/2015, mar/Last/2015);
   check_eq(apr/30/2015, apr/Last/2015);
   check_eq(may/31/2015, may/Last/2015);
   check_eq(jun/30/2015, jun/Last/2015);
   check_eq(jul/31/2015, jul/Last/2015);
   check_eq(aug/31/2015, aug/Last/2015);
   check_eq(sep/30/2015, sep/Last/2015);
   check_eq(oct/31/2015, oct/Last/2015);
   check_eq(nov/30/2015, nov/Last/2015);
   check_eq(dec/31/2015, dec/Last/2015);

   check_eq(jan/31/2016, jan/Last/2016);
   check_eq(feb/29/2016, feb/Last/2016);
   check_eq(mar/31/2016, mar/Last/2016);
   check_eq(apr/30/2016, apr/Last/2016);
   check_eq(may/31/2016, may/Last/2016);
   check_eq(jun/30/2016, jun/Last/2016);
   check_eq(jul/31/2016, jul/Last/2016);
   check_eq(aug/31/2016, aug/Last/2016);
   check_eq(sep/30/2016, sep/Last/2016);
   check_eq(oct/31/2016, oct/Last/2016);
   check_eq(nov/30/2016, nov/Last/2016);
   check_eq(dec/31/2016, dec/Last/2016);
}

TestCase("make_time - Nanoseconds")
{
   auto tod = make_time(std::chrono::nanoseconds{18429000000022});
        
   check_eq(tod.hours(), std::chrono::hours{5});
   check_eq(tod.minutes(), std::chrono::minutes{7});
   check_eq(tod.seconds(), std::chrono::seconds{9});
   check_eq(tod.subseconds(), std::chrono::nanoseconds{22});

   check_eq(tod.to_duration(), std::chrono::nanoseconds{18429000000022});

   tod = make_time(std::chrono::hours{5}, 
                   std::chrono::minutes{7}, 
                   std::chrono::seconds{9}, 
                   std::chrono::nanoseconds{22});

   check_eq(tod.hours(), std::chrono::hours{5});
   check_eq(tod.minutes(), std::chrono::minutes{7});
   check_eq(tod.seconds(), std::chrono::seconds{9});
   check_eq(tod.subseconds(), std::chrono::nanoseconds{22});

   check_eq(tod.to_duration(), std::chrono::nanoseconds{18429000000022});
}

TestCase("make_time - Microseconds")
{
   auto tod = make_time(std::chrono::microseconds{18429000022});
        
   check_eq(tod.hours(), std::chrono::hours{5});
   check_eq(tod.minutes(), std::chrono::minutes{7});
   check_eq(tod.seconds(), std::chrono::seconds{9});
   check_eq(tod.subseconds(), std::chrono::microseconds{22});

   check_eq(tod.to_duration(), std::chrono::microseconds{18429000022});

   tod = make_time(std::chrono::hours{5}, 
                   std::chrono::minutes{7}, 
                   std::chrono::seconds{9}, 
                   std::chrono::microseconds{22});

   check_eq(tod.hours(), std::chrono::hours{5});
   check_eq(tod.minutes(), std::chrono::minutes{7});
   check_eq(tod.seconds(), std::chrono::seconds{9});
   check_eq(tod.subseconds(), std::chrono::microseconds{22});

   check_eq(tod.to_duration(), std::chrono::microseconds{18429000022});
}

TestCase("make_time - Seconds")
{
   auto tod = make_time(std::chrono::seconds{18429});
   
   check_eq(tod.hours(), std::chrono::hours{5});
   check_eq(tod.minutes(), std::chrono::minutes{7});
   check_eq(tod.seconds(), std::chrono::seconds{9});

   check_eq(tod.to_duration(), std::chrono::seconds{18429});

   tod = make_time(std::chrono::hours{5}, 
                   std::chrono::minutes{7}, 
                   std::chrono::seconds{9});

   check_eq(tod.hours(), std::chrono::hours{5});
   check_eq(tod.minutes(), std::chrono::minutes{7});
   check_eq(tod.seconds(), std::chrono::seconds{9});

   check_eq(tod.to_duration(), std::chrono::seconds{18429});
}

TestCase("make_time - Minutes")
{
   auto tod = make_time(std::chrono::minutes{307});
   
   check_eq(tod.hours(), std::chrono::hours{5});
   check_eq(tod.minutes(), std::chrono::minutes{7});

   check_eq(tod.to_duration(), std::chrono::minutes{307});

   tod = make_time(std::chrono::hours{5}, std::chrono::minutes{7});

   check_eq(tod.hours(), std::chrono::hours{5});
   check_eq(tod.minutes(), std::chrono::minutes{7});

   check_eq(tod.to_duration(), std::chrono::minutes{307});
}

TestCase("make_time - Hours")
{
   auto tod = make_time(std::chrono::hours{5});

   check_eq(tod.hours(), std::chrono::hours{5});

   check_eq(tod.to_duration(), std::chrono::hours{5});
}

TestCase("TimeOfDay - Seconds")
{
   auto t1 = TimeOfDay<std::chrono::seconds>{std::chrono::hours{13} + 
                                             std::chrono::minutes{7} +
                                             std::chrono::seconds{5}};

   check_eq(t1.hours(), std::chrono::hours{13});
   check_eq(t1.minutes(), std::chrono::minutes{7});
   check_eq(t1.seconds(), std::chrono::seconds{5});

   auto t2 = t1;
   check_eq(t2.hours(), t1.hours());
   check_eq(t2.minutes(), t1.minutes());
   check_eq(t2.seconds(), t1.seconds());
   check_eq(t2.to_duration(), t1.to_duration());

   auto t3 = TimeOfDay<std::chrono::seconds>{std::chrono::hours{13}, 
                                             std::chrono::minutes{7},
                                             std::chrono::seconds{5}};
   check_eq(t1.to_duration(), t3.to_duration());                                             
}

TestCase("TimeOfDay - Minutes")
{
   auto t1 = TimeOfDay<std::chrono::minutes>{std::chrono::hours{13} + 
                                             std::chrono::minutes{7}};

   check_eq(t1.hours(), std::chrono::hours{13});
   check_eq(t1.minutes(), std::chrono::minutes{7});

   auto t2 = t1;
   check_eq(t2.hours(), t1.hours());
   check_eq(t2.minutes(), t1.minutes());
   check_eq(t2.to_duration(), t1.to_duration());
}

TestCase("TimeOfDay - Hours")
{
   auto t1 = TimeOfDay<std::chrono::hours>{std::chrono::hours{13}};

   check_eq(t1.hours(), std::chrono::hours{13});

   auto t2 = t1;
   check_eq(t2.hours(), t1.hours());
   check_eq(t2.to_duration(), t1.to_duration());
}

}
