//
// DwarfWrapper.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <debug/dw/DwarfWrapper.h>

#include <orion/Assert.h>

#include <fmt/format.h>

#include <algorithm>
#include <iostream>

namespace orion
{
namespace debug
{
namespace dw
{

namespace
{

constexpr const Dwarf_Bool is_info = 1;

static std::string make_string(Dwarf_Debug dbg, char* str)
{
   if (dbg == nullptr or str == nullptr)
      return {};

   std::string result_str(str);

   dwarf_dealloc(dbg, str, DW_DLA_STRING);

   return result_str;
}

struct InAddressRange
{
   Dwarf_Addr addr;

   bool operator()(const Range& r) const { return addr >= r.address1 and addr < r.address2; }
};

} // namespace

//--------------------------------------------------------------------------------------------------

std::string to_string(SectionType st)
{
   static std::array<std::string, 12> text{".debug_abbrev",
                                           ".debug_aranges",
                                           ".debug_frame",
                                           ".debug_info",
                                           ".debug_line",
                                           ".debug_loc",
                                           ".debug_macinfo",
                                           ".debug_pubnames",
                                           ".debug_pubtypes",
                                           ".debug_ranges",
                                           ".debug_str",
                                           ".debug_types"};

   return text[static_cast<int>(st)];
}

//--------------------------------------------------------------------------------------------------

std::string tag_to_string(Dwarf_Half tag)
{
   const char* result;
   if (dwarf_get_TAG_name(tag, &result) == DW_DLV_OK)
      return std::string(result);
   return {};
}

std::string attr_to_string(Dwarf_Half attr)
{
   const char* result;
   if (dwarf_get_AT_name(attr, &result) == DW_DLV_OK)
      return std::string(result);
   return {};
}

std::string form_to_string(Dwarf_Half form)
{
   const char* result;
   if (dwarf_get_FORM_name(form, &result) == DW_DLV_OK)
      return std::string(result);
   return {};
}

//--------------------------------------------------------------------------------------------------

Error::Error(Dwarf_Debug dbg, Dwarf_Error err)
   : _error_number(dwarf_errno(err))
   , _error_message(dwarf_errmsg(err))
{
   // Dwarf_Errors have to be deallocated, and we need to do so now while dbg is
   // still in scope.
   if (dbg != nullptr)
      dwarf_dealloc(dbg, err, DW_DLA_ERROR);
   else
   {
      // From the libdwarf docs:
      // "An Dwarf_Error returned from dwarf_init() or dwarf_elf_init() in case of
      // a failure cannot be freed using dwarf_dealloc().The only way to free the
      // Dwarf_Error from either of those calls is to use free(3) directly.Every
      // Dwarf_Error must be freed by dwarf_dealloc() except those returned by
      // dwarf_init() or dwarf_elf_init()."
      free(err);
   }
}

Dwarf_Unsigned Error::error_number() const
{
   return _error_number;
}

const std::string& Error::error_message() const
{
   return _error_message;
}

const char* Error::what() const noexcept
{
   return _error_message.c_str();
}

//--------------------------------------------------------------------------------------------------

[[noreturn]] static void dwarf_error_handler(Dwarf_Error error, Dwarf_Ptr ptr_to_dbg)
{
   Dwarf_Debug dbg = *static_cast<Dwarf_Debug*>(ptr_to_dbg);

   throw Error{dbg, error};
}

//--------------------------------------------------------------------------------------------------

Debug::Debug()
   : _dbg(nullptr)
{
}

Debug::Debug(const Dwarf_Obj_Access_Interface& obj_access)
   : _dbg(nullptr)
{
   Dwarf_Error error = nullptr;

   auto& oai = const_cast<Dwarf_Obj_Access_Interface&>(obj_access);

   // Initialize the interface struct
   int ret = dwarf_object_init(&oai, dwarf_error_handler, nullptr, &_dbg, &error);

   if (ret == DW_DLV_NO_ENTRY)
   {
      _dbg = nullptr;
      return;
   }
   if (ret != DW_DLV_OK)
   {
      Error init_error(_dbg, error); // Create the error before freeing Dbg.
      dealloc_dbg();                 // If dbg was set we need to free it.
      throw init_error;
   }

   _aranges_table.set_debug(_dbg);
   _abbrevs_table.set_debug(_dbg);
   _cu_table.set_debug(_dbg);
}

Debug::Debug(Debug&& other) noexcept
   : _dbg(other._dbg)
   , _aranges_table(std::move(other._aranges_table))
   , _abbrevs_table(std::move(other._abbrevs_table))
   , _cu_table(std::move(other._cu_table))
{
}

Debug::~Debug()
{
   dealloc_dbg();
}

Debug& Debug::operator=(Debug&& other) noexcept
{
   if (_dbg != other._dbg)
      dealloc_dbg();

   _dbg           = other._dbg;
   _aranges_table = std::move(other._aranges_table);
   _abbrevs_table = std::move(other._abbrevs_table);
   _cu_table      = std::move(other._cu_table);
   return *this;
}

void Debug::dealloc_dbg()
{
   if (_dbg == nullptr)
      return;

   _aranges_table.dealloc_aranges();
   _abbrevs_table.dealloc_abbrevs();
   _cu_table.dealloc_comp_units();

   Dwarf_Error err;
   dwarf_object_finish(_dbg, &err);

   // TODO: log error
   _dbg = nullptr;
}

bool Debug::empty() const
{
   return _dbg == nullptr;
}

Dwarf_Debug Debug::get() const
{
   return _dbg;
}

Dwarf_Debug Debug::operator*() const
{
   return _dbg;
}

const CompileUnitTable& Debug::compile_units() const
{
   return _cu_table;
}

const AbbreviationsTable& Debug::abbreviations() const
{
   return _abbrevs_table;
}

const AddressRangeTable& Debug::address_ranges() const
{
   return _aranges_table;
}

//--------------------------------------------------------------------------------------------------

Die::Die()
   : _debug(nullptr)
   , _die(nullptr)
{
}

Die::Die(Dwarf_Debug dbg)
   : _debug(dbg)
   , _die(nullptr)
{
}

Die::Die(Dwarf_Debug dbg, Dwarf_Die raw_die)
   : _debug(dbg)
   , _die(raw_die)
{
}

Die::Die(Die&& other) noexcept
   : _debug(other._debug)
   , _die(other._die)
   , _source_files(std::move(other._source_files))
   , _ranges(std::move(other._ranges))
{
}

Die::~Die() = default;

Die& Die::operator=(Die&& other) noexcept
{
   dealloc_die();

   _debug        = other._debug;
   _die          = other._die;
   _source_files = std::move(other._source_files);
   _ranges       = std::move(other._ranges);

   return *this;
}

bool Die::empty() const
{
   return _die == nullptr;
}

Die::Iterator Die::children_begin() const
{
   return DieChildIterator(*this);
}

Die::Iterator Die::children_end() const
{
   return DieChildIterator();
}

Dwarf_Off Die::global_offset() const
{
   if (empty())
      return 0;

   Dwarf_Off offset;
   dwarf_dieoffset(_die, &offset, nullptr);
   return offset;
}

std::string Die::name() const
{
   if (empty())
      return {};

   Dwarf_Error error = nullptr;

   char* name = nullptr;
   int ret = dwarf_diename(_die, &name, &error);
   if (ret != DW_DLV_OK and error != nullptr)
      throw Error{_debug, error};

   // If DW_AT_name is not present, but DW_AT_specification is
   // present, then probably the actual name is in the DIE
   // referenced by DW_AT_specification.
   if (name == nullptr)
   {
      AttrValue value = attr(DW_AT_specification);
      if (not value.empty())
      {
         Dwarf_Die spec_die = nullptr;
         if (dwarf_offdie(_debug, value.get_reference(), &spec_die, &error) != DW_DLV_OK)
            return "Unknown";

         ret = dwarf_diename(spec_die, &name, &error);
         if (ret != DW_DLV_OK)
            return "Unknown";
      }
   }

   return make_string(_debug, name);
}

Dwarf_Tag Die::tag() const
{
   if (empty())
      return 0;

   Dwarf_Half result;
   dwarf_tag(_die, &result, nullptr);
   return result;
}

Dwarf_Addr Die::low_pc() const
{
   Dwarf_Addr addr = 0;

   if (dwarf_lowpc(_die, &addr, nullptr) != DW_DLV_OK)
      return 0;

   return addr;
}

Dwarf_Addr Die::high_pc() const
{
   Dwarf_Addr hipc        = 0;
   Dwarf_Half return_form = 0;
   enum Dwarf_Form_Class return_class;

   if (dwarf_highpc_b(_die, &hipc, &return_form, &return_class, nullptr) != DW_DLV_OK)
      return 0;

   if (return_class == DW_FORM_CLASS_CONSTANT)
      hipc += low_pc();

   return hipc;
}

LineTable Die::line_table() const
{
   return LineTable{*this};
}

const std::vector<std::string>& Die::source_files() const
{
   load_source_files();
   return _source_files;
}

void Die::dealloc_die()
{
   if (_debug == nullptr or _die == nullptr)
      return;

   dwarf_dealloc(_debug, _die, DW_DLA_DIE);

   _debug = nullptr;
   _die   = nullptr;

   _ranges.clear();
}

bool Die::in_range(Dwarf_Addr addr) const
{
   if (not has_attr(DW_AT_ranges))
   {
      return addr >= low_pc() and addr < high_pc();
   }

   load_ranges();

   auto result = std::find_if(std::begin(_ranges), std::end(_ranges), InAddressRange{addr});

   return result != std::end(_ranges);
}

bool Die::has_attr(Dwarf_Half attr) const
{
   if (empty())
      return false;

   Dwarf_Bool result;
   dwarf_hasattr(_die, attr, &result, nullptr);
   return result != 0;
}

AttrValue Die::attr(Dwarf_Half attr) const
{
   // Get attr.
   Dwarf_Attribute attribute;
   int ret = dwarf_attr(_die, attr, &attribute, nullptr);
   if (ret != DW_DLV_OK)
      return AttrValue{}; // Empty.

   return Die::attr(attribute);
}

AttrValue Die::attr(Dwarf_Attribute attribute) const
{
   if (attribute == nullptr)
      return AttrValue{}; // Empty.

   Dwarf_Half form;
   dwarf_whatform(attribute, &form, nullptr);
   switch (form)
   {
      case DW_FORM_ref_addr:
      case DW_FORM_ref1:
      case DW_FORM_ref2:
      case DW_FORM_ref4:
      case DW_FORM_ref8:
      case DW_FORM_ref_udata:
      case DW_FORM_ref_sig8:
      case DW_FORM_sec_offset:
      {
         Dwarf_Off reference;
         dwarf_global_formref(attribute, &reference, nullptr);
         return AttrValue{reference, AttrValueKind::Reference, form};
      }
      case DW_FORM_addr:
      case DW_FORM_addrx:
      case DW_FORM_GNU_addr_index:
      {
         Dwarf_Addr address;
         dwarf_formaddr(attribute, &address, nullptr);
         return AttrValue{address, AttrValueKind::Address, form};
      }
      case DW_FORM_flag:
      case DW_FORM_flag_present:
      {
         Dwarf_Bool boolean;
         dwarf_formflag(attribute, &boolean, nullptr);
         return AttrValue{boolean, form};
      }
      case DW_FORM_data1:
      case DW_FORM_data2:
      case DW_FORM_data4:
      case DW_FORM_data8:
      case DW_FORM_udata:
      case DW_FORM_implicit_const:
      {
         Dwarf_Unsigned unsignedv;
         dwarf_formudata(attribute, &unsignedv, nullptr);
         return AttrValue{unsignedv, AttrValueKind::Unsigned, form};
      }
      case DW_FORM_sdata:
      {
         Dwarf_Signed signedv;
         dwarf_formsdata(attribute, &signedv, nullptr);
         return AttrValue{signedv, form};
      }
      case DW_FORM_block:
      case DW_FORM_block1:
      case DW_FORM_block2:
      case DW_FORM_block4:
      {
         Dwarf_Block* blocks;
         dwarf_formblock(attribute, &blocks, nullptr);

         auto data = reinterpret_cast<uint8_t*>(blocks->bl_data);

         AttrValue result{
            std::vector<uint8_t>(data, data + blocks->bl_len), AttrValueKind::Bytes, form};

         dwarf_dealloc(_debug, blocks, DW_DLA_BLOCK);
         return result;
      }
      case DW_FORM_exprloc:
      {
         Dwarf_Unsigned expr_len;
         Dwarf_Ptr blocks;
         dwarf_formexprloc(attribute, &expr_len, &blocks, nullptr);

         auto data = reinterpret_cast<uint8_t*>(blocks);

         return AttrValue{
            std::vector<uint8_t>(data, data + expr_len), AttrValueKind::Exprloc, form};
      }
      case DW_FORM_string:
      case DW_FORM_strp:
      case DW_FORM_strp_sup:
      case DW_FORM_strx:
      case DW_FORM_GNU_strp_alt:
      case DW_FORM_GNU_str_index:
      {
         char* str;
         dwarf_formstring(attribute, &str, nullptr);
         return AttrValue{make_string(_debug, str), form};
      }
      default:
         return AttrValue{form}; // Unknown Form.
   }
}

std::string Die::find_function(Dwarf_Addr addr) const
{
   if (empty())
      return {};

   /* to find the enclosing function, skim through the children and siblings
   of this CU's DIE to find the one DIE that contains the frame address. */
   auto it  = children_begin();
   auto end = children_end();

   for (; it != end; ++it)
   {
      /* only care about DIEs that represent functions (section 3.3 of the DWARF 4 spec). */
      if (it->tag() != DW_TAG_subprogram and it->tag() != DW_TAG_inlined_subroutine)
         continue;

      if (not it->has_attr(DW_AT_low_pc))
         continue;

      if (addr < it->low_pc() or addr >= it->high_pc())
         continue;

      break;
   }

   if (it != end)
   {
      return it->name();
   }

   return {};
}

void Die::load_source_files() const
{
   if (empty() or not _source_files.empty())
      return;

   char** src_files = nullptr;
   Dwarf_Signed src_files_count;

   auto ret = dwarf_srcfiles(_die, &src_files, &src_files_count, nullptr);
   if (ret != DW_DLV_OK)
      return;

   // A file ID of 0 always means no file, so set [0] to empty string.
   _source_files.reserve(static_cast<size_t>(src_files_count) + 1);
   _source_files.emplace_back("");

   for (Dwarf_Signed i = 0; i < src_files_count; ++i)
   {
      _source_files.emplace_back(src_files[i]);
      dwarf_dealloc(_debug, src_files[i], DW_DLA_STRING);
   }
   dwarf_dealloc(_debug, src_files, DW_DLA_LIST);
}

void Die::load_ranges() const
{
   if (not has_attr(DW_AT_ranges))
      return;

   if (not _ranges.empty())
      return;

   Dwarf_Error error = nullptr;

   Dwarf_Ranges* ranges      = nullptr;
   Dwarf_Signed ranges_count = 0;
   Dwarf_Unsigned bytes      = 0;

   Dwarf_Off offset = attr(DW_AT_ranges).get_reference();

   int ret = dwarf_get_ranges_a(_debug, offset, _die, &ranges, &ranges_count, &bytes, &error);
   if (ret != DW_DLV_OK)
      return;

   for (int i = 0; i < ranges_count; ++i)
   {
      if (ranges[i].dwr_addr1 == 0 and ranges[i].dwr_addr2 == 0)
         break;

      _ranges.emplace_back(Range{ranges[i].dwr_addr1, ranges[i].dwr_addr2});
   }
}

void Die::dump() const
{
   if (empty())
      return;

   Dwarf_Error error = nullptr;

   Dwarf_Attribute* attributs   = nullptr;
   Dwarf_Signed attributs_count = 0;

   auto dealloc_attrs = [&]() {
      if (_debug == nullptr or attributs == nullptr)
         return;

      for (Dwarf_Signed i = 0; i < attributs_count; ++i)
      {
         dwarf_dealloc(_debug, attributs[i], DW_DLA_ATTR);
      }
      dwarf_dealloc(_debug, attributs, DW_DLA_LIST);
   };

   auto ret = dwarf_attrlist(_die, &attributs, &attributs_count, &error);
   if (ret != DW_DLV_OK)
   {
      dealloc_attrs();
      return;
   }

   std::cout << "Section name: " << name() << "\n"
             << "Tag: " << tag_to_string(tag()) << "\n";

   for (int i = 0; i < attributs_count; ++i)
   {
      AttrValue value = attr(attributs[i]);

      Dwarf_Half attr_num;
      dwarf_whatattr(attributs[i], &attr_num, &error);

      std::cout << attr_to_string(attr_num) << ": ";

      switch (value.kind())
      {
         case AttrValueKind::Reference:
            std::cout << value.get_reference();
            break;
         case AttrValueKind::Address:
            std::cout << value.get_address();
            break;
         case AttrValueKind::Boolean:
            std::cout << value.get_bool();
            break;
         case AttrValueKind::Unsigned:
            std::cout << value.get_unsigned();
            break;
         case AttrValueKind::Signed:
            std::cout << value.get_signed();
            break;
         case AttrValueKind::Bytes:
            break;
         case AttrValueKind::Exprloc:
            break;
         case AttrValueKind::String:
            std::cout << value.get_string();
            break;
         default:
            break;
      }
      std::cout << "\n";
   }
   std::cout << "\n";

   dealloc_attrs();

   load_ranges();

   if (_ranges.empty())
      return;

   std::cout << "Ranges\n";

   for (auto& r : _ranges)
   {
      std::cout << fmt::format("{:#014x} {:#014x}\n", r.address1, r.address2);
   }
   std::cout << "\n";
}

//--------------------------------------------------------------------------------------------------

CompileUnit::CompileUnit()
   : _cu_die()
   , _header_offset(0)
   , _abbrev_offset(0)
{
}

CompileUnit::CompileUnit(Dwarf_Debug dbg, Dwarf_Die raw_cu_die)
   : _cu_die(dbg, raw_cu_die)
   , _header_offset(0)
   , _abbrev_offset(0)
{
}

CompileUnit::CompileUnit(Dwarf_Debug dbg, Dwarf_Die raw_cu_die, Dwarf_Off hoff, Dwarf_Off aoff)
   : _cu_die(dbg, raw_cu_die)
   , _header_offset(hoff)
   , _abbrev_offset(aoff)
{
}

CompileUnit::CompileUnit(CompileUnit&& other) noexcept
   : _cu_die(std::move(other._cu_die))
   , _header_offset(other._header_offset)
   , _abbrev_offset(other._abbrev_offset)
{
}

CompileUnit::~CompileUnit() = default;

CompileUnit& CompileUnit::operator=(CompileUnit&& other) noexcept
{
   _cu_die        = std::move(other._cu_die);
   _header_offset = other._header_offset;
   _abbrev_offset = other._abbrev_offset;

   return *this;
}

const Die& CompileUnit::die() const
{
   return _cu_die;
}

std::string CompileUnit::name() const
{
   return _cu_die.name();
}

std::string CompileUnit::producer() const
{
   AttrValue value = _cu_die.attr(DW_AT_producer);
   return value.empty() ? "" : value.get_string();
}

std::string CompileUnit::language() const
{
   AttrValue value = _cu_die.attr(DW_AT_language);

   const char* name;
   dwarf_get_LANG_name(value.get_signed(), &name);

   return name;
}

std::string CompileUnit::comp_dir() const
{
   AttrValue value = _cu_die.attr(DW_AT_comp_dir);
   return value.empty() ? "" : value.get_string();
}

bool CompileUnit::empty() const
{
   return _cu_die.empty();
}

bool CompileUnit::is_address_in_range(Dwarf_Addr addr) const
{
   return _cu_die.in_range(addr);
}

void CompileUnit::dealloc_comp_unit()
{
   _cu_die.dealloc_die();

   _header_offset = 0;
   _abbrev_offset = 0;
}

void CompileUnit::dump() const
{
   _cu_die.dump();
}

//--------------------------------------------------------------------------------------------------

CompileUnitTable::CompileUnitTable()
   : _debug(nullptr)
{
}

CompileUnitTable::CompileUnitTable(Dwarf_Debug dbg)
   : _debug(dbg)
{
   init();
}

/// Move constructor
CompileUnitTable::CompileUnitTable(CompileUnitTable&& other) noexcept
   : _debug(other._debug)
   , _comp_units(std::move(other._comp_units))
{
}

/// Default destructor
CompileUnitTable::~CompileUnitTable() = default;

/// Move assignment
CompileUnitTable& CompileUnitTable::operator=(CompileUnitTable&& other) noexcept
{
   _debug      = other._debug;
   _comp_units = std::move(other._comp_units);

   return *this;
}

bool CompileUnitTable::empty() const
{
   return _debug == nullptr or _comp_units.empty();
}

std::size_t CompileUnitTable::size() const
{
   return _comp_units.size();
}

CompileUnitTable::iterator CompileUnitTable::begin() const
{
   return _comp_units.begin();
}

CompileUnitTable::iterator CompileUnitTable::end() const
{
   return _comp_units.end();
}

const CompileUnit& CompileUnitTable::at(std::size_t index) const
{
   return _comp_units.at(index);
}

const CompileUnit& CompileUnitTable::operator[](std::size_t index) const
{
   return _comp_units.at(index);
}

void CompileUnitTable::set_debug(Dwarf_Debug dbg)
{
   if (dbg == nullptr)
      return;

   if (not empty())
      dealloc_comp_units();

   _debug = dbg;

   init();
}

const CompileUnit& CompileUnitTable::find(Dwarf_Addr addr) const
{
   static const CompileUnit null_cu;

   if (_debug == nullptr or addr == 0)
      return null_cu;

   for (auto& cu : _comp_units)
   {
      if (cu.is_address_in_range(addr))
         return cu;
   }

   return null_cu;
}

void CompileUnitTable::init()
{
   if (_debug == nullptr)
      return;

   Dwarf_Unsigned current_header = 0U;
   for (;;)
   {
      Dwarf_Unsigned next_header;
      Dwarf_Off abbrev_offset;

      int ret = dwarf_next_cu_header_d(_debug,
                                       is_info,
                                       nullptr /*cu_header_length*/,
                                       nullptr /*version_stamp*/,
                                       &abbrev_offset,
                                       nullptr /*address_size*/,
                                       nullptr /*offset_size*/,
                                       nullptr /*extension_size*/,
                                       nullptr /*signature*/,
                                       nullptr /*typeoffse*/,
                                       &next_header,
                                       nullptr /*header_cu_type*/,
                                       nullptr /*error*/);
      if (ret != DW_DLV_OK)
         break;

      // Each CU header should have a CU sibling.
      Dwarf_Die raw_cu_die;
      ret = dwarf_siblingof_b(_debug, /*die*/ nullptr, is_info, &raw_cu_die, nullptr);
      if (ret != DW_DLV_OK)
         break;

      _comp_units.emplace_back(CompileUnit{_debug, raw_cu_die, current_header, abbrev_offset});

      current_header = next_header;
   }
}

void CompileUnitTable::dealloc_comp_units()
{
   for (auto& cu : _comp_units)
      cu.dealloc_comp_unit();
}

void CompileUnitTable::dump() const
{
   if (empty())
      return;

   for (auto& cu : _comp_units)
      cu.dump();
}

//--------------------------------------------------------------------------------------------------

Abbreviation::Abbreviation()
   : _debug(nullptr)
   , _abbrev(nullptr)
   , _abbrev_count(0)
{
}

Abbreviation::Abbreviation(Dwarf_Debug dbg)
   : _debug(dbg)
   , _abbrev(nullptr)
   , _abbrev_count(0)
{
}

Abbreviation::Abbreviation(Dwarf_Debug dbg, Dwarf_Abbrev raw_abbrev, Dwarf_Unsigned count)
   : _debug(dbg)
   , _abbrev(raw_abbrev)
   , _abbrev_count(count)
{
}

/// Move constructor
Abbreviation::Abbreviation(Abbreviation&& other) noexcept
   : _debug(other._debug)
   , _abbrev(other._abbrev)
   , _abbrev_count(other._abbrev_count)
{
}

/// Default destructor
Abbreviation::~Abbreviation() = default;

/// Move assignment
Abbreviation& Abbreviation::operator=(Abbreviation&& other) noexcept
{
   _debug        = other._debug;
   _abbrev       = other._abbrev;
   _abbrev_count = other._abbrev_count;

   return *this;
}

bool Abbreviation::empty() const
{
   return _abbrev == nullptr;
}

Dwarf_Half Abbreviation::tag() const
{
   if (empty())
      return 0;

   Dwarf_Half abbrev_tag = 0;
   dwarf_get_abbrev_tag(_abbrev, &abbrev_tag, nullptr);

   return abbrev_tag;
}

Dwarf_Unsigned Abbreviation::code() const
{
   if (empty())
      return 0;

   Dwarf_Unsigned abbrev_code = 0;
   dwarf_get_abbrev_code(_abbrev, &abbrev_code, nullptr);

   return abbrev_code;
}

bool Abbreviation::has_children() const
{
   if (empty())
      return false;

   Dwarf_Signed flag = DW_children_no;
   ;
   dwarf_get_abbrev_children_flag(_abbrev, &flag, nullptr);

   return flag == DW_children_yes;
}

void Abbreviation::dealloc_abbrev()
{
   if (_debug == nullptr or _abbrev == nullptr)
      return;

   dwarf_dealloc(_debug, _abbrev, DW_DLA_ABBREV);

   _debug        = nullptr;
   _abbrev       = nullptr;
   _abbrev_count = 0;
}

void Abbreviation::dump() const
{
   if (empty())
      return;

   Dwarf_Error error = nullptr;

   std::cout << "Abbrev\n"
             << " " << code() << "    " << tag_to_string(tag()) << "\n";

   Dwarf_Half attr_num = 0;
   Dwarf_Signed form   = 0;
   Dwarf_Off offset    = 0;

   for (int i = 0; i < _abbrev_count; ++i)
   {
      int ret = dwarf_get_abbrev_entry(_abbrev, i, &attr_num, &form, &offset, &error);
      if (ret != DW_DLV_OK)
         break;

      std::cout << fmt::format("  {:20} {:20}\n", attr_to_string(attr_num), form_to_string(form));
   }
   std::cout << "\n";
}

//--------------------------------------------------------------------------------------------------

AbbreviationsTable::AbbreviationsTable()
   : _debug(nullptr)
{
}

AbbreviationsTable::AbbreviationsTable(Dwarf_Debug dbg)
   : _debug(dbg)
{
   init();
}

/// Move constructor
AbbreviationsTable::AbbreviationsTable(AbbreviationsTable&& other) noexcept
   : _debug(other._debug)
   , _abbrevs(std::move(other._abbrevs))
{
}

/// Default destructor
AbbreviationsTable::~AbbreviationsTable() = default;

/// Move assignment
AbbreviationsTable& AbbreviationsTable::operator=(AbbreviationsTable&& other) noexcept
{
   _debug   = other._debug;
   _abbrevs = std::move(other._abbrevs);

   return *this;
}

bool AbbreviationsTable::empty() const
{
   return _debug == nullptr or _abbrevs.empty();
}

std::size_t AbbreviationsTable::size() const
{
   return _abbrevs.size();
}

AbbreviationsTable::iterator AbbreviationsTable::begin() const
{
   return _abbrevs.begin();
}

AbbreviationsTable::iterator AbbreviationsTable::end() const
{
   return _abbrevs.end();
}

const Abbreviation& AbbreviationsTable::at(std::size_t index) const
{
   return _abbrevs.at(index);
}

const Abbreviation& AbbreviationsTable::operator[](std::size_t index) const
{
   return _abbrevs.at(index);
}

void AbbreviationsTable::set_debug(Dwarf_Debug dbg)
{
   if (dbg == nullptr)
      return;

   if (not empty())
      dealloc_abbrevs();

   _debug = dbg;

   init();
}

void AbbreviationsTable::init()
{
   if (_debug == nullptr)
      return;

   Dwarf_Error error = nullptr;

   Dwarf_Abbrev abbrev         = nullptr;
   Dwarf_Unsigned length       = 0;
   Dwarf_Unsigned abbrev_count = 0;
   Dwarf_Unsigned offset       = 0;

   int ret = dwarf_get_abbrev(_debug, offset, &abbrev, &length, &abbrev_count, &error);
   while (ret == DW_DLV_OK)
   {
      _abbrevs.emplace_back(Abbreviation{_debug, abbrev, abbrev_count});

      offset += length;

      ret = dwarf_get_abbrev(_debug, offset, &abbrev, &length, &abbrev_count, &error);
   }
}

void AbbreviationsTable::dealloc_abbrevs()
{
   for (auto& abbrev : _abbrevs)
      abbrev.dealloc_abbrev();
}

void AbbreviationsTable::dump() const
{
   if (empty())
      return;

   for (auto& abbrev : _abbrevs)
      abbrev.dump();
}

//--------------------------------------------------------------------------------------------------

AddressRange::AddressRange()
   : _debug(nullptr)
   , _arange(nullptr)
   , _start(0)
   , _length(0)
   , _cu_offset(0)
   , _compile_unit()
{
}

AddressRange::AddressRange(Dwarf_Debug dbg)
   : _debug(dbg)
   , _arange(nullptr)
   , _start(0)
   , _length(0)
   , _cu_offset(0)
   , _compile_unit()
{
}

AddressRange::AddressRange(Dwarf_Debug dbg, Dwarf_Arange raw_arange)
   : _debug(dbg)
   , _arange(raw_arange)
   , _start(0)
   , _length(0)
   , _cu_offset(0)
   , _compile_unit()
{
   dwarf_get_arange_info_b(_arange, nullptr, nullptr, &_start, &_length, &_cu_offset, nullptr);
}

/// Move constructor
AddressRange::AddressRange(AddressRange&& other) noexcept
   : _debug(other._debug)
   , _arange(other._arange)
   , _start(other._start)
   , _length(other._length)
   , _cu_offset(other._cu_offset)
   , _compile_unit(std::move(other._compile_unit))
{
}

/// Default destructor
AddressRange::~AddressRange() = default;

/// Move assignment
AddressRange& AddressRange::operator=(AddressRange&& other) noexcept
{
   _debug        = other._debug;
   _arange       = other._arange;
   _start        = other._start;
   _length       = other._length;
   _cu_offset    = other._cu_offset;
   _compile_unit = std::move(other._compile_unit);

   return *this;
}

bool AddressRange::empty() const
{
   return _arange == nullptr;
}

Dwarf_Addr AddressRange::start() const
{
   return _start;
}

Dwarf_Addr AddressRange::end() const
{
   return _start + _length;
}

Dwarf_Unsigned AddressRange::length() const
{
   return _length;
}

Dwarf_Off AddressRange::cu_offset() const
{
   return _cu_offset;
}

const CompileUnit& AddressRange::compile_unit() const
{
   load_compile_unit();
   return _compile_unit;
}

void AddressRange::load_compile_unit() const
{
   if (not _compile_unit.empty())
      return;

   Dwarf_Error error = nullptr;

   Dwarf_Die cu_die;
   if (dwarf_offdie_b(_debug, _cu_offset, is_info, &cu_die, &error) != DW_DLV_OK)
   {
      throw Error{_debug, error};
   }

   _compile_unit = CompileUnit{_debug, cu_die};
}

void AddressRange::dealloc_arange()
{
   dwarf_dealloc(_debug, _arange, DW_DLA_ARANGE);

   _compile_unit.dealloc_comp_unit();

   _debug  = nullptr;
   _arange = nullptr;

   _start     = 0;
   _length    = 0;
   _cu_offset = 0;
}
void AddressRange::dump() const
{
   if (empty())
      return;

   Dwarf_Error error = nullptr;

   Dwarf_Addr start                  = 0;
   Dwarf_Unsigned segment            = 0;
   Dwarf_Unsigned segment_entry_size = 0;
   Dwarf_Unsigned length             = 0;
   Dwarf_Off cu_die_offset           = 0;

   int ret = dwarf_get_arange_info_b(
      _arange, &segment, &segment_entry_size, &start, &length, &cu_die_offset, &error);
   if (ret != DW_DLV_OK)
      return;

   std::cout << fmt::format("Length: {} \n", length)
             << fmt::format(
                   "Segment: {} Size: {} Offset {}\n", segment, segment_entry_size, cu_die_offset)
             << fmt::format("Address\n{:#014x} {:#014x}\n", start, start + length);
   std::cout << "\n";
}

//--------------------------------------------------------------------------------------------------

AddressRangeTable::AddressRangeTable()
   : _debug(nullptr)
{
}

AddressRangeTable::AddressRangeTable(Dwarf_Debug dbg)
   : _debug(dbg)
{
   init();
}

/// Move constructor
AddressRangeTable::AddressRangeTable(AddressRangeTable&& other) noexcept
   : _debug(other._debug)
   , _aranges(std::move(other._aranges))
{
}

/// Default destructor
AddressRangeTable::~AddressRangeTable() = default;

/// Move assignment
AddressRangeTable& AddressRangeTable::operator=(AddressRangeTable&& other) noexcept
{
   _debug   = other._debug;
   _aranges = std::move(other._aranges);

   return *this;
}

bool AddressRangeTable::empty() const
{
   return _aranges.empty();
}

std::size_t AddressRangeTable::size() const
{
   return _aranges.size();
}

const AddressRange& AddressRangeTable::at(std::size_t index) const
{
   return _aranges.at(index);
}

const AddressRange& AddressRangeTable::operator[](std::size_t index) const
{
   return _aranges.at(index);
}

void AddressRangeTable::set_debug(Dwarf_Debug dbg)
{
   if (dbg == nullptr)
      return;

   if (not empty())
      dealloc_aranges();

   _debug = dbg;

   init();
}

const AddressRange& AddressRangeTable::find(Dwarf_Addr addr) const
{
   static const AddressRange null_ar;

   if (empty())
      return null_ar;

   for (auto& ar : _aranges)
   {
      if (ar.start() == 0 and ar.end() == 0)
         continue;

      if (addr >= ar.start() and addr < ar.end())
         return ar;
   }

   return null_ar;
}

void AddressRangeTable::init()
{
   if (_debug == nullptr)
      return;

   Dwarf_Error error = nullptr;

   Dwarf_Arange* aranges;
   Dwarf_Signed aranges_count;

   if (dwarf_get_aranges(_debug, &aranges, &aranges_count, &error) != DW_DLV_OK)
   {
      throw Error{_debug, error};
   }

   for (int i = 0; i < aranges_count; ++i)
   {
      _aranges.emplace_back(AddressRange{_debug, aranges[i]});
   }
   dwarf_dealloc(_debug, aranges, DW_DLA_LIST);
}

void AddressRangeTable::dealloc_aranges()
{
   if (_debug == nullptr or _aranges.empty())
      return;

   _debug = nullptr;

   for (auto& a : _aranges)
      a.dealloc_arange();

   _aranges.clear();
}

void AddressRangeTable::dump() const
{
   if (empty())
      return;

   std::cout << "Address ranges\n"
             << "Offset   Address\n";

   for (auto& ar : _aranges)
   {
      if (ar.start() == 0 and ar.length() == 0)
         continue;

      std::cout << fmt::format("{:8} {:#014x} {:#014x}\n", ar.cu_offset(), ar.start(), ar.end());
   }
}

//--------------------------------------------------------------------------------------------------

DieChildIterator::DieChildIterator(const Die& parent)
{
   Dwarf_Die raw_child_die;
   if (dwarf_child(*parent, &raw_child_die, nullptr) == DW_DLV_OK)
      _child = std::make_shared<Die>(parent._debug, raw_child_die);
}

DieChildIterator& DieChildIterator::operator++()
{
   Expects(_child) // Incremented end DieChildIterator
   if (_child)
   {
      Dwarf_Die raw_child_die;
      int ret = dwarf_siblingof_b(_child->_debug, **_child, is_info, &raw_child_die, nullptr);

      if (ret == DW_DLV_OK)
         _child = std::make_shared<Die>(_child->_debug, raw_child_die);
      else
         _child = nullptr;
   }
   return *this;
}

//--------------------------------------------------------------------------------------------------

Attribute::Attribute()
   : _debug(nullptr)
   , _attr(nullptr)
{
}

Attribute::Attribute(Dwarf_Debug dbg)
   : _debug(dbg)
   , _attr(nullptr)
{
}

Attribute::Attribute(Dwarf_Debug dbg, Dwarf_Attribute raw_attr)
   : _debug(dbg)
   , _attr(raw_attr)
{
}

/// Move constructor
Attribute::Attribute(Attribute&& other) noexcept
   : _debug(other._debug)
   , _attr(other._attr)
{
}

/// Default destructor
Attribute::~Attribute()
{
   dealloc_attr();
}

/// Move assignment
Attribute& Attribute::operator=(Attribute&& other) noexcept
{
   _debug = other._debug;
   _attr  = other._attr;

   return *this;
}

bool Attribute::empty() const
{
   return _attr == nullptr;
}

void Attribute::dealloc_attr()
{
   if (_debug == nullptr or _attr == nullptr)
      return;

   dwarf_dealloc(_debug, _attr, DW_DLA_ATTR);

   _debug = nullptr;
   _attr  = nullptr;
}

void Attribute::dump() const
{
   if (empty())
      return;
}

//--------------------------------------------------------------------------------------------------

AttrValue::AttrValue()
   : _kind(AttrValueKind::Empty)
   , _form(0)
{
}

AttrValue::AttrValue(const AttrValue& other)
{
   _kind = other._kind;
   _form = other._form;

   switch (_kind)
   {
      case AttrValueKind::Empty:
      case AttrValueKind::UnknownForm:
         break;
      case AttrValueKind::Reference:
         _value.Reference = other._value.Reference;
         break;
      case AttrValueKind::Address:
         _value.Address = other._value.Address;
         break;
      case AttrValueKind::Boolean:
         _value.Boolean = other._value.Boolean;
         break;
      case AttrValueKind::Unsigned:
         _value.Unsigned = other._value.Unsigned;
         break;
      case AttrValueKind::Signed:
         _value.Signed = other._value.Signed;
         break;
      case AttrValueKind::Bytes:
         new (&_value.Bytes) std::vector<uint8_t>(other._value.Bytes);
         break;
      case AttrValueKind::Exprloc:
         new (&_value.Exprloc) std::vector<uint8_t>(other._value.Exprloc);
         break;
      case AttrValueKind::String:
         new (&_value.String) std::string(other._value.String);
         break;
      default:
         AssertUnreachable("Invalid AttrValueKind");
         break;
   }
}

AttrValue::AttrValue(AttrValue&& other) noexcept
{
   _kind = other._kind;
   _form = other._form;

   switch (_kind)
   {
      case AttrValueKind::Empty:
      case AttrValueKind::UnknownForm:
         break;
      case AttrValueKind::Reference:
         _value.Reference = other._value.Reference;
         break;
      case AttrValueKind::Address:
         _value.Address = other._value.Address;
         break;
      case AttrValueKind::Boolean:
         _value.Boolean = other._value.Boolean;
         break;
      case AttrValueKind::Unsigned:
         _value.Unsigned = other._value.Unsigned;
         break;
      case AttrValueKind::Signed:
         _value.Signed = other._value.Signed;
         break;
      case AttrValueKind::Bytes:
         new (&_value.Bytes) std::vector<uint8_t>(std::move(other._value.Bytes));
         break;
      case AttrValueKind::Exprloc:
         new (&_value.Exprloc) std::vector<uint8_t>(std::move(other._value.Exprloc));
         break;
      case AttrValueKind::String:
         new (&_value.String) std::string(std::move(other._value.String));
         break;
      default:
         AssertUnreachable("Invalid AttrValueKind");
         break;
   }
}

AttrValue& AttrValue::operator=(AttrValue&& other) noexcept
{
   destroy_value();

   _kind = other._kind;
   _form = other._form;

   switch (_kind)
   {
      case AttrValueKind::Empty:
      case AttrValueKind::UnknownForm:
         break;
      case AttrValueKind::Reference:
         _value.Reference = other._value.Reference;
         break;
      case AttrValueKind::Address:
         _value.Address = other._value.Address;
         break;
      case AttrValueKind::Boolean:
         _value.Boolean = other._value.Boolean;
         break;
      case AttrValueKind::Unsigned:
         _value.Unsigned = other._value.Unsigned;
         break;
      case AttrValueKind::Signed:
         _value.Signed = other._value.Signed;
         break;
      case AttrValueKind::Bytes:
         new (&_value.Bytes) std::vector<uint8_t>(std::move(other._value.Bytes));
         break;
      case AttrValueKind::Exprloc:
         new (&_value.Exprloc) std::vector<uint8_t>(std::move(other._value.Exprloc));
         break;
      case AttrValueKind::String:
         new (&_value.String) std::string(std::move(other._value.String));
         break;
      default:
         AssertUnreachable("Invalid AttrValueKind");
         break;
   }
   return *this;
}

bool AttrValue::empty() const
{
   return _kind == AttrValueKind::Empty;
}
AttrValueKind AttrValue::kind() const
{
   return _kind;
}
Dwarf_Half AttrValue::form() const
{
   Expects(not empty());
   return _form;
}

Dwarf_Off AttrValue::get_reference() const
{
   Expects(_kind == AttrValueKind::Reference);
   return _value.Reference;
}

Dwarf_Addr AttrValue::get_address() const
{
   Expects(_kind == AttrValueKind::Address);
   return _value.Address;
}

Dwarf_Bool AttrValue::get_bool() const
{
   Expects(_kind == AttrValueKind::Boolean);
   return _value.Boolean;
}

Dwarf_Unsigned AttrValue::get_unsigned() const
{
   Expects(_kind == AttrValueKind::Unsigned);
   return _value.Unsigned;
}

Dwarf_Signed AttrValue::get_signed() const
{
   Expects(_kind == AttrValueKind::Signed);
   return _value.Signed;
}

const std::vector<uint8_t>& AttrValue::get_bytes() const
{
   Expects(_kind == AttrValueKind::Bytes);
   return _value.Bytes;
}

const std::vector<uint8_t>& AttrValue::get_exprloc() const
{
   Expects(_kind == AttrValueKind::Exprloc);
   return _value.Exprloc;
}

const std::string& AttrValue::get_string() const
{
   Expects(_kind == AttrValueKind::String);
   return _value.String;
}

AttrValue::AttrValue(Dwarf_Half form)
   : _kind(AttrValueKind::UnknownForm)
   , _form(form)
{
}

AttrValue::AttrValue(Dwarf_Bool value, Dwarf_Half form)
   : _kind(AttrValueKind::Boolean)
   , _form(form)
{
   _value.Boolean = value;
}

AttrValue::AttrValue(Dwarf_Signed value, Dwarf_Half form)
   : _kind(AttrValueKind::Signed)
   , _form(form)
{
   _value.Signed = value;
}

AttrValue::AttrValue(std::vector<uint8_t>&& value, AttrValueKind kind, Dwarf_Half form)
   : _kind(kind)
   , _form(form)
{
   switch (_kind)
   {
      case AttrValueKind::Bytes:
         new (&_value.Bytes) std::vector<uint8_t>(std::move(value));
         break;
      case AttrValueKind::Exprloc:
         new (&_value.Exprloc) std::vector<uint8_t>(std::move(value));
         break;
      case AttrValueKind::UnknownForm:
      case AttrValueKind::Boolean:
      case AttrValueKind::Empty:
      case AttrValueKind::Reference:
      case AttrValueKind::Address:
      case AttrValueKind::Unsigned:
      case AttrValueKind::Signed:
      case AttrValueKind::String:
      default:
         AssertUnreachable("Invalid AttrValueKind");
         break;
   }
}

AttrValue::AttrValue(std::string&& value, Dwarf_Half form)
   : _kind(AttrValueKind::String)
   , _form(form)
{
   new (&_value.String) std::string(std::move(value));
}

AttrValue::AttrValue(Dwarf_Unsigned value, AttrValueKind kind, Dwarf_Half form)
   : _kind(kind)
   , _form(form)
{
   switch (_kind)
   {
      case AttrValueKind::Reference:
         _value.Reference = value;
         break;
      case AttrValueKind::Address:
         _value.Address = value;
         break;
      case AttrValueKind::Unsigned:
         _value.Unsigned = value;
         break;
      case AttrValueKind::Empty:
      case AttrValueKind::UnknownForm:
      case AttrValueKind::Boolean:
      case AttrValueKind::Signed:
      case AttrValueKind::Bytes:
      case AttrValueKind::Exprloc:
      case AttrValueKind::String:
      default:
         AssertUnreachable("Invalid AttrValueKind");
         break;
   }
}

void AttrValue::destroy_value()
{
   switch (_kind)
   {
      case AttrValueKind::Empty:
      case AttrValueKind::UnknownForm:
      case AttrValueKind::Reference:
      case AttrValueKind::Address:
      case AttrValueKind::Boolean:
      case AttrValueKind::Unsigned:
      case AttrValueKind::Signed:
         break;
      case AttrValueKind::Bytes:
         _value.Bytes.~vector();
         break;
      case AttrValueKind::Exprloc:
         _value.Exprloc.~vector();
         break;
      case AttrValueKind::String:
         _value.String.~basic_string();
         break;
      default:
         AssertUnreachable("Invalid AttrValueKind");
         break;
   }
   _kind = AttrValueKind::Empty;
}

//--------------------------------------------------------------------------------------------------

LineTable::LineTable(const Die& cu)
   : _context(nullptr)
   , _lines(nullptr)
   , _line_count(0U)
{
   // line_table() is only valid on compile units
   Expects(cu.tag() == DW_TAG_compile_unit); 

   Dwarf_Unsigned version  = 0;
   Dwarf_Small table_count = 0;

   auto ret = dwarf_srclines_b(*cu, &version, &table_count, &_context, nullptr);

   Expects(table_count <= 1); // Multiline table is not supported
   if (ret != DW_DLV_OK or table_count != 1)
   {
      dealloc_lines();
      return;
   }

   Dwarf_Signed sline_count = 0;

   ret = dwarf_srclines_from_linecontext(_context, &_lines, &sline_count, nullptr);
   if (ret != DW_DLV_OK)
   {
      dealloc_lines();
      return;
   }

   _line_count = (sline_count < 0) ? 0U : static_cast<std::size_t>(sline_count);
}

LineTable::LineTable(LineTable&& other) noexcept
   : _context(other._context)
   , _lines(other._lines)
   , _line_count(other._line_count)
{
}

LineTable::~LineTable()
{
   dealloc_lines();
}

LineTable& LineTable::operator=(LineTable&& other) noexcept
{
   if (this != &other)
   {
      dealloc_lines();

      _context    = other._context;
      _lines      = other._lines;
      _line_count = other._line_count;
   }
   return *this;
}

bool LineTable::empty() const
{
   return _line_count == 0;
}
std::size_t LineTable::size() const
{
   return _line_count;
}

LineEntry LineTable::operator[](std::size_t index) const
{
   return get_line(index);
}

LineEntry LineTable::get_line(std::size_t index) const
{
   Expects(index < size());

   Dwarf_Line line = _lines[index];

   LineEntry result{};

   dwarf_lineno(line, &result.lineno, nullptr);
   dwarf_line_srcfileno(line, &result.src_file_id, nullptr);
   dwarf_lineaddr(line, &result.line_addr, nullptr);

   dwarf_linebeginstatement(line, &result.is_begin_statement, nullptr);
   dwarf_lineendsequence(line, &result.is_end_sequence, nullptr);
   dwarf_lineblock(line, &result.is_begin_block, nullptr);
   dwarf_prologue_end_etc(line,
                          &result.is_prolog_end,
                          &result.is_epilogue_begin,
                          &result.isa,
                          &result.discriminator,
                          nullptr);

   return result;
}

LineEntry LineTable::find(Dwarf_Addr addr) const
{
   // Skim through all available statements to find the one that fits best
   // (with an address <= "addr", as close as possible to "addr").
   dw::LineEntry best{};

   int delta = 65;
   for (Dwarf_Signed i = 0; i < size(); ++i)
   {
      auto line_addr = get_line(i).line_addr;

      int d = addr - line_addr;
      if (d >= 0 and d < delta)
      {
         best = get_line(i);
         if (d == 0) // found a perfect match.
            break;
         delta = d;
      }
   }

   return best;
}

void LineTable::dealloc_lines()
{
   if (_context != nullptr)
      dwarf_srclines_dealloc_b(_context);

   _context    = nullptr;
   _lines      = nullptr;
   _line_count = 0U;
}

} // namespace dw
} // namespace debug
} // namespace orion
