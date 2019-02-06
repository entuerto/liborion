//
//  test-semver.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/Semver.h>
#include <orion/Test.h>

using namespace orion;
using namespace orion::unittest;

Section(OrionCore_Semver, Label{"Semver"})
{

TestCase("Basic version")
{
   Version v = make_version("1.2.3");

   check_eq(v.major, 1u);
   check_eq(v.minor, 2u);
   check_eq(v.patch, 3u);

   check_true(v.prerelease.empty());
   check_true(v.build.empty());

   Version v2 = make_version("0.0.1");

   check_eq(v2.major, 0u);
   check_eq(v2.minor, 0u);
   check_eq(v2.patch, 1u);

   check_true(v2.prerelease.empty());
   check_true(v2.build.empty());
}

TestCase("Version with big number")
{
   Version v = make_version("100.200.300");

   check_eq(v.major, 100u);
   check_eq(v.minor, 200u);
   check_eq(v.patch, 300u);

   check_true(v.prerelease.empty());
   check_true(v.build.empty());
}

TestCase("Version with prerelease")
{
   Version v = make_version("1.2.3-alpha.1");

   check_eq(v.major, 1u);
   check_eq(v.minor, 2u);
   check_eq(v.patch, 3u);

   check_true(v.prerelease.size() == 2);
   check_true(v.prerelease[0] == "alpha");
   check_true(v.prerelease[1] == "1");

   check_true(v.build.empty());
}

TestCase("Version with build")
{
   Version v = make_version("1.2.3+4100");

   check_eq(v.major, 1u);
   check_eq(v.minor, 2u);
   check_eq(v.patch, 3u);
   
   check_true(v.prerelease.empty());

   check_true(v.build.size() == 1);
   check_true(v.build[0] == "4100");

   v = make_version("1.2.3+123.456");

   check_eq(v.major, 1u);
   check_eq(v.minor, 2u);
   check_eq(v.patch, 3u);
   
   check_true(v.prerelease.empty());

   check_true(v.build.size() == 2);
   check_true(v.build[0] == "123");
   check_true(v.build[1] == "456");
}

TestCase("Complete version with prerelease and build")
{
   Version v = make_version("0.0.1-alpha.preview+123.456");

   check_eq(v.major, 0u);
   check_eq(v.minor, 0u);
   check_eq(v.patch, 1u);

   check_true(v.prerelease.size() == 2);
   check_true(v.prerelease[0] == "alpha");
   check_true(v.prerelease[1] == "preview");

   check_true(v.build.size() == 2);
   check_true(v.build[0] == "123");
   check_true(v.build[1] == "456");

   v = make_version("1.2.3-alpha.1+123.456");

   check_eq(v.major, 1u);
   check_eq(v.minor, 2u);
   check_eq(v.patch, 3u);

   check_true(v.prerelease.size() == 2);
   check_true(v.prerelease[0] == "alpha");
   check_true(v.prerelease[1] == "1");

   check_true(v.build.size() == 2);
   check_true(v.build[0] == "123");
   check_true(v.build[1] == "456");
}

TestCase("Compare version equals")
{
   std::vector<std::pair<Version, Version>> values{
      {Version{1, 0, 0}, Version{1, 0, 0}},
      {Version{0, 1, 0}, Version{0, 1, 0}},
      {Version{0, 0, 1}, Version{0, 0, 1}},
      {Version{1, 2, 3}, Version{1, 2, 3}}
   };

   for (auto& item : values)
   {
      check_eq(item.first, item.second);
   }
}

TestCase("Compare version Greater than")
{
   std::vector<std::pair<Version, Version>> values{
      {Version{2, 0, 0}, Version{1, 0, 0}},
      {Version{0, 2, 0}, Version{0, 1, 0}},
      {Version{0, 0, 2}, Version{0, 0, 1}},
      {Version{2, 2, 4}, Version{1, 2, 4}},
      {Version{1, 3, 3}, Version{1, 2, 3}},
      {Version{1, 2, 4}, Version{1, 2, 3}},
      {Version{1, 0, 0}, Version{1, 0, 0, {"alpha"}}}
   };

   for (auto& item : values)
   {
      check_gt(item.first, item.second);
   }
}

TestCase("Compare version Less than")
{
   std::vector<std::pair<Version, Version>> values{
      {Version{1, 0, 0}, Version{2, 0, 0}},
      {Version{2, 0, 0}, Version{2, 1, 0}},
      {Version{2, 1, 0}, Version{2, 1, 1}},
      {Version{1, 0, 0, {"alpha"}},             Version{1, 0, 0, {{"alpha"}, {"1"}}}},
      {Version{1, 0, 0, {{"alpha"}, {"1"}}},    Version{1, 0, 0, {{"alpha"}, {"beta"}}}},
      {Version{1, 0, 0, {{"alpha"}, {"beta"}}}, Version{1, 0, 0, {{"beta"}}}},
      {Version{1, 0, 0, {{"beta"}}},            Version{1, 0, 0, {{"beta"},  {"2"}}}},
      {Version{1, 0, 0, {{"beta"},  {"2"}}},    Version{1, 0, 0, {{"beta"},  {"11"}}}},
      {Version{1, 0, 0, {{"beta"},  {"11"}}},   Version{1, 0, 0, {{"rc"},    {"1"}}}},
      {Version{1, 0, 0, {{"rc"},    {"1"}}},      Version{1, 0, 0}}
   };

   for (auto& item : values)
   {
      check_lt(item.first, item.second);
   }
}

TestCase("With leading zeros")
{
   std::vector<std::string> values{
      {"01.1.1"},
      {"001.1.1"},
      {"1.01.1"},
      {"1.001.1"},
      {"1.1.01"},
      {"1.1.001"},
      {"1.1.1-01"},
      {"1.1.1-001"},
      {"1.1.1-beta.01"},
      {"1.1.1-beta.001"}
   };

}

TestCase("Invalid version strings")
{
   std::vector<std::string> values{
      {""},
      {"."},
      {"1."},
      {".1"},
      {"a.b.c"},
      {"1.a.b"},
      {"1.1.a"},
      {"1.a.1"},
      {"a.1.1"},
      {".."},
      {"1.."},
      {"1.1."},
      {"1..1"},
      {"1.1.+123"},
      {"1.1.-beta"},
      {"-1.1.1"},
      {"1.-1.1"},
      {"1.1.-1"}
   };

   auto invoke_make = [](const std::string& text) {
      Version v = make_version(text);
   };

   for (auto& item : values)
      check_throws(invoke_make(item));
}

}
