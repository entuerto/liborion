//
//  test-string.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/String.h>
#include <orion/Test.h>

using namespace std::string_literals;

using namespace orion;
using namespace orion::unittest;
using namespace orion::unittest::option;

Section(OrionCore_String, Label{"String"})
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TestCase("To lower")
{
   std::string str   = "sdGWASgeWEF";
   std::string lower = to_lower(str);

   check_eq("sdgwasgewef"s, lower);
   check_eq(str, lower);
}

TestCase("To upper")
{
   std::string str   = "sdGWASgeWEF";
   std::string upper = to_upper(str);

   check_eq("SDGWASGEWEF"s, upper);
   check_eq(str, upper);
}

TestCase("Equals no case")
{
   std::string str1 = "sdGWASgeWEF";
   std::string str2 = "SDGWASGEWEF";

   check_ne(str1, str2, _src_loc);

   check_true(iequals(str1, str2));
}

TestCase("Trim left")
{
   std::string str1 = "   toto";

   check_eq("toto"s, trim_left(str1));
}

TestCase("Trim right")
{
   std::string str1 = "toto   ";

   check_eq("toto"s, trim_right(str1));
}

TestCase("Trim")
{
   std::string str1 = "   toto   ";

   check_eq("toto"s, trim(str1));
}

TestCase("Split by char")
{
   std::string text{"one,two,three,four"};

   std::vector<std::string> items = split(by_char{','}) | text;

   check_eq(std::size_t(4), items.size());
   check_eq("one"s, items[0]);
   check_eq("two"s, items[1]);
   check_eq("three"s, items[2]);
   check_eq("four"s, items[3]);
}

TestCase("Split by char no match")
{
   std::string text{"one,two,three,four"};

   std::vector<std::string> items = split(by_char{';'}) | text;

   check_eq(std::size_t(1), items.size());
   check_eq(text, items[0]);
}

TestCase("Split by string")
{
   std::string text{"one{s}two{s}three{s}four"};

   std::vector<std::string> items = split(by_string{"{s}"}) | text;

   check_eq(std::size_t(4), items.size());
   check_eq("one"s, items[0]);
   check_eq("two"s, items[1]);
   check_eq("three"s, items[2]);
   check_eq("four"s, items[3]);
}

TestCase("Split by string no match")
{
   std::string text{"one{s}two{s}three{s}four"};

   std::vector<std::string> items = split(by_string{"|s|"}) | text;

   check_eq(std::size_t(1), items.size());
   check_eq(text, items[0]);
}

TestCase("Split any of")
{
   std::string str1 = "This is a test; 4";

   std::vector<std::string> tokens = split(str1, any_of{" ;"});

   check_eq(std::size_t(5), tokens.size());
   check_eq("This"s, tokens[0]);
   check_eq("is"s, tokens[1]);
   check_eq("a"s, tokens[2]);
   check_eq("test"s, tokens[3]);
   check_eq("4"s, tokens[4]);
}

TestCase("Split any of no match")
{
   std::string str1 = "This is a test; 4";

   std::vector<std::string> tokens = split(str1, any_of{"#&"});

   check_eq(std::size_t(1), tokens.size());
   check_eq(str1, tokens[0]);
}

TestCase("Join string")
{
   std::vector<std::string> v{"bob", "john", "rob"};

   // Iterator
   std::string r = join(std::begin(v), std::end(v), ", "s);

   check_eq("bob, john, rob"s, r);

   // Range
   r = join(v, ", "s);

   check_eq("bob, john, rob"s, r);

   // Pipe
   r = Join{", "} | v;

   check_eq("bob, john, rob"s, r);

   // Initializer list
   r = join({"bob", "john", "rob"}, ", "s);

   check_eq("bob, john, rob"s, r);
}

TestCase("Join integers to string")
{
   std::vector<int> v{1, 2, 3};

   // Iterator
   std::string r = join(std::begin(v), std::end(v), ", "s);

   check_eq("1, 2, 3"s, r);

   // Range
   r = join(v, ", "s);

   check_eq("1, 2, 3"s, r);

   // Pipe
   r = Join{", "} | v;

   check_eq("1, 2, 3"s, r);

   // Initializer list
   r = join({1, 2, 3}, ", "s);

   check_eq("1, 2, 3"s, r);
}

}
