//
//  test-filesystem.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/fs/Path.h>
#include <orion/Test.h>

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

   t.assert<eq>(0, p.compare("/toto"), _src_loc);
   t.assert<eq>("/toto", p, _src_loc);
}

void path_join(Test& t)
{
   Path p("/toto");

   p /= "tata";
   t.assert<eq>(Path("/toto/tata").normalize(), p.normalize(), _src_loc);

   p /= "/titi";
   t.assert<eq>(Path("/toto/tata/titi").normalize(), p.normalize(), _src_loc);

   p = Path("first");
   t.assert<eq>(Path("first").normalize(), p.normalize(), _src_loc);

   p /= "second";
   t.assert<eq>(Path("first/second").normalize(), p.normalize(), _src_loc);
}

void path_decomposition(Test& t)
{
   Path p("/foo");

   t.assert<eq>("/", p.root_path(), _src_loc);
   t.assert<eq>("", p.root_name(), _src_loc);
   t.assert<eq>("/", p.root_directory(), _src_loc);
   t.assert<eq>("foo", p.relative_path(), _src_loc);
   t.assert<eq>("/", p.parent_path(), _src_loc);
   //t.assert<eq>("foo", p.filename(), _src_loc);
   //t.assert<eq>("foo", p.stem(), _src_loc);
   //t.assert<eq>("", p.extension(), _src_loc);

   p = Path("c:");
   t.assert<eq>("c:", p.root_path(), _src_loc);
   t.assert<eq>("c:", p.root_name(), _src_loc);
   t.assert<eq>("", p.root_directory(), _src_loc);
   t.assert<eq>("", p.relative_path(), _src_loc);
   //t.assert<eq>("", p.parent_path(), _src_loc);
   //t.assert<eq>("c:", p.filename(), _src_loc);
   //t.assert<eq>("", p.stem(), _src_loc);
   //t.assert<eq>("", p.extension(), _src_loc);

   p = Path("c:/");
   t.assert<eq>("c:/", p.root_path(), _src_loc);
   t.assert<eq>("c:", p.root_name(), _src_loc);
   t.assert<eq>("", p.root_directory(), _src_loc);
   t.assert<eq>("", p.relative_path(), _src_loc);
   //t.assert<eq>("c:", p.parent_path(), _src_loc);
   //t.assert<eq>("/", p.filename(), _src_loc);
   //t.assert<eq>("", p.stem(), _src_loc);
   //t.assert<eq>("", p.extension(), _src_loc);
   
   p = Path("\\\\net\\\\\\foo");
   t.assert<eq>("\\\\net\\", p.root_path(), _src_loc);
   t.assert<eq>("\\\\net", p.root_name(), _src_loc);
   t.assert<eq>("\\", p.root_directory(), _src_loc);
   t.assert<eq>("foo", p.relative_path(), _src_loc);
   //t.assert<eq>("\\\\net\\", p.parent_path(), _src_loc);
   //t.assert<eq>("foo", p.filename(), _src_loc);
   //t.assert<eq>("", p.stem(), _src_loc);
   //t.assert<eq>("", p.extension(), _src_loc);

   p = Path("//net/foo");
   t.assert<eq>("//net/", p.root_path(), _src_loc);
   t.assert<eq>("//net", p.root_name(), _src_loc);
   t.assert<eq>("/", p.root_directory(), _src_loc);
   t.assert<eq>("foo", p.relative_path(), _src_loc);
   //t.assert<eq>("//net/", p.parent_path(), _src_loc);
   //t.assert<eq>("foo", p.filename(), _src_loc);
   //t.assert<eq>("", p.stem(), _src_loc);
   //t.assert<eq>("", p.extension(), _src_loc);

   p = Path("//net");
   t.assert<eq>("//net", p.root_path(), _src_loc);
   t.assert<eq>("//net", p.root_name(), _src_loc);
   t.assert<eq>("", p.root_directory(), _src_loc);
   t.assert<eq>("", p.relative_path(), _src_loc);
   //t.assert<eq>("//net/", p.parent_path(), _src_loc);
   //t.assert<eq>("//net", p.filename(), _src_loc);
   //t.assert<eq>("", p.stem(), _src_loc);
   //t.assert<eq>("", p.extension(), _src_loc);

   p = Path("//net/");
   t.assert<eq>("//net/", p.root_path(), _src_loc);
   t.assert<eq>("//net", p.root_name(), _src_loc);
   t.assert<eq>("/", p.root_directory(), _src_loc);
   t.assert<eq>("", p.relative_path(), _src_loc);
   //t.assert<eq>("//net", p.parent_path(), _src_loc);
   //t.assert<eq>("/", p.filename(), _src_loc);
   //t.assert<eq>("", p.stem(), _src_loc);
   //t.assert<eq>("", p.extension(), _src_loc);
}

RegisterTestCase(OrionFilesystem, path_construction);
RegisterTestCase(OrionFilesystem, path_join);
RegisterTestCase(OrionFilesystem, path_decomposition);
}
