//
// string-example.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/String.h>

#include <functional>
#include <iostream>

using namespace orion;
using namespace std::string_literals;

void split_examples()
{
   std::string text{"one,two,three,four"};

   // std::vector<std::string> v = Split<std::string>{text, ByChar{','}};

   std::vector<std::string> v = Split<std::string>{by_char{','}} | text;

   for (auto& item : v)
   {
      std::cout << item << " -\n";
   }

   std::cout << "\n";

   // auto s = Split<std::string>{ByChar{','}} | text;
   auto s = split(by_char{','}) | text;

   for (auto& item : s)
   {
      std::cout << item << " *\n";
   }
}

void join_examples()
{
   std::vector<std::string> t{"bob", "john", "rob"};

   std::string r = join(std::begin(t), std::end(t), ", "s);

   std::cout << "Function join(begin, end):\n";
   std::cout << r << "\n\n";

   r = join(t, ", "s);

   std::cout << "Function join(range):\n";
   std::cout << r << "\n\n";

   r = Join{", "} | t;

   std::cout << "Pipe join | range:\n";
   std::cout << r << "\n\n";

   r = join({"1", "2", "3", "4"}, ", "s);

   std::cout << "Function join({list}):\n";
   std::cout << r << "\n\n";

   std::cout << "\n";

   std::vector<int> seq{1, 2, 3};

   std::cout << "Function join(int):\n";
   r = join(seq, ", "s);

   std::cout << r << "\n";
}

int main()
{
   split_examples();

   std::cout << "\n";

   join_examples();

   return EXIT_SUCCESS;
}
