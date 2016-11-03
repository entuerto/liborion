// utils.cpp
//
// Copyright 2014 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
//

#include <orion/fs/Utils.h>

#include <windows.h>

#include <orion/fs/Path.h>

namespace orion
{
namespace fs
{
///
/// Creates a hard link link with its target set to target
/// @param  target  path of the file or directory to link to
/// @param  link    path of the new hard link   
std::error_code create_hard_link(const Path& target, const Path& link)
{
   std::wstring wtarget = static_cast<std::wstring>(target);
   std::wstring wlink = static_cast<std::wstring>(link);

   if (::CreateHardLinkW(wtarget.c_str(), wlink.c_str(), 0) == 0)
      return std::error_code(::GetLastError(), std::system_category());

   return std::error_code();
}

///
/// Creates a symbolic link link with its target set to target
/// @param  target  path of the file or directory to link to
/// @param  link    path of the new symbolic link
std::error_code create_symlink(const Path& target, const Path& link)
{
   std::wstring wtarget = static_cast<std::wstring>(target);
   std::wstring wlink = static_cast<std::wstring>(link);

   // 0x0 The link target is a file.
   // 0x1 The link target is a directory.
   if (::CreateSymbolicLinkW(wlink.c_str(), wtarget.c_str(), 0x0) == 0)
      return std::error_code(::GetLastError(), std::system_category());

   return std::error_code();
}

///
/// Creates a symbolic link link with its target set to target
/// @param  target  path of the file or directory to link to
/// @param  link    path of the new symbolic link
std::error_code create_directory_symlink(const Path& target, const Path& link)
{
   std::wstring wtarget = static_cast<std::wstring>(target);
   std::wstring wlink = static_cast<std::wstring>(link);

   // 0x0 The link target is a file.
   // 0x1 The link target is a directory.
   if (::CreateSymbolicLinkW(wlink.c_str(), wtarget.c_str(), 0x1) == 0)
      return std::error_code(::GetLastError(), std::system_category());

   return std::error_code();
}

///
/// Creates the directory p as if by POSIX mkdir()
std::error_code create_directory(const Path& p)
{
   std::wstring name = static_cast<std::wstring>(p);

   if (::CreateDirectoryW(name.c_str(), 0) == 0)
      return std::error_code();

   if (::GetLastError() == ERROR_ALREADY_EXISTS and is_directory(p))
      return std::error_code();
   
   return std::error_code(::GetLastError(), std::system_category());
}

///
/// Executes create_directory() for every element of p that does not already exist.
std::error_code create_directories(const Path& /* p */)
{
   return std::error_code();
}

///
/// Returns the size of the regular file p, determined as if by reading the st_size member 
/// of the structure obtained by POSIX stat (symlinks are followed)
std::tuple<std::uintmax_t, std::error_code> file_size(const Path& p)
{
   WIN32_FILE_ATTRIBUTE_DATA fad;

   std::wstring name = static_cast<std::wstring>(p);

   if (::GetFileAttributesExW(name.c_str(), GetFileExInfoStandard, &fad) == 0)
      return std::make_tuple(0, std::error_code(::GetLastError(), std::system_category()));

   if ((fad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
      return std::make_tuple(0, std::error_code());

   uintmax_t size = (static_cast<uintmax_t>(fad.nFileSizeHigh) << 32) + fad.nFileSizeLow;

   return std::make_tuple(size, std::error_code());
}

///
/// Determines the information about the filesystem on which the pathname p is located.
std::tuple<SpaceInfo, std::error_code> space(const Path& p)
{
   ULARGE_INTEGER avail, total, free;

   SpaceInfo info{};

   std::wstring name = static_cast<std::wstring>(p);

   if (::GetDiskFreeSpaceExW(name.c_str(), &avail, &total, &free) == 0)
      return std::make_tuple(info, std::error_code(::GetLastError(), std::system_category()));

   info.capacity  = (static_cast<uintmax_t>(total.HighPart) << 32) + total.LowPart;
   info.free      = (static_cast<uintmax_t>(free.HighPart)  << 32) + free.LowPart;
   info.available = (static_cast<uintmax_t>(avail.HighPart) << 32) + avail.LowPart;
   
   return std::make_tuple(info, std::error_code());
}

/// checks whether the given path refers to block device 
bool is_block_file(const Path& p)
{
   return false;
}
  
/// checks whether the given path refers to a character device 
bool is_character_file(const Path& p)
{
   return false;
}

/// checks whether the given path refers to a directory 
bool is_directory(const Path& p)
{
   return false;
}

/// checks whether the given path refers to an empty file or directory
bool is_empty(const Path& p)
{
   return false;
}
 
/// checks whether the given path refers to a named pipe 
bool is_fifo(const Path& p)
{
   return false;
}

/// checks whether the argument refers to an other file 
bool is_other(const Path& p)
{
   return false;
}

/// checks whether the argument refers to a regular file 
bool is_regular_file(const Path& p)
{
   return false;
}

/// checks whether the argument refers to a named IPC socket 
bool is_socket(const Path& p)
{
   return false;
}
 
/// checks whether the argument refers to a symbolic link 
bool is_symlink(const Path& p)
{
   return false;
}

/// checks whether file status is known 
bool status_known(const Path& p)
{
   return false;
}

} // filesystem
} // orion
