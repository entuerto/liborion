//
//  test-string.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/StringUtils.h>
#include <orion/Test.h>

using namespace std::string_literals;

using namespace orion;
using namespace orion::unittest;

TestSuite(OrionCore_String)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
void string_tolower(Test& t)
{
   std::string str = "sdGWASgeWEF";
   std::string lower = to_lower(str);

   t.assert<std::equal_to<>>("sdgwasgewef"s, lower, _src_loc);
   t.assert<std::equal_to<>>(str, lower, _src_loc);
}

void string_toupper(Test& t)
{
   std::string str = "sdGWASgeWEF";
   std::string upper = to_upper(str);

   t.assert<std::equal_to<>>("SDGWASGEWEF"s, upper, _src_loc);
   t.assert<std::equal_to<>>(str, upper, _src_loc);
}

void string_equal_nocase(Test& t)
{
   std::string str1 = "sdGWASgeWEF";
   std::string str2 = "SDGWASGEWEF";

   t.assert<std::not_equal_to<>>(str1, str2, _src_loc);
   //t.assert<equals_no_case>(str1, str2, _src_loc);
}

void string_trim_left(Test& t)
{
   std::string str1 = "   toto";

   t.assert<std::equal_to<>>("toto"s, trim_left(str1), _src_loc);
}

void string_trim_right(Test& t)
{
   std::string str1 = "toto   ";

   t.assert<std::equal_to<>>("toto"s, trim_right(str1), _src_loc);
}

void string_trim(Test& t)
{
   std::string str1 = "   toto   ";

   t.assert<std::equal_to<>>("toto"s, trim(str1), _src_loc);
}

void string_tokenize(Test& t)
{
   std::string str1 = "This is a test; 4";

   std::vector<std::string> tokens;
   tokenize(str1, tokens, " ;");

   t.assert<std::equal_to<>>(std::size_t(5), tokens.size(), _src_loc);
   t.assert<std::equal_to<>>("This"s, tokens[0], _src_loc);
   t.assert<std::equal_to<>>("is"s,   tokens[1], _src_loc);
   t.assert<std::equal_to<>>("a"s,    tokens[2], _src_loc);
   t.assert<std::equal_to<>>("test"s, tokens[3], _src_loc);
   t.assert<std::equal_to<>>("4"s,    tokens[4], _src_loc);
}

RegisterTestCase(OrionCore_String, string_tolower);
RegisterTestCase(OrionCore_String, string_toupper);
RegisterTestCase(OrionCore_String, string_equal_nocase);
RegisterTestCase(OrionCore_String, string_trim_left);
RegisterTestCase(OrionCore_String, string_trim_right);
RegisterTestCase(OrionCore_String, string_trim);
RegisterTestCase(OrionCore_String, string_tokenize);
}
