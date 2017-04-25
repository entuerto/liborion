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

TestSuite(OrionFilesystem)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
void path_construction(Test& t)
{
   Path p("/toto");

   t.assert<std::equal_to<>>(0, p.compare("/toto"), _src_loc);
   t.assert<std::equal_to<>>("/toto", p, _src_loc);
}

void path_join(Test& t)
{
   Path p("/toto");

   p /= "tata";
   t.assert<std::equal_to<>>(Path("/toto/tata").normalize(), p.normalize(), _src_loc);

   p /= "/titi";
   t.assert<std::equal_to<>>(Path("/toto/tata/titi").normalize(), p.normalize(), _src_loc);

   p = Path("first");
   t.assert<std::equal_to<>>(Path("first").normalize(), p.normalize(), _src_loc);

   p /= "second";
   t.assert<std::equal_to<>>(Path("first/second").normalize(), p.normalize(), _src_loc);
}

void path_decomposition(Test& t)
{
   Path p("/foo");

   t.assert<std::equal_to<>>("/", p.root_path(), _src_loc);
   t.assert<std::equal_to<>>("", p.root_name(), _src_loc);
   t.assert<std::equal_to<>>("/", p.root_directory(), _src_loc);
   t.assert<std::equal_to<>>("foo", p.relative_path(), _src_loc);
   t.assert<std::equal_to<>>("/", p.parent_path(), _src_loc);
   //t.assert<std::equal_to<>>("foo", p.filename(), _src_loc);
   //t.assert<std::equal_to<>>("foo", p.stem(), _src_loc);
   //t.assert<std::equal_to<>>("", p.extension(), _src_loc);

   p = Path("c:");
   t.assert<std::equal_to<>>("c:", p.root_path(), _src_loc);
   t.assert<std::equal_to<>>("c:", p.root_name(), _src_loc);
   t.assert<std::equal_to<>>("", p.root_directory(), _src_loc);
   t.assert<std::equal_to<>>("", p.relative_path(), _src_loc);
   //t.assert<std::equal_to<>>("", p.parent_path(), _src_loc);
   //t.assert<std::equal_to<>>("c:", p.filename(), _src_loc);
   //t.assert<std::equal_to<>>("", p.stem(), _src_loc);
   //t.assert<std::equal_to<>>("", p.extension(), _src_loc);

   p = Path("c:/");
   t.assert<std::equal_to<>>("c:/", p.root_path(), _src_loc);
   t.assert<std::equal_to<>>("c:", p.root_name(), _src_loc);
   t.assert<std::equal_to<>>("", p.root_directory(), _src_loc);
   t.assert<std::equal_to<>>("", p.relative_path(), _src_loc);
   //t.assert<std::equal_to<>>("c:", p.parent_path(), _src_loc);
   //t.assert<std::equal_to<>>("/", p.filename(), _src_loc);
   //t.assert<std::equal_to<>>("", p.stem(), _src_loc);
   //t.assert<std::equal_to<>>("", p.extension(), _src_loc);
   
   p = Path("\\\\net\\\\\\foo");
   t.assert<std::equal_to<>>("\\\\net\\", p.root_path(), _src_loc);
   t.assert<std::equal_to<>>("\\\\net", p.root_name(), _src_loc);
   t.assert<std::equal_to<>>("\\", p.root_directory(), _src_loc);
   t.assert<std::equal_to<>>("foo", p.relative_path(), _src_loc);
   //t.assert<std::equal_to<>>("\\\\net\\", p.parent_path(), _src_loc);
   //t.assert<std::equal_to<>>("foo", p.filename(), _src_loc);
   //t.assert<std::equal_to<>>("", p.stem(), _src_loc);
   //t.assert<std::equal_to<>>("", p.extension(), _src_loc);

   p = Path("//net/foo");
   t.assert<std::equal_to<>>("//net/", p.root_path(), _src_loc);
   t.assert<std::equal_to<>>("//net", p.root_name(), _src_loc);
   t.assert<std::equal_to<>>("/", p.root_directory(), _src_loc);
   t.assert<std::equal_to<>>("foo", p.relative_path(), _src_loc);
   //t.assert<std::equal_to<>>("//net/", p.parent_path(), _src_loc);
   //t.assert<std::equal_to<>>("foo", p.filename(), _src_loc);
   //t.assert<std::equal_to<>>("", p.stem(), _src_loc);
   //t.assert<std::equal_to<>>("", p.extension(), _src_loc);

   p = Path("//net");
   t.assert<std::equal_to<>>("//net", p.root_path(), _src_loc);
   t.assert<std::equal_to<>>("//net", p.root_name(), _src_loc);
   t.assert<std::equal_to<>>("", p.root_directory(), _src_loc);
   t.assert<std::equal_to<>>("", p.relative_path(), _src_loc);
   //t.assert<std::equal_to<>>("//net/", p.parent_path(), _src_loc);
   //t.assert<std::equal_to<>>("//net", p.filename(), _src_loc);
   //t.assert<std::equal_to<>>("", p.stem(), _src_loc);
   //t.assert<std::equal_to<>>("", p.extension(), _src_loc);

   p = Path("//net/");
   t.assert<std::equal_to<>>("//net/", p.root_path(), _src_loc);
   t.assert<std::equal_to<>>("//net", p.root_name(), _src_loc);
   t.assert<std::equal_to<>>("/", p.root_directory(), _src_loc);
   t.assert<std::equal_to<>>("", p.relative_path(), _src_loc);
   //t.assert<std::equal_to<>>("//net", p.parent_path(), _src_loc);
   //t.assert<std::equal_to<>>("/", p.filename(), _src_loc);
   //t.assert<std::equal_to<>>("", p.stem(), _src_loc);
   //t.assert<std::equal_to<>>("", p.extension(), _src_loc);
}

RegisterTestCase(OrionFilesystem, path_construction);
RegisterTestCase(OrionFilesystem, path_join);
RegisterTestCase(OrionFilesystem, path_decomposition);
}
