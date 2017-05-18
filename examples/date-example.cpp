//
// date-example.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//

#include <iostream>

#include <orion/Datetime.h>
#include <orion/DateUtils.h>

using namespace orion;

int main()
{
   auto now = std::chrono::system_clock::now();

   std::cout << "\n"
             << "ANSIC:       " << to_string(now, ANSIC)
             << "\n"
             << "UnixDate:    " << to_string(now, UnixDate)
             << "\n"
             << "RFC822:      " << to_string(now, RFC822)
             << "\n"
             << "RFC822Z:     " << to_string(now, RFC822Z)
             << "\n"
             << "RFC850:      " << to_string(now, RFC850)
             << "\n"
             << "RFC1123:     " << to_string(now, RFC1123)
             << "\n"
             << "RFC1123Z:    " << to_string(now, RFC1123Z)
             << "\n"
             << "RFC3339:     " << to_string(now, RFC3339)
             << "\n"
             << "ISO8601:     " << to_string(now, ISO8601)
             << "\n\n";

   auto y = std::chrono::duration_cast<years>(now.time_since_epoch());
   auto m = std::chrono::duration_cast<months>(now.time_since_epoch());
   auto d = std::chrono::duration_cast<days>(now.time_since_epoch());
   auto w = std::chrono::duration_cast<weeks>(now.time_since_epoch());
   auto h = std::chrono::duration_cast<std::chrono::hours>(now.time_since_epoch());
   auto min = std::chrono::duration_cast<std::chrono::minutes>(now.time_since_epoch());
   auto s = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
   auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
   auto us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
   auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());


   std::chrono::duration<double> o = now.time_since_epoch() - y;

   std::cout << "\nTime Since Epoch (01/01/1970)\n"
             << "\nyears:    " << y
             << "\nmonths:   " << m
             << "\ndays:     " << d
             << "\nweeks:    " << w
             << "\nhours:    " << h
             << "\nmins:     " << min
             << "\nMillis:   " << ms
             << "\nMicro:    " << us
             << "\nNano:     " << ns
             << "\n"
             << "\nother:    " << o
             << "\n\n";

   return EXIT_SUCCESS;
}
