//
// Semver.ipp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_SEMVER_IPP
#define ORION_SEMVER_IPP

#include <orion/String.h>
#include <orion/Throw.h>

#include <regex>
#include <string>

using namespace std::string_literals;

namespace orion
{

static constexpr const char* digits = "0123456789";
static constexpr const char* alphas = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static constexpr const char* alphanum =
   "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

/// compares two prerelease Versions v1 and v2:
///  -1 == v1 is less than v2
///   0 == v1 is equal to v2
///   1 == v1 is greater than v2
inline int prerelease_compare(const std::string& v1, const std::string& v2) noexcept
{
   bool is_v1_all_num = v1.find_first_not_of(digits) == std::string::npos;
   bool is_v2_all_num = v2.find_first_not_of(digits) == std::string::npos;

   if (is_v1_all_num and not is_v2_all_num)
   {
      return -1;
   }
   else if (not is_v1_all_num and is_v2_all_num)
   {
      return 1;
   }
   else if (is_v1_all_num and is_v2_all_num)
   {
      auto v1_num = std::stoul(v1);
      auto v2_num = std::stoul(v2);
      return (v1_num == v2_num) ? 0 : (v1_num > v2_num) ? 1 : -1;
   }
   else
   {
      return v1.compare(v2);
   }

   return 0;
}

/// Compare compares two Versions
///  -1 == v1 is less than v2
///   0 == v1 is equal to v2
///   1 == v1 is greater than v2
inline int compare(const Version& v1, const Version& v2) noexcept
{
   if (v1.major != v2.major)
   {
      if (v1.major < v2.major)
         return -1;

      return 1;
   }

   if (v1.minor != v2.minor)
   {
      if (v1.minor < v2.minor)
         return -1;

      return 1;
   }

   if (v1.patch != v2.patch)
   {
      if (v1.patch < v2.patch)
         return -1;

      return 1;
   }

   // Quick comparison if a version has no prerelease versions
   if (v1.prerelease.empty() and v2.prerelease.empty())
   {
      return 0;
   }
   else if (v1.prerelease.empty() and not v2.prerelease.empty())
   {
      return 1;
   }
   else if (not v1.prerelease.empty() and v2.prerelease.empty())
   {
      return -1;
   }

   // Prerelease elements
   for (auto i = 0u; i < v1.prerelease.size() and i < v2.prerelease.size(); i++)
   {
      int res = prerelease_compare(v1.prerelease[i], v2.prerelease[i]);

      if (res != 0)
         return res;
   }

   // If all prerelease versions are equal but one has further prerelease
   // versions, then this one is greater.
   return (v1.prerelease.size() == v2.prerelease.size())
             ? 0
             : (v1.prerelease.size() < v2.prerelease.size()) ? -1 : 1;
}

inline bool operator==(const Version& v1, const Version& v2) noexcept
{
   return compare(v1, v2) == 0;
}

inline bool operator!=(const Version& v1, const Version& v2) noexcept
{
   return compare(v1, v2) != 0;
}

inline bool operator<(const Version& v1, const Version& v2) noexcept
{
   return compare(v1, v2) == -1;
}

inline bool operator>(const Version& v1, const Version& v2) noexcept
{
   return compare(v1, v2) == 1;
}

inline bool operator<=(const Version& v1, const Version& v2) noexcept
{
   return compare(v1, v2) <= 0;
}

inline bool operator>=(const Version& v1, const Version& v2) noexcept
{
   return compare(v1, v2) >= 0;
}

//-------------------------------------------------------------------------------------------------

inline std::string to_string(const Version& v)
{
   fmt::memory_buffer buffer;

   fmt::format_to(buffer, "{}.{}.{}", v.major, v.minor, v.patch);

   if (not v.prerelease.empty())
   {
      std::string text = join(v.prerelease, "."s);

      fmt::format_to(buffer, "-{}", text);
   }

   if (not v.build.empty())
   {
      std::string text = join(v.build, "."s);

      fmt::format_to(buffer, "+{}", text);
   }

   return fmt::to_string(buffer);
}

inline std::istream& operator>>(std::istream& s, Version& v)
{
   std::string tmp;

   s >> tmp;

   v = make_version(tmp);

   return s;
}

inline std::ostream& operator<<(std::ostream& s, const Version& v)
{
   s << v.major << "." << v.minor << "." << v.patch;

   if (not v.prerelease.empty())
   {
      std::string text = join(v.prerelease, "."s);

      s << "-" << text;
   }

   if (not v.build.empty())
   {
      std::string text = join(v.build, "."s);

      s << "+" << text;
   }

   return s;
}

//-------------------------------------------------------------------------------------------------

bool has_leading_zero(const std::string& text)
{
   return text.size() > 1 and text[0] == '0';
}

// Parse parses version string and returns a validated Version or error
Version parse_version(const std::string& value)
{
   throw_if<InvalidArgumentError>(value.empty(), "Empty value to parse", _src_loc);

   // Split into major.minor.(patch+pr+meta)
   std::vector<std::string> parts = split_max(by_char{'.'}, 3) | value;

   // Error when not 3 parts
   throw_if<VersionError>(parts.size() != 3, "No Major.Minor.Patch elements found", _src_loc);

   Version v;

   // Major
   throw_if<VersionError>(has_leading_zero(parts[0]), "Leading zero for major", _src_loc);

   try
   {
      auto num =  std::stol(parts[0]);

      if (num < 0)
         throw_exception<VersionError>("Invalid major number (< 0)", _src_loc);

      v.major = num;
   }
   catch (std::invalid_argument& ia)
   {
      throw_exception<VersionError>(ia.what(), _src_loc);
   }

   // Minor
   throw_if<VersionError>(has_leading_zero(parts[1]), "Leading zero for minor", _src_loc);

   try
   {
      auto num =  std::stol(parts[1]);

      if (num < 0)
         throw_exception<VersionError>("Invalid minor number (< 0)", _src_loc);

      v.minor = num;
   }
   catch (std::invalid_argument& ia)
   {
      throw_exception<VersionError>(ia.what(), _src_loc);
   }

   std::string build;
   std::string patch = std::move(parts[2]);

   // Extract build

   auto pos = patch.find('+');

   if (pos != std::string::npos)
   {
      std::vector<std::string> p = split(by_char{'+'}) | patch;

      patch = std::move(p[0]);
      build = std::move(p[1]);
   }

   // Extract prerelease

   std::string prerelease;

   pos = patch.find('-');

   if (pos != std::string::npos)
   {
      std::vector<std::string> p = split(by_char{'-'}) | patch;

      patch      = std::move(p[0]);
      prerelease = std::move(p[1]);
   }

   // Patch
   try
   {
      v.patch = std::stoul(patch);
   }
   catch (std::invalid_argument& ia)
   {
      throw_exception<VersionError>(ia.what(), _src_loc);
   }

   // Prerelease
   auto r1 = split(by_char{'.'}) | prerelease;
   v.prerelease.assign(r1.begin(), r1.end());

   // Validate prerelease numbers
   for (auto& item : v.prerelease)
   {
      if (item.find_first_not_of(alphanum) != std::string::npos)
         throw_exception<VersionError>("Invalid prerelease number", _src_loc);
   }

   // Build
   auto r2 = split(by_char{'.'}) | build;
   v.build.assign(r2.begin(), r2.end());

   // Validate build numbers
   for (auto& item : v.build)
   {
      if (item.find_first_not_of(alphanum) != std::string::npos)
         throw_exception<VersionError>("Invalid build number", _src_loc);
   }

   return v;
}

//-------------------------------------------------------------------------------------------------

inline Version make_version(const std::string& value)
{
   return parse_version(value);
}

} // namespace orion

//--------------------------------------------------------------------------------------------------
// Formatters for fmtlib

namespace fmt
{
template<>
struct formatter<orion::Version>
{
   template<typename ParseContext>
   constexpr auto parse(ParseContext& ctx)
   {
      return ctx.begin();
   }

   template<typename FormatContext>
   auto format(const orion::Version& v, FormatContext& ctx)
   {
      return fmt::format_to(ctx.begin(), "{}", orion::to_string(v));
   }
};
} // namespace fmt

#endif // ORION_SEMVER_IPP
