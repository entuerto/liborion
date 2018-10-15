//
// MapFile.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_IO_MAPFILE_H
#define ORION_IO_MAPFILE_H

#include <orion/Orion-Stddefs.h>

#include <memory>
#include <system_error>

namespace orion
{
namespace io
{
struct MapFileImpl;

enum class MapFlags : uint8_t
{
   ReadOnly  = 0x02, // ReadOnly maps the memory read-only.
   ReadWrite = 0x04, // ReadWrite maps the memory as read-write. Writes to the Map object
                     // will update the underlying file.
   Copy = 0x08,      // Copy maps the memory as copy-on-write. Writes to the Map object will affect
                     // memory, but the underlying file will remain unchanged.
   Execute = 0x10    // If Execute is set, the mapped memory is marked as executable.
};

///
/// MapFile allows mapping files into memory. It tries to provide a simple, reasonably
/// portable interface, but doesn't abstract away every little
/// platform detail.
///
class API_EXPORT MapFile
{
public:
   MapFile();
   ~MapFile();

   explicit MapFile(std::unique_ptr<MapFileImpl>&& priv);

   // Move
   MapFile(MapFile&& rhs) noexcept;
   MapFile& operator=(MapFile&& rhs) noexcept;

   // No copy
   MapFile(const MapFile&) = delete;
   MapFile& operator=(const MapFile&) = delete;

   /// File size
   std::size_t size() const;

   /// Size of actually mapped bytes
   std::size_t mapped_size() const;

   uint8_t* data();

   const uint8_t* data() const;

   uintptr_t file_handle() const;

   /// Lock keeps the mapped region in physical memory, ensuring that it will not be
   /// swapped out.
   std::error_code lock();

   /// Unlock reverses the effect of lock, allowing the mapped region to potentially
   /// be swapped out.
   std::error_code unlock();

   /// Flush synchronizes the mapping's contents to the file's contents on disk.
   std::error_code flush();

   /// Unmap deletes the memory mapped region, flushes any remaining changes.
   std::error_code unmap();

private:
   const MapFileImpl* impl() const { return _impl.get(); }
   MapFileImpl* impl() { return _impl.get(); }

   std::unique_ptr<MapFileImpl> _impl;
};

MapFile make_mapfile(const std::string& file_name,
                     MapFlags flags,
                     std::size_t bytes_to_map = 0, // 0 = Whole file
                     uint64_t offset          = 0);

MapFile make_mapfile(const std::wstring& file_name,
                     MapFlags flags,
                     std::size_t bytes_to_map = 0, // 0 = Whole file
                     uint64_t offset          = 0);

MapFile make_mapfile(uintptr_t file_handle,
                     MapFlags flags,
                     std::size_t bytes_to_map = 0, // 0 = Whole file
                     uint64_t offset          = 0);

} // namespace io
} // namespace orion

#endif // ORION_IO_MAPFILE_H
