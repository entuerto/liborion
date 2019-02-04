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

#include <orion/Exception.h>

#include <fmt/format.h>

#include <iostream>
#include <vector>

namespace orion
{
// ------------------------------------------------------------------------------------------------
// VersionError 

/// Version error
///
/// Reports errors when a version is not valid.
///
class VersionError : public Exception
{
public:
   explicit VersionError(std::string text)
      : Exception(std::move(text))
   {
   }
   VersionError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

//--------------------------------------------------------------------------------------------------
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
   uint32_t major{0};
   uint32_t minor{0};
   uint32_t patch{0};

   std::vector<std::string> prerelease;
   std::vector<std::string> build;

   Version() = default;
   Version(uint32_t ma, uint32_t mi);
   Version(uint32_t ma, uint32_t mi, uint32_t pa);
   Version(uint32_t ma, uint32_t mi, uint32_t pa, std::vector<std::string> pre);
   Version(uint32_t ma,
           uint32_t mi,
           uint32_t pa,
           std::vector<std::string> pre,
           std::vector<std::string> b);
};

inline Version make_version(const std::string& value);

constexpr bool operator==(const Version& v1, const Version& v2) noexcept;
constexpr bool operator!=(const Version& v1, const Version& v2) noexcept;
constexpr bool operator<(const Version& v1, const Version& v2) noexcept;
constexpr bool operator>(const Version& v1, const Version& v2) noexcept;
constexpr bool operator<=(const Version& v1, const Version& v2) noexcept;
constexpr bool operator>=(const Version& v1, const Version& v2) noexcept;

inline std::string to_string(const Version& v);

inline std::istream& operator>>(std::istream& s, Version& v);

inline std::ostream& operator<<(std::ostream& s, const Version& v);


} // namespace orion

#include <orion/impl/Semver.ipp>

#endif // ORION_SEMVER_H 
