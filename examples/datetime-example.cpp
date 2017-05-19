//
// date-example.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//

#include <orion/Datetime.h>

#include <iomanip>
#include <iostream>
#include <tuple>

using namespace orion;

void current_time()
{
   auto utc_offset = std::chrono::hours(-4);

   // Get duration in local units
   auto now = std::chrono::system_clock::now().time_since_epoch() + utc_offset;

   // Get duration in days
   auto today = std::chrono::duration_cast<days>(now);

   // Subtract off days, leaving now containing time since local midnight
   now -= today;
   auto h = std::chrono::duration_cast<std::chrono::hours>(now);
   now -= h;
   auto m = std::chrono::duration_cast<std::chrono::minutes>(now);
   now -= m;
   auto s = std::chrono::duration_cast<std::chrono::seconds>(now);
   now -= s;
   auto us = std::chrono::duration_cast<std::chrono::microseconds>(now);

   std::cout.fill('0');
   std::cout << "Current time:  " << std::setw(2) << h.count() << ':' << std::setw(2) << m.count()
             << ':' << std::setw(2) << s.count() << '.' << std::setw(6) << us.count() << '\n';
}

int main()
{
   days date = Year{2017} / May / Day{17};

   int32_t year;
   uint32_t month;
   uint32_t day;

   // Convert days IntTo year/month/day
   std::tie(year, month, day) = to_days(date.count());

   std::cout.fill('0');
   std::cout << "Date is " << year << '-' << std::setw(2) << month << '-' << std::setw(2) << day
             << "\n";

   current_time();

   return EXIT_SUCCESS;
}
