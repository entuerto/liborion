//  test-encoding.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/fs/Path.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::fs;
using namespace orion::unittest;

TEST_SUITE(OrionFilesystem)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(Path, Construction)
{
   Path p("/toto");

   EXPECT_EQ(p.compare("/toto"), 0);
   EXPECT_EQ(p, "/toto");
}

TEST(Path, Join)
{
   Path p("/toto");

   p /= "tata";
   EXPECT_EQ(p.normalize(), Path("/toto/tata").normalize());

   p /= "/titi";
   EXPECT_EQ(p.normalize(), Path("/toto/tata/titi").normalize());

   p = Path("first");
   EXPECT_EQ(p.normalize(), Path("first").normalize());

   p /= "second";
   EXPECT_EQ(p.normalize(), Path("first/second").normalize()); 
}

TEST(Path, Decomposition)
{
   Path p("/foo");

   EXPECT_EQ(p.root_path(), "/");
   EXPECT_EQ(p.root_name(), "");
   EXPECT_EQ(p.root_directory(), "/");
   EXPECT_EQ(p.relative_path(), "foo");
   EXPECT_EQ(p.parent_path(), "/");
   //EXPECT(p.filename() == "foo");
   //EXPECT(p.stem() == "foo");
   //EXPECT(p.extension() == "");

   p = Path("c:");
   EXPECT_EQ(p.root_path(), "c:");
   EXPECT_EQ(p.root_name(), "c:");
   EXPECT_EQ(p.root_directory(), "");
   EXPECT_EQ(p.relative_path(), "");
   //EXPECT(p.parent_path() == "");
   //EXPECT(p.filename() == "c:");

   p = Path("c:/");
   EXPECT_EQ(p.root_path(), "c:/");
   EXPECT_EQ(p.root_name(), "c:");
   EXPECT_EQ(p.root_directory(), "/");
   EXPECT_EQ(p.relative_path(), "");
   //EXPECT(p.parent_path() == "c:");
   //EXPECT(p.filename() == "/");

   p = Path("\\\\net\\\\\\foo");
   EXPECT_EQ(p.root_path(), "\\\\net\\");
   EXPECT_EQ(p.root_name(), "\\\\net");
   EXPECT_EQ(p.root_directory(), "\\");
   EXPECT_EQ(p.relative_path(), "foo");
   //EXPECT(p.parent_path() == "\\\\net\\");
   //EXPECT(p.filename() == "foo");

   p = Path("//net/foo");
   EXPECT_EQ(p.root_path(), "//net/");
   EXPECT_EQ(p.root_name(), "//net");
   EXPECT_EQ(p.root_directory(), "/");
   EXPECT_EQ(p.relative_path(), "foo");
   //EXPECT(p.parent_path() == "//net/");
   //EXPECT(p.filename() == "foo");

   p = Path("//net");
   EXPECT_EQ(p.root_path(), "//net");
   EXPECT_EQ(p.root_name(), "//net");
   EXPECT_EQ(p.root_directory(), "");
   EXPECT_EQ(p.relative_path(), "");
   //EXPECT(p.parent_path() == "//net/");
   //EXPECT(p.filename() == "//net");

   p = Path("//net/");
   EXPECT_EQ(p.root_path(), "//net/");
   EXPECT_EQ(p.root_name(), "//net");
   EXPECT_EQ(p.root_directory(), "/");
   EXPECT_EQ(p.relative_path(), "");
   //EXPECT(p.parent_path() == "//net");
   //EXPECT(p.filename() == "/");
}

}
