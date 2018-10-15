//
// MapFile-win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/io/MapFile.h>

#include <orion/ErrorMacros.h>
#include <orion/Exception.h>
#include <orion/SourceLocation.h>
#include <orion/Throw.h>

#include <host/win32/Error.h>
#include <host/win32/String.h>

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace orion
{
namespace io
{

struct MapFileImpl
{
   HANDLE file_handle      = INVALID_HANDLE_VALUE;
   HANDLE file_mapping     = INVALID_HANDLE_VALUE;
   uint8_t* view_address   = nullptr; // Base byte* of mapped file
   std::size_t file_size   = 0;
   std::size_t mapped_size = 0;
};

static std::unique_ptr<MapFileImpl> make_impl(HANDLE file_handle,
                                              MapFlags flags,
                                              std::size_t bytes_to_map,
                                              uint64_t offset);

//--------------------------------------------------------------------------------------------------

MapFile::MapFile()
   : _impl(std::make_unique<MapFileImpl>())
{
}

MapFile::MapFile(MapFile&& rhs) noexcept
   : _impl(std::move(rhs._impl))
{
}

MapFile::MapFile(std::unique_ptr<MapFileImpl>&& priv)
   : _impl(std::move(priv))
{
}

MapFile::~MapFile() {}

MapFile& MapFile::operator=(MapFile&& rhs) noexcept
{
   _impl = std::move(rhs._impl);

   return *this;
}

std::size_t MapFile::size() const
{
   return impl()->file_size;
}

std::size_t MapFile::mapped_size() const
{
   return impl()->mapped_size;
}

uint8_t* MapFile::data()
{
   return impl()->view_address;
}

const uint8_t* MapFile::data() const
{
   return impl()->view_address;
}

uintptr_t MapFile::file_handle() const
{
   return reinterpret_cast<uintptr_t>(impl()->file_handle);
}

std::error_code MapFile::lock()
{
   if (not VirtualLock(impl()->view_address, impl()->mapped_size))
      return win32::make_error_code(GetLastError());

   return std::error_code();
}

std::error_code MapFile::unlock()
{
   if (not VirtualUnlock(impl()->view_address, impl()->mapped_size))
      return win32::make_error_code(GetLastError());

   return std::error_code();
}

std::error_code MapFile::flush()
{
   if (not FlushViewOfFile(impl()->view_address, impl()->mapped_size))
      return win32::make_error_code(GetLastError());

   return std::error_code();
}

std::error_code MapFile::unmap()
{
   if (not UnmapViewOfFile(impl()->view_address))
      return win32::make_error_code(GetLastError());

   CloseHandle(impl()->file_mapping);

   if (impl()->file_handle != INVALID_HANDLE_VALUE)
      CloseHandle(impl()->file_handle);

   return std::error_code();
}

//--------------------------------------------------------------------------------------------------

MapFile make_mapfile(const std::string& file_name,
                     MapFlags flags,
                     std::size_t bytes_to_map,
                     uint64_t offset)
{
   std::wstring fname = win32::utf8_to_wstring(file_name);

   return make_mapfile(fname, flags, bytes_to_map, offset);
}

MapFile make_mapfile(const std::wstring& file_name,
                     MapFlags flags,
                     std::size_t bytes_to_map,
                     uint64_t offset)
{
   HANDLE file_handle = CreateFileW(file_name.c_str(),
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    nullptr,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    nullptr);

   if (file_handle == INVALID_HANDLE_VALUE)
   {
      // Error
      throw std::system_error(std::make_error_code(std::errc::bad_file_descriptor));
   }

   std::unique_ptr<MapFileImpl> impl = make_impl(file_handle, flags, bytes_to_map, offset);

   impl->file_handle = file_handle;

   return MapFile{std::move(impl)};
}

MapFile make_mapfile(uintptr_t file_handle,
                     MapFlags flags,
                     std::size_t bytes_to_map,
                     uint64_t offset)
{
   std::unique_ptr<MapFileImpl> impl =
      make_impl(reinterpret_cast<HANDLE>(file_handle), flags, bytes_to_map, offset);

   return MapFile{std::move(impl)};
}

//--------------------------------------------------------------------------------------------------

std::unique_ptr<MapFileImpl> make_impl(HANDLE file_handle,
                                       MapFlags flags,
                                       std::size_t bytes_to_map,
                                       uint64_t offset)
{
   DWORD protect = PAGE_READONLY;
   DWORD access  = FILE_MAP_READ;

   switch (flags)
   {
      case MapFlags::ReadOnly:
         break;
      case MapFlags::ReadWrite:
         protect = PAGE_READWRITE;
         access  = FILE_MAP_WRITE;
         break;
      case MapFlags::Copy:
         protect = PAGE_WRITECOPY;
         access  = FILE_MAP_COPY;
         break;
      case MapFlags::Execute:
         break;
   }

   // Get file size
   LARGE_INTEGER win32_file_size;
   if (not GetFileSizeEx(file_handle, &win32_file_size))
   {
      // Error
      throw std::system_error(win32::make_error_code(GetLastError()));
      return {};
   }
   auto file_size = static_cast<std::size_t>(win32_file_size.QuadPart);

   if (offset > file_size)
   {
      // Error
      throw std::system_error(win32::make_error_code(GetLastError()));
   }

   HANDLE file_mapping = CreateFileMapping(file_handle, nullptr, protect, 0, 0, nullptr);
   if (not file_mapping)
   {
      // Error
      throw std::system_error(win32::make_error_code(GetLastError()));
   }

   if ((offset + bytes_to_map) > file_size)
      bytes_to_map = std::size_t(file_size - offset);

   auto offset_low  = static_cast<uint32_t>(offset & 0xFFFFFFFF);
   auto offset_high = static_cast<uint32_t>(offset >> 32u);

   // Get mapped view address
   auto view_address = static_cast<uint8_t*>(
      MapViewOfFile(file_mapping, access, offset_high, offset_low, bytes_to_map));
   if (view_address == nullptr)
   {
      CloseHandle(file_mapping);
      throw std::system_error(win32::make_error_code(GetLastError()));
   }

   // mapped_size = (bytes_to_map == 0) ? file_size : bytes_to_map;

   return std::make_unique<MapFileImpl>(
      MapFileImpl{file_handle,
                  file_mapping,
                  view_address,
                  file_size,
                  ((bytes_to_map == 0) ? file_size : bytes_to_map)});
}

} // namespace io
} // namespace orion
