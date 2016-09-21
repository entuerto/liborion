//  test-string.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/StringUtils.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::unittest;

//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(TestToLower)
{
   std::string str = "sdGWASgeWEF";
   std::string lower = to_lower(str);

   EXPECT(lower == "sdgwasgewef");
   EXPECT(lower == str);
}

TEST(TestToUpper)
{
   std::string str = "sdGWASgeWEF";
   std::string lower = to_upper(str);

   EXPECT(lower == "SDGWASGEWEF");
   EXPECT(lower == str);
}

TEST(TestEqualNoCase)
{
   std::string str1 = "sdGWASgeWEF";
   std::string str2 = "SDGWASGEWEF";

   EXPECT(str1 != str2);
   EXPECT(equals_no_case(str1, str2));
}

TEST(TestTrimLeft)
{
   std::string str1 = "   toto";

   EXPECT(trim_left(str1) == "toto");
}

TEST(TestTrimRight)
{
   std::string str1 = "toto   ";

   EXPECT(trim_right(str1) == "toto");
}

TEST(TestTrim)
{
   std::string str1 = "   toto   ";

   EXPECT(trim(str1) == "toto");
}

TEST(TestTokenize)
{
   std::string str1 = "This is a test; 4";

   std::vector<std::string> tokens;
   tokenize(str1, tokens, " ;");

   EXPECT(tokens.size() == 5);
   EXPECT(tokens[0] == "This");
   EXPECT(tokens[1] == "is");
   EXPECT(tokens[2] == "a");
   EXPECT(tokens[3] == "test");
   EXPECT(tokens[4] == "4");
}
