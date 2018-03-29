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

TestSuite(OrionCore_String, Label{"String"})
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
void string_tolower(Test & t)
{
   std::string str   = "sdGWASgeWEF";
   std::string lower = to_lower(str);

   t.assert<eq>("sdgwasgewef"s, lower, _src_loc);
   t.assert<eq>(str, lower, _src_loc);
}

void string_toupper(Test & t)
{
   std::string str   = "sdGWASgeWEF";
   std::string upper = to_upper(str);

   t.assert<eq>("SDGWASGEWEF"s, upper, _src_loc);
   t.assert<eq>(str, upper, _src_loc);
}

void string_equal_nocase(Test & t)
{
   std::string str1 = "sdGWASgeWEF";
   std::string str2 = "SDGWASGEWEF";

   t.assert<ne>(str1, str2, _src_loc);
   t.assert<true>(iequals(str1, str2), _src_loc);
}

void string_trim_left(Test & t)
{
   std::string str1 = "   toto";

   t.assert<eq>("toto"s, trim_left(str1), _src_loc);
}

void string_trim_right(Test & t)
{
   std::string str1 = "toto   ";

   t.assert<eq>("toto"s, trim_right(str1), _src_loc);
}

void string_trim(Test & t)
{
   std::string str1 = "   toto   ";

   t.assert<eq>("toto"s, trim(str1), _src_loc);
}

void string_by_character(Test & t)
{
   std::string text{"one,two,three,four"};

   std::vector<std::string> items = split(by_char{','}) | text;

   t.assert<eq>(std::size_t(4), items.size(), _src_loc);
   t.assert<eq>("one"s, items[0], _src_loc);
   t.assert<eq>("two"s, items[1], _src_loc);
   t.assert<eq>("three"s, items[2], _src_loc);
   t.assert<eq>("four"s, items[3], _src_loc);
}

void string_by_character_no_match(Test & t)
{
   std::string text{"one,two,three,four"};

   std::vector<std::string> items = split(by_char{';'}) | text;

   t.assert<eq>(std::size_t(1), items.size(), _src_loc);
   t.assert<eq>(text, items[0], _src_loc);
}

void string_by_string(Test & t)
{
   std::string text{"one{s}two{s}three{s}four"};

   std::vector<std::string> items = split(by_string{"{s}"}) | text;

   t.assert<eq>(std::size_t(4), items.size(), _src_loc);
   t.assert<eq>("one"s, items[0], _src_loc);
   t.assert<eq>("two"s, items[1], _src_loc);
   t.assert<eq>("three"s, items[2], _src_loc);
   t.assert<eq>("four"s, items[3], _src_loc);
}

void string_by_string_no_match(Test & t)
{
   std::string text{"one{s}two{s}three{s}four"};

   std::vector<std::string> items = split(by_string{"|s|"}) | text;

   t.assert<eq>(std::size_t(1), items.size(), _src_loc);
   t.assert<eq>(text, items[0], _src_loc);
}

void string_tokenize(Test & t)
{
   std::string str1 = "This is a test; 4";

   std::vector<std::string> tokens = split(str1, any_of{" ;"});

   t.assert<eq>(std::size_t(5), tokens.size(), _src_loc);
   t.assert<eq>("This"s, tokens[0], _src_loc);
   t.assert<eq>("is"s, tokens[1], _src_loc);
   t.assert<eq>("a"s, tokens[2], _src_loc);
   t.assert<eq>("test"s, tokens[3], _src_loc);
   t.assert<eq>("4"s, tokens[4], _src_loc);
}

void string_tokenize_no_match(Test & t)
{
   std::string str1 = "This is a test; 4";

   std::vector<std::string> tokens = split(str1, any_of{"#&"});

   t.assert<eq>(std::size_t(1), tokens.size(), _src_loc);
   t.assert<eq>(str1, tokens[0], _src_loc);
}

void string_join_string(Test & t)
{
   std::vector<std::string> v{"bob", "john", "rob"};

   // Iterator
   std::string r = join(std::begin(v), std::end(v), ", "s);

   t.assert<eq>("bob, john, rob"s, r, _src_loc);

   // Range
   r = join(v, ", "s);

   t.assert<eq>("bob, john, rob"s, r, _src_loc);

   // Pipe
   r = Join{", "} | v;

   t.assert<eq>("bob, john, rob"s, r, _src_loc);

   // Initializer list
   r = join({"bob", "john", "rob"}, ", "s);

   t.assert<eq>("bob, john, rob"s, r, _src_loc);
}

void string_join_ints(Test & t)
{
   std::vector<int> v{1, 2, 3};

   // Iterator
   std::string r = join(std::begin(v), std::end(v), ", "s);

   t.assert<eq>("1, 2, 3"s, r, _src_loc);

   // Range
   r = join(v, ", "s);

   t.assert<eq>("1, 2, 3"s, r, _src_loc);

   // Pipe
   r = Join{", "} | v;

   t.assert<eq>("1, 2, 3"s, r, _src_loc);

   // Initializer list
   r = join({1, 2, 3}, ", "s);

   t.assert<eq>("1, 2, 3"s, r, _src_loc);
}

RegisterTestCase(OrionCore_String, string_tolower, Label{"To lower"});
RegisterTestCase(OrionCore_String, string_toupper, Label{"To upper"});
RegisterTestCase(OrionCore_String, string_equal_nocase, Label{"Equals no case"});
RegisterTestCase(OrionCore_String, string_trim_left, Label{"Trim left"});
RegisterTestCase(OrionCore_String, string_trim_right, Label{"Trim right"});
RegisterTestCase(OrionCore_String, string_trim, Label{"Trim"});
RegisterTestCase(OrionCore_String, string_by_character, Label{"Split by char"});
RegisterTestCase(OrionCore_String, string_by_character_no_match, Label{"Split by char no match"});
RegisterTestCase(OrionCore_String, string_by_string, Label{"Split by string"});
RegisterTestCase(OrionCore_String, string_by_string_no_match, Label{"Split by string no match"});
RegisterTestCase(OrionCore_String, string_tokenize, Label{"Split any of"});
RegisterTestCase(OrionCore_String, string_tokenize_no_match, Label{"Split any of no match"});
RegisterTestCase(OrionCore_String, string_join_string, Label{"Join string"});
RegisterTestCase(OrionCore_String, string_join_ints, Label{"Join integers to string"});
}
