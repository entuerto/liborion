//
// Semver.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_SEMVER_H
#define ORION_SEMVER_H

#include <orion/Orion-Stddefs.h>

#include <fmt/format.h>

#include <iostream>

namespace orion
{

///
/// Semantic Versioning
/// 
/// Given a version number Major.Minor.Patch, increment the:
/// 
/// - Major version when you make incompatible API changes,
/// - Minor version when you add functionality in a backwards-compatible manner, and
/// - Patch version when you make backwards-compatible bug fixes.
/// 
/// Additional labels for pre-release and build metadata are available as extensions to 
/// the Major.Minor.Patch format.
///
struct Version
{
   uint32_t major;
   uint32_t minor;
   uint32_t patch;

   std::vector<std::string> prerelease;
   std::vector<std::string> build;
};

inline Version make_version(const std::string& value);

bool operator==(const Version& v1, const Version& v2) noexcept;
bool operator!=(const Version& v1, const Version& v2) noexcept;
bool operator<(const Version& v1, const Version& v2) noexcept;
bool operator>(const Version& v1, const Version& v2) noexcept;
bool operator<=(const Version& v1, const Version& v2) noexcept;
bool operator>=(const Version& v1, const Version& v2) noexcept;

inline void format_arg(fmt::BasicFormatter<char>& f, const char*& /*fmt_str*/, const Version& v);

inline std::istream& operator>>(std::istream& s, Version& v);

inline std::ostream& operator<<(std::ostream& s, const Version& v);


} // namespace orion

#include <orion/impl/Semver.ipp>

#endif // ORION_SEMVER_H 
