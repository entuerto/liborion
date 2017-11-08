//
// Utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_FS_UTILS_H
#define ORION_FS_UTILS_H

#include <system_error>
#include <string>
#include <tuple>
#include <vector>

#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace fs
{
//---------------------------------------------------------------------------------------

class Path;   

//---------------------------------------------------------------------------------------

enum class FileType
{
   None,      /// indicates that the file status has not been evaluated yet, or an error 
              /// occurred when evaluating it
   NotFound,  /// indicates that the file was not found (this is not considered an error)
   Regular,   /// a regular file
   Directory, /// a directory
   Symlink,   /// a symbolic link
   Block,     /// a block special file
   Character, /// a character special file
   Fifo,      /// a FIFO (also known as pipe) file
   Socket,    /// a socket file
   Unknown,   /// an unknown file
};

//---------------------------------------------------------------------------------------

///
/// Creates the directory p as if by POSIX mkdir()
API_EXPORT std::error_code create_directory(const Path& p);

///
/// Executes create_directory() for every element of p that does not already exist.
API_EXPORT std::error_code create_directories(const Path& p);

///
/// Creates a hard link link with its target set to target
/// @param  target  path of the file or directory to link to
/// @param  link    path of the new hard link   
API_EXPORT std::error_code create_hard_link(const Path& target, const Path& link);

///
/// Creates a symbolic link link with its target set to target
/// @param  target  path of the file or directory to link to
/// @param  link    path of the new symbolic link
API_EXPORT std::error_code create_symlink(const Path& target, const Path& link);

///
/// Creates a symbolic link link with its target set to target
/// @param  target  path of the file or directory to link to
/// @param  link    path of the new symbolic link
API_EXPORT std::error_code create_directory_symlink(const Path& target, const Path& link);

///
/// Returns the size of the regular file p, determined as if by reading the st_size member 
/// of the structure obtained by POSIX stat (symlinks are followed)
API_EXPORT std::tuple<std::uintmax_t, std::error_code> file_size(const Path& p);

//---------------------------------------------------------------------------------------

///
/// Represents the filesystem information as determined by space
struct SpaceInfo 
{
   std::uintmax_t capacity;  /// Total size of the filesystem, in bytes.
   std::uintmax_t free;      /// Free space on the filesystem, in bytes.
   std::uintmax_t available; /// Free space available (may be equal or less than free).
};

///
/// Determines the information about the filesystem on which the pathname p is located.
API_EXPORT std::tuple<SpaceInfo, std::error_code> space(const Path& path); // POSIX statvfs

//---------------------------------------------------------------------------------------

//std::string path_join(const std::vector<std::string>& paths);

//std::string path_join(const std::string& value1, const std::string& value2);

//---------------------------------------------------------------------------------------

/// checks whether the given path refers to block device 
API_EXPORT bool is_block_file(const Path& p);
  
/// checks whether the given path refers to a character device 
API_EXPORT bool is_character_file(const Path& p);

/// checks whether the given path refers to a directory 
API_EXPORT bool is_directory(const Path& p);

/// checks whether the given path refers to an empty file or directory
API_EXPORT bool is_empty(const Path& p);
 
/// checks whether the given path refers to a named pipe 
API_EXPORT bool is_fifo(const Path& p);

/// checks whether the argument refers to an other file 
API_EXPORT bool is_other(const Path& p);

/// checks whether the argument refers to a regular file 
API_EXPORT bool is_regular_file(const Path& p);

/// checks whether the argument refers to a named IPC socket 
API_EXPORT bool is_socket(const Path& p);
 
/// checks whether the argument refers to a symbolic link 
API_EXPORT bool is_symlink(const Path& p);

/// checks whether file status is known 
API_EXPORT bool status_known(const Path& p);
 

} // filesystem
} // orion
#endif
