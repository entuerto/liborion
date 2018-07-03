//
// Dwarf.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_DEBUG_DW_DWARF_H
#define ORION_DEBUG_DW_DWARF_H

#include <orion/Orion-Stddefs.h>

#include "DwarfConstants.h"

#include <map>
#include <string>
#include <vector>

namespace orion
{
namespace debug
{
namespace dw
{

//--------------------------------------------------------------------------------------------------

enum class Format : uint8_t
{
   Unknown,
   Dwarf32,
   Dwarf64
};

enum class ByteOrder : uint8_t
{
   LSB,  // Least significant byte (Little-endian)
   MSB   // Most significant byte (Big-endian)
};

//--------------------------------------------------------------------------------------------------

/// DWARF section types.
enum class SectionType
{
   debug_abbrev,   // .debug_abbrev
   debug_aranges,  // .debug_aranges
   debug_frame,    // .debug_frame
   debug_info,     // .debug_info
   debug_line,     // .debug_line
   debug_loc,      // .debug_loc
   debug_macinfo,  // .debug_macinfo
   debug_pubnames, // .debug_pubnames
   debug_pubtypes, // .debug_pubtypes
   debug_ranges,   // .debug_ranges
   debug_str,      // .debug_str
   debug_types     // .debug_types
};

std::string to_string(SectionType ds);

//--------------------------------------------------------------------------------------------------

struct ObjectAccess
{
   virtual ~ObjectAccess() = default;

   virtual ByteOrder byte_order() const =0;

   virtual std::error_code load_section(SectionType dt, uint8_t*& data, std::size_t& size) =0;
};

//--------------------------------------------------------------------------------------------------

///
/// An unparsed DWARF section.
///
class Section
{
public:
   Section(SectionType dt, const uint8_t* start, std::size_t size, ByteOrder byte_order);

   SectionType section_type() const;

   ByteOrder byte_order() const;

   const uint8_t* start() const;

   std::size_t size() const;

private:
   SectionType _type;
   ByteOrder _byte_order; // Whether the data is big-endian.

   const uint8_t* _start;   // Start of the buffer.
   std::size_t _size;  
};

//--------------------------------------------------------------------------------------------------

class Reader
{
public:
   Reader(const uint8_t* data, std::size_t size, ByteOrder byte_order);

   /// Report an error for a DWARF buffer.
   void error(const std::string& msg);

   // True if we've reported an underflow error.
   bool underflow() const;

   /// Require at least count bytes in buffer.
   bool require(std::size_t count);

   /// Advance count bytes in buffer.
   bool advance(std::size_t count);

   /// Read one byte from buffer and advance 1 byte.
   uint8_t read_byte();

   /// Read a signed char from buffer and advance 1 byte.
   int8_t read_sbyte();

   /// Read a uint16 from buffer and advance 2 bytes.
   uint16_t read_uint16();

   /// Read a uint32 from buffer and advance 4 bytes.
   uint32_t read_uint32();

   /// Read a uint64 from buffer and advance 8 bytes.
   uint64_t read_uint64();

   /// Read an offset from buffer and advance the appropriate number of bytes.
   uint64_t read_offset(int is_dwarf64);

   /// Read an address from buffer and advance the appropriate number of bytes.
   uint64_t read_address(int addrsize);

   /// Read an unsigned LEB128 number.
   uint64_t read_uleb128();

   /// Read a signed LEB128 number.
   int64_t read_sleb128();

   template<typename T>
   T read()
   {
      static_assert(sizeof(T) <= 8, "T too big");

      require(sizeof(T));

      uint64_t value   = 0;
      const uint8_t* p = (const uint8_t*)_current;

      if (_byte_order == ByteOrder::LSB)
      {
         for (unsigned i = 0; i < sizeof(T); i++)
            value |= ((uint64_t)p[i]) << (i * 8);
      }
      else
      {
         for (unsigned i = 0; i < sizeof(T); i++)
            value = (value << 8) | (uint64_t)p[i];
      }

      advance(sizeof(T));

      return (T)value;
   }

private:
   const uint8_t* _current; // Next byte to read.

   std::size_t _remaining; // The number of bytes remaining.

   ByteOrder _byte_order; // Whether the data is big-endian.
   bool _underflow;       // True if we've reported an underflow error.
};

Reader make_reader(const Section& section);

//--------------------------------------------------------------------------------------------------

///
/// A single attribute in a DWARF abbreviation.
///
struct Attr
{
   Attribute name; // The attribute name.  
   Form form;      // The attribute form.  
};

///
/// A single DWARF abbreviation.
///
struct Abbrev
{
   // The abbrev code--the number used to refer to the abbrev.  
   uint64_t code;

   Tag tag;            // The entry tag.  
   bool has_children;  // has child entries.

   // The attributes.  
   std::vector<Attr> attrs;
};

///
/// The DWARF abbreviations for a compilation unit.  This structure
/// only exists while reading the compilation unit.  Most DWARF readers
/// seem to a hash table to map abbrev ID's to abbrev entries.
/// However, we primarily care about GCC, and GCC simply issues ID's in
/// numerical order starting at 1.  So we simply keep a sorted vector,
/// and try to just look up the code.  */
///
struct Abbrevs
{
   /* The number of abbrevs in the vector.  */
   size_t num_abbrevs;
   /* The abbrevs, sorted by the code field.  */
   Abbrev* abbrevs;
};

//--------------------------------------------------------------------------------------------------

// The different kinds of attribute values.
enum class AttrValueEnc
{
   Address,    // An address.  
   UInt,       // A unsigned integer.  
   SInt,       // A sigd integer.  
   String,     // A string.  
   RefUnit,    // An offset to other data in the containing unit.  
   RefInfo,    // An offset to other data within the .dwarf_info section.  
   RefSection, // An offset to data in some other section.  
   RefType,    // A type signature.  
   Block,      // A block of data (not represented).  
   Expr        // An expression (not represented).  
};

///
/// An attribute value.
///
struct AttrValue
{
   /* How the value is stored in the field u.  */
   AttrValueEnc encoding;

   union
   {
      uint64_t uint;      // ATTR_VAL_ADDRESS, ATTR_VAL_UINT, ATTR_VAL_REF*.  
      int64_t sint;       // ATTR_VAL_SINT.  
      const char* string; // ATTR_VAL_STRING.  

      /* ATTR_VAL_BLOCK not stored.  */
   } u;
};

//--------------------------------------------------------------------------------------------------

///
/// The line number program header.  
///
struct LineHeader
{
   int version;                   // The version of the line number information.
   uint32_t min_insn_len;         // The minimum instruction length.
   uint32_t max_ops_per_insn;     // The maximum number of ops per instruction.
   int line_base;                 // The line base for special opcodes.
   uint32_t line_range;           // The line range for special opcodes.
   uint32_t opcode_base;          // The opcode base--the first special opcode.
   const uint8_t* opcode_lengths; // Opcode lengths, indexed by opcode - 1.

   std::size_t dirs_count;        // The number of directory entries.
   const char** dirs;             // The directory entries.

   std::size_t filenames_count;   // The number of filenames.
   const char** filenames;        // The filenames.
};

///
/// Map a single PC value to a file/line.  We will keep a vector of
/// these sorted by PC value.  Each file/line will be correct from the
/// PC up to the PC of the next entry if there is one.  We allocate one
/// extra entry at the end so that we can use bsearch.  
///
struct Line
{
   uintptr_t pc;         // PC.  

   std::string filename; // File name.  Many entries in the array are expected to point to
                         // the same file name.  
   int line_number;      // Line number.  
   int idx;              // Index of the object in the original array read from the DWARF
                         // section, before it has been sorted.  The index makes it possible
                         // to use Quicksort and maintain stability.  
};

//--------------------------------------------------------------------------------------------------

///
/// A function described in the debug info.
///
struct Function
{

   std::string name;            // The name of the function.  
   std::string caller_filename; // If this is an inlined function, the filename of the call site.  
   int caller_lineno;           // If this is an inlined function, the line number of the call site.

   /* Map PC ranges to inlined functions.  */
   struct function_addrs* function_addrs;
   size_t function_addrs_count;
};

///
/// An address range for a function. This maps a PC value to a specific function.
///
struct FunctionAddrs
{
   // Range is LOW <= PC < HIGH.  
   uint64_t low;
   uint64_t high;

   Function* function; // Function for this address range. 
};

//--------------------------------------------------------------------------------------------------

///
/// A DWARF compilation unit. This only holds the information we need
/// to map a PC to a file and line.
///
struct Unit
{
   const uint8_t* unit_data; // The first entry for this compilation unit.  

   std::size_t unit_data_len;    // The length of the data for this compilation unit. 
   std::size_t unit_data_offset; // The offset of UNIT_DATA from the start of the information for
                                 // this compilation unit.

   int version;     // DWARF version.  
   bool is_dwarf64; // Whether unit is DWARF64.  
   int addrsize;    // Address size.  
   off_t lineoff;   // Offset into line number information.  

   std::string filename;     // Primary source file.
   std::string comp_dir;     // Compilation command working directory.
   std::string abs_filename; // Absolute file name, only set if needed.

   struct abbrevs abbrevs; /* The abbreviations for this unit.  */

   /* The fields above this point are read in during initialization and
      may be accessed freely.  The fields below this point are read in
      as needed, and therefore require care, as different threads may
      try to initialize them simultaneously.  */

   /* PC to line number mapping.  This is NULL if the values have not
      been read.  This is (struct line *) -1 if there was an error
      reading the values.  */
   struct line* lines;
   /* Number of entries in lines.  */
   size_t lines_count;

   /* PC ranges to function.  */
   struct function_addrs* function_addrs;
   size_t function_addrs_count;
};

///
/// An address range for a compilation unit.  This maps a PC value to a
/// specific compilation unit.  Note that we invert the representation
/// in DWARF: instead of listing the units and attaching a list of
/// ranges, we list the ranges and have each one point to the unit.
/// This lets us do a binary search to find the unit.  */
///
struct UnitAddrs
{
  /* Range is LOW <= PC < HIGH.  */
  uint64_t low;
  uint64_t high;
  
  Unit* u; // Compilation unit for this address range.  
};

//--------------------------------------------------------------------------------------------------
//

using Sections = std::map<SectionType, Section>;
using CompilationUnits = std::vector<CompilationUnit>;

///
/// The information we need to map a PC to a file and line.  
///
class Dwarf
{
public:
   Dwarf(const ObjectAccess* obj_access);

   const Section& section(SectionType t) const;

private:
   void init() noexcept;

   const ObjectAccess* _obj_access;

   Format _format;

   Sections _sections; // The unparsed .debug_XXXX sections.

   CompilationUnits _compilation_units;
};

} // namespace dw
} // namespace debug
} // namespace orion

#endif // ORION_DEBUG_DW_DWARF_H
