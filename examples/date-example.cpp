//
// date-example.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//

#include <orion/Chrono.h>
#include <orion/Datetime.h>

#include <iostream>

using namespace orion;
using namespace std::chrono;

int main()
{
   auto now = system_clock::now();

   std::cout << "\n"
             << "ANSIC:       " << to_string(now, ANSIC) << "\n"
             << "UnixDate:    " << to_string(now, UnixDate) << "\n"
             << "RFC822:      " << to_string(now, RFC822) << "\n"
             << "RFC822Z:     " << to_string(now, RFC822Z) << "\n"
             << "RFC850:      " << to_string(now, RFC850) << "\n"
             << "RFC1123:     " << to_string(now, RFC1123) << "\n"
             << "RFC1123Z:    " << to_string(now, RFC1123Z) << "\n"
             << "RFC3339:     " << to_string(now, RFC3339) << "\n"
             << "ISO8601:     " << to_string(now, ISO8601) << "\n\n";

   auto y   = duration_cast<years>(now.time_since_epoch());
   auto m   = duration_cast<months>(now.time_since_epoch());
   auto d   = duration_cast<days>(now.time_since_epoch());
   auto w   = duration_cast<weeks>(now.time_since_epoch());
   auto h   = duration_cast<hours>(now.time_since_epoch());
   auto min = duration_cast<minutes>(now.time_since_epoch());
   auto s   = duration_cast<seconds>(now.time_since_epoch());
   auto ms  = duration_cast<milliseconds>(now.time_since_epoch());
   auto us  = duration_cast<microseconds>(now.time_since_epoch());
   auto ns  = duration_cast<nanoseconds>(now.time_since_epoch());

   duration<double> o = now.time_since_epoch() - y;

   std::cout << "\nTime Since Epoch (01/01/1970)\n"
             << "\nyears:    " << y << "\nmonths:   " << m << "\ndays:     " << d
             << "\nweeks:    " << w << "\nhours:    " << h << "\nmins:     " << min
             << "\nMillis:   " << ms << "\nMicro:    " << us << "\nNano:     " << ns << "\n"
             << "\nother:    " << o << "\n\n";

   return EXIT_SUCCESS;
}
