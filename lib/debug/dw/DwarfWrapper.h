//
// DwarfWrapper.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_DEBUG_DW_DWARF_H
#define ORION_DEBUG_DW_DWARF_H

#include <orion/Config.h>

#include <dwarf.h>
#include <libdwarf.h>

#include <memory>
#include <string>
#include <system_error>
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
   LSB, // Least significant byte (Little-endian)
   MSB  // Most significant byte (Big-endian)
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

std::string to_string(SectionType st);

//--------------------------------------------------------------------------------------------------

API_EXPORT std::string tag_to_string(Dwarf_Half tag);
API_EXPORT std::string attr_to_string(Dwarf_Half attr);
API_EXPORT std::string form_to_string(Dwarf_Half form);

//--------------------------------------------------------------------------------------------------

class DieChildIterator;
class AttrValue;
class LineTable;

//--------------------------------------------------------------------------------------------------
///
/// Exception wrapping a libdwarf error code.
///
class API_EXPORT Error : public std::exception
{
public:
   Error(Dwarf_Debug dbg, Dwarf_Error err);

   Dwarf_Unsigned error_number() const;
   const std::string& error_message() const;

private:
   Dwarf_Unsigned _error_number;
   std::string _error_message;

   const char* what() const noexcept override;
};

//--------------------------------------------------------------------------------------------------
///
/// A range of addresses
///
struct Range
{
   Dwarf_Addr address1;
   Dwarf_Addr address2;
};

//--------------------------------------------------------------------------------------------------
///
/// Wrapper around a Dwarf_Die
///
class API_EXPORT Die
{
public:
   friend class DieChildIterator;

   using Iterator = DieChildIterator;

   Die();
   explicit Die(Dwarf_Debug dbg);
   Die(Dwarf_Debug dbg, Dwarf_Die raw_die);

   /// Move constructor
   Die(Die&& other) noexcept;

   /// Default destructor
   ~Die();

   /// Move assignment
   Die& operator=(Die&& other) noexcept;

   // No copy
   Die(const Die&) = delete;
   Die& operator=(const Die&) = delete;

   /// Get the wrapped Dwarf_Die instance.
   Dwarf_Die get() const { return _die; }

   /// get the wrapped Dwarf_Die instance.
   Dwarf_Die operator*() const { return _die; }

   /// Return true if there is no data.
   bool empty() const;

   Iterator children_begin() const;
   Iterator children_end() const;

   Dwarf_Off global_offset() const;

   std::string name() const;

   Dwarf_Tag tag() const;

   Dwarf_Addr low_pc() const;
   Dwarf_Addr high_pc() const;

   bool in_range(Dwarf_Addr addr) const;

   // Attribute getters.
   bool has_attr(Dwarf_Half attr) const;

   AttrValue attr(Dwarf_Half attr) const;
   AttrValue attr(Dwarf_Attribute attribute) const;

   /// Returns the line table. Only valid for compile units.
   LineTable line_table() const;

   const std::vector<std::string>& source_files() const;

   std::string find_function(Dwarf_Addr addr) const;

   Dwarf_Debug debug() const { return _debug; }

   void dump() const;

   // Deallocatte Dwarf_Die and set it to nullptr.
   void dealloc_die();

private:
   void load_source_files() const;
   void load_ranges() const;

   Dwarf_Debug _debug;
   Dwarf_Die _die;

   // Loaded on demand for better performance
   mutable std::vector<std::string> _source_files;
   mutable std::vector<Range> _ranges;
};

//--------------------------------------------------------------------------------------------------
///
/// Container for the CU Die and its metadata.
///
class API_EXPORT CompileUnit
{
public:
   CompileUnit();
   CompileUnit(Dwarf_Debug dbg, Dwarf_Die raw_cu_die);
   CompileUnit(Dwarf_Debug dbg, Dwarf_Die raw_cu_die, Dwarf_Off hoff, Dwarf_Off aoff);

   /// Move constructor
   CompileUnit(CompileUnit&& other) noexcept;

   /// Default destructor
   ~CompileUnit();

   /// Move assignment
   CompileUnit& operator=(CompileUnit&& other) noexcept;

   // No copy
   CompileUnit(const CompileUnit&) = delete;
   CompileUnit& operator=(const CompileUnit&) = delete;

   const Die& die() const;

   std::string name() const;
   std::string producer() const;
   std::string language() const;
   std::string comp_dir() const;

   /// Return true if there is no data.
   bool empty() const;

   bool is_address_in_range(Dwarf_Addr addr) const;

   void dump() const;

   // Deallocatte compile unit.
   void dealloc_comp_unit();

private:
   Die _cu_die;

   Dwarf_Off _header_offset;
   Dwarf_Off _abbrev_offset;
};

//--------------------------------------------------------------------------------------------------
///
/// Holds the compmile units of the debug object.
/// It is the owner of the compile unit objects and will deallocate them
///
class API_EXPORT CompileUnitTable
{
public:
   using iterator = std::vector<CompileUnit>::const_iterator;

   CompileUnitTable();
   explicit CompileUnitTable(Dwarf_Debug dbg);

   /// Move constructor
   CompileUnitTable(CompileUnitTable&& other) noexcept;

   /// Default destructor
   ~CompileUnitTable();

   /// Move assignment
   CompileUnitTable& operator=(CompileUnitTable&& other) noexcept;

   // No copy
   CompileUnitTable(const CompileUnitTable&) = delete;
   CompileUnitTable& operator=(const CompileUnitTable&) = delete;

   /// Return true if there is no data.
   bool empty() const;

   std::size_t size() const;

   iterator begin() const;
   iterator end() const;

   const CompileUnit& at(std::size_t index) const;
   const CompileUnit& operator[](std::size_t index) const;

   void set_debug(Dwarf_Debug dbg);

   /// Find an compilation unit for an address
   const CompileUnit& find(Dwarf_Addr addr) const;

   void dump() const;

   // Deallocatte compile units and set it to nullptr.
   void dealloc_comp_units();

private:
   void init();

   Dwarf_Debug _debug;

   std::vector<CompileUnit> _comp_units;
};

//--------------------------------------------------------------------------------------------------
///
/// Wrapper around a Dwarf_Abbrev
///
class API_EXPORT Abbreviation
{
public:
   Abbreviation();
   explicit Abbreviation(Dwarf_Debug dbg);
   Abbreviation(Dwarf_Debug dbg, Dwarf_Abbrev raw_abbrev, Dwarf_Unsigned count);

   /// Move constructor
   Abbreviation(Abbreviation&& other) noexcept;

   /// Default destructor
   ~Abbreviation();

   /// Move assignment
   Abbreviation& operator=(Abbreviation&& other) noexcept;

   // No copy
   Abbreviation(const Abbreviation&) = delete;
   Abbreviation& operator=(const Abbreviation&) = delete;

   /// Get the wrapped Dwarf_Abbrev instance.
   Dwarf_Abbrev get() const { return _abbrev; }

   /// get the wrapped Dwarf_Abbrev instance.
   Dwarf_Abbrev operator*() const { return _abbrev; }

   /// Return true if there is no data.
   bool empty() const;

   Dwarf_Half tag() const;
   Dwarf_Unsigned code() const;
   bool has_children() const;

   void dump() const;

   // Deallocatte Dwarf_Abbrev and set it to nullptr.
   void dealloc_abbrev();

private:
   Dwarf_Debug _debug;
   Dwarf_Abbrev _abbrev;
   Dwarf_Unsigned _abbrev_count;
};

//--------------------------------------------------------------------------------------------------
///
/// Holds the abbreviations table of the debug object.
/// It is the owner of the Dwarf_Abbrev objects and will deallocate them
///
class API_EXPORT AbbreviationsTable
{
public:
   using iterator = std::vector<Abbreviation>::const_iterator;

   AbbreviationsTable();
   explicit AbbreviationsTable(Dwarf_Debug dbg);

   /// Move constructor
   AbbreviationsTable(AbbreviationsTable&& other) noexcept;

   /// Default destructor
   ~AbbreviationsTable();

   /// Move assignment
   AbbreviationsTable& operator=(AbbreviationsTable&& other) noexcept;

   // No copy
   AbbreviationsTable(const AbbreviationsTable&) = delete;
   AbbreviationsTable& operator=(const AbbreviationsTable&) = delete;

   /// Return true if there is no data.
   bool empty() const;

   std::size_t size() const;

   iterator begin() const;
   iterator end() const;

   const Abbreviation& at(std::size_t index) const;
   const Abbreviation& operator[](std::size_t index) const;

   void set_debug(Dwarf_Debug dbg);

   void dump() const;

   // Deallocatte Dwarf_Arange and set it to nullptr.
   void dealloc_abbrevs();

private:
   void init();

   Dwarf_Debug _debug;

   std::vector<Abbreviation> _abbrevs;
};

//--------------------------------------------------------------------------------------------------
///
/// Wrapper around a Dwarf_Arange
/// The AddressRangeTable is responsible for deallocation
///
class API_EXPORT AddressRange
{
public:
   AddressRange();
   explicit AddressRange(Dwarf_Debug dbg);
   AddressRange(Dwarf_Debug dbg, Dwarf_Arange raw_arange);

   /// Move constructor
   AddressRange(AddressRange&& other) noexcept;

   /// Default destructor
   ~AddressRange();

   /// Move assignment
   AddressRange& operator=(AddressRange&& other) noexcept;

   // No copy
   AddressRange(const AddressRange&) = delete;
   AddressRange& operator=(const AddressRange&) = delete;

   /// Get the wrapped Dwarf_Arange instance.
   Dwarf_Arange get() const { return _arange; }

   /// get the wrapped Dwarf_Arange instance.
   Dwarf_Arange operator*() const { return _arange; }

   /// Return true if there is no data.
   bool empty() const;

   Dwarf_Addr start() const;
   Dwarf_Addr end() const;

   Dwarf_Unsigned length() const;

   Dwarf_Off cu_offset() const;

   const CompileUnit& compile_unit() const;

   void dump() const;

   // Deallocatte Dwarf_Arange and set it to nullptr.
   void dealloc_arange();

private:
   void load_compile_unit() const;

   Dwarf_Debug _debug;
   Dwarf_Arange _arange;

   // For faster access
   Dwarf_Addr _start;
   Dwarf_Unsigned _length;
   Dwarf_Off _cu_offset;

   mutable CompileUnit _compile_unit;
};

//--------------------------------------------------------------------------------------------------
///
/// Holds the address range table of the debug object.
/// It is the owner of the Dwarf_Arange objects and will deallocte them
///
class API_EXPORT AddressRangeTable
{
public:
   AddressRangeTable();
   explicit AddressRangeTable(Dwarf_Debug dbg);

   /// Move constructor
   AddressRangeTable(AddressRangeTable&& other) noexcept;

   /// Default destructor
   ~AddressRangeTable();

   /// Move assignment
   AddressRangeTable& operator=(AddressRangeTable&& other) noexcept;

   // No copy
   AddressRangeTable(const AddressRangeTable&) = delete;
   AddressRangeTable& operator=(const AddressRangeTable&) = delete;

   /// Return true if there is no data.
   bool empty() const;

   std::size_t size() const;

   const AddressRange& at(std::size_t index) const;
   const AddressRange& operator[](std::size_t index) const;

   void set_debug(Dwarf_Debug dbg);

   /// Find an address range for an address
   const AddressRange& find(Dwarf_Addr addr) const;

   void dump() const;

   // Deallocatte Dwarf_Arange and set it to nullptr.
   void dealloc_aranges();

private:
   void init();

   Dwarf_Debug _debug;

   std::vector<AddressRange> _aranges;
   // Dwarf_Arange* _aranges;
   // Dwarf_Signed  _aranges_count;
};

//--------------------------------------------------------------------------------------------------
/// Access all a DIE's children in sequence.
///
/// Typical usage:
/// @code
///   for (auto it = Die.children_begin(), end = Die.children_end(); it != end; ++it) 
///   {
///     auto tag = it->tag();
///     ...
///   }
/// @endcode
class API_EXPORT DieChildIterator
{
public:
   DieChildIterator() = default;
   explicit DieChildIterator(const Die& parent);

   const Die& operator*() const { return *_child; }
   const Die* operator->() const { return _child.get(); }

   friend bool operator==(const DieChildIterator& a, const DieChildIterator& b)
   {
      return a._child == b._child;
   }
   friend bool operator!=(const DieChildIterator& a, const DieChildIterator& b)
   {
      return !(a == b);
   }

   DieChildIterator& operator++();
   void operator++(int) { ++(*this); }

   bool at_end() const { return _child == nullptr; }

private:
   std::shared_ptr<Die> _child;
};

//--------------------------------------------------------------------------------------------------
///
/// The kind of an attribute value.
///
enum class AttrValueKind
{
   Empty,
   UnknownForm,
   Reference,
   Address,
   Boolean,
   Unsigned,
   Signed,
   Bytes,
   Exprloc,
   String,
};

//--------------------------------------------------------------------------------------------------
///
/// Wrapper around a Dwarf_Attribute
///
class API_EXPORT Attribute
{
public:
   Attribute();
   explicit Attribute(Dwarf_Debug dbg);
   Attribute(Dwarf_Debug dbg, Dwarf_Attribute raw_attr);

   /// Move constructor
   Attribute(Attribute&& other) noexcept;

   /// Default destructor
   ~Attribute();

   /// Move assignment
   Attribute& operator=(Attribute&& other) noexcept;

   // No copy
   Attribute(const Attribute&) = delete;
   Attribute& operator=(const Attribute&) = delete;

   /// Get the wrapped Dwarf_Arange instance.
   Dwarf_Attribute get() const { return _attr; }

   /// get the wrapped Dwarf_Arange instance.
   Dwarf_Attribute operator*() const { return _attr; }

   /// Return true if there is no data.
   bool empty() const;

   void dump() const;

private:
   // Deallocatte Dwarf_Attribute and set it to nullptr.
   void dealloc_attr();

   Dwarf_Debug _debug;
   Dwarf_Attribute _attr;
};

//--------------------------------------------------------------------------------------------------
///
/// Discriminated union for the values of DWARF attributes.
///
/// Can be 'Empty' signifying there was no attribute, or 'UnknownForm' where
/// the DWARF form for the attribtue is not supported.
///
class API_EXPORT AttrValue
{
public:
   AttrValue(); // Empty.
   ~AttrValue() { destroy_value(); }

   AttrValue(const AttrValue& other);
   AttrValue(AttrValue&& other) noexcept;

   AttrValue& operator=(const AttrValue& other);
   AttrValue& operator=(AttrValue&& other) noexcept;

   bool empty() const;
   AttrValueKind kind() const;
   Dwarf_Half form() const;

   Dwarf_Off get_reference() const;
   Dwarf_Addr get_address() const;
   Dwarf_Bool get_bool() const;
   Dwarf_Unsigned get_unsigned() const;
   Dwarf_Signed get_signed() const;
   const std::vector<uint8_t>& get_bytes() const;
   const std::vector<uint8_t>& get_exprloc() const;
   const std::string& get_string() const;

private:
   friend class Die;

   explicit AttrValue(Dwarf_Half Form); // Unknown Form.
   explicit AttrValue(Dwarf_Bool Val, Dwarf_Half Form);
   explicit AttrValue(Dwarf_Signed Val, Dwarf_Half Form);
   explicit AttrValue(std::string&& Val, Dwarf_Half Form);

   // Reference, Address and Unsigned have the same underlying type.
   explicit AttrValue(Dwarf_Unsigned Val, AttrValueKind ValKind, Dwarf_Half Form);

   // Bytes and Exprloc have the same underlying type.
   explicit AttrValue(std::vector<uint8_t>&& Val, AttrValueKind ValKind, Dwarf_Half Form);

   void destroy_value();

   union ValueUnion
   {
      Dwarf_Off Reference;
      Dwarf_Addr Address;
      Dwarf_Bool Boolean;
      Dwarf_Unsigned Unsigned;
      Dwarf_Signed Signed;
      std::vector<uint8_t> Bytes;
      std::vector<uint8_t> Exprloc;
      std::string String;

      ValueUnion() {}
      ~ValueUnion() {}
   };

   AttrValueKind _kind;
   Dwarf_Half _form;
   ValueUnion _value;
};

//--------------------------------------------------------------------------------------------------

struct LineEntry
{
   Dwarf_Unsigned lineno;
   Dwarf_Unsigned src_file_id;
   Dwarf_Addr line_addr;

   Dwarf_Bool is_begin_statement;
   Dwarf_Bool is_end_sequence;
   Dwarf_Bool is_begin_block;
   Dwarf_Bool is_prolog_end;
   Dwarf_Bool is_epilogue_begin;
   Dwarf_Unsigned isa;
   Dwarf_Unsigned discriminator;
};

//--------------------------------------------------------------------------------------------------
///
/// Wrapper around a line table
///
class API_EXPORT LineTable
{
public:
   explicit LineTable(const Die& cu);

   /// Move constructor
   LineTable(LineTable&& other) noexcept;

   /// Default destructor
   ~LineTable();

   // No copy
   LineTable(const LineTable&) = delete;
   LineTable& operator=(const LineTable&) = delete;

   // Move assignment
   LineTable& operator=(LineTable&& other) noexcept;

   bool empty() const;
   size_t size() const;

   LineEntry get_line(std::size_t index) const;
   LineEntry operator[](std::size_t index) const;

   LineEntry find(Dwarf_Addr addr) const;

private:
   // Deallocate lines and reset values to null/zero.
   void dealloc_lines();

   Dwarf_Line_Context _context;

   Dwarf_Line* _lines;
   std::size_t _line_count;
};

//--------------------------------------------------------------------------------------------------
///
/// Wrapper around a Dwarf_Debug
///
class API_EXPORT Debug
{
public:
   Debug();

   explicit Debug(const Dwarf_Obj_Access_Interface& obj_access);

   /// Move constructor
   Debug(Debug&& other) noexcept;

   /// Default descructor
   ~Debug();

   /// Move assignment
   Debug& operator=(Debug&& other) noexcept;

   // No copy
   Debug(const Debug&) = delete;
   Debug& operator=(const Debug&) = delete;

   /// Return true if there is no debug data.
   bool empty() const;

   /// Get the wrapped Dwarf_Debug instance.
   Dwarf_Debug get() const;

   /// Get the wrapped Dwarf_Debug instance.
   Dwarf_Debug operator*() const;

   /// Get all the compile units in the debug data.
   const CompileUnitTable& compile_units() const;

   const AbbreviationsTable& abbreviations() const;

   const AddressRangeTable& address_ranges() const;

private:
   void init() noexcept;

   // Free _dbg and set it to nullptr.
   void dealloc_dbg();

   Dwarf_Debug _dbg;

   AddressRangeTable _aranges_table;
   AbbreviationsTable _abbrevs_table;
   CompileUnitTable _cu_table;
};

} // namespace dw
} // namespace debug
} // namespace orion

#endif // ORION_DEBUG_DW_DWARF_H
