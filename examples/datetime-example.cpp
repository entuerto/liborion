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
   auto today = std::chrono::duration_cast<Days>(now);

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
   Date date = Year{2017}/May/Day{17};

   std::cout << "Date\n";
   
   std::cout << "Year{2017}/May/Day{17} = " << date << '\n';

   date = Year{2017}/May/Last;

   std::cout << "Year{2017}/May/Last    = " << date << '\n';

   auto ym = Year{2018}/July;

   std::cout << "Year{2018}/July        = " << ym << '\n';

   auto md = June/Day{1};

   std::cout << "June/Day{1}            = " << md << '\n';

   md = June/Last;

   std::cout << "June/Last              = " << md << '\n';

   auto mwd = April/Monday[2];

   std::cout << "April/Monday[2]        = " << mwd << '\n';

   current_time();

   return EXIT_SUCCESS;
}
