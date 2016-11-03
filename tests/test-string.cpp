//  test-string.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/StringUtils.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::unittest;

TEST_SUITE(OrionCore)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(String, ToLower)
{
   std::string str = "sdGWASgeWEF";
   std::string lower = to_lower(str);

   EXPECT_EQ(lower, "sdgwasgewef");
   EXPECT_EQ(lower, str);
}

TEST(String, ToUpper)
{
   std::string str = "sdGWASgeWEF";
   std::string lower = to_upper(str);

   EXPECT_EQ(lower, "SDGWASGEWEF"); 
   EXPECT_EQ(lower, str);
}

TEST(String, EqualNoCase)
{
   std::string str1 = "sdGWASgeWEF";
   std::string str2 = "SDGWASGEWEF";

   EXPECT_NE(str1, str2);
   EXPECT_TRUE(equals_no_case(str1, str2));
}

TEST(String, TrimLeft)
{
   std::string str1 = "   toto";

   EXPECT_EQ(trim_left(str1), "toto");
}

TEST(String, TrimRight)
{
   std::string str1 = "toto   ";

   EXPECT_EQ(trim_right(str1), "toto");
}

TEST(String, Trim)
{
   std::string str1 = "   toto   ";

   EXPECT_EQ(trim(str1), "toto");
}

TEST(String, Tokenize)
{
   std::string str1 = "This is a test; 4";

   std::vector<std::string> tokens;
   tokenize(str1, tokens, " ;");

   EXPECT_EQ(tokens.size(), 5);
   EXPECT_EQ(tokens[0], "This");
   EXPECT_EQ(tokens[1], "is");
   EXPECT_EQ(tokens[2], "a");
   EXPECT_EQ(tokens[3], "test");
   EXPECT_EQ(tokens[4], "4");
}

}
