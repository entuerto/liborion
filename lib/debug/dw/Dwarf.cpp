//
// Dwarf.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <debug/dw/Dwarf.h>

namespace orion
{
namespace debug
{
namespace dw
{

// Return whether a value is the highest possible address, given the
// address size.
static int is_highest_address(uint64_t address, int addrsize)
{
   switch (addrsize)
   {
      case 1:
         return address == (uint8_t)-1;
      case 2:
         return address == (uint16_t)-1;
      case 4:
         return address == (uint32_t)-1;
      case 8:
         return address == (uint64_t)-1;
      default:
         return 0;
   }
}

//--------------------------------------------------------------------------------------------------

std::string to_string(SectionType dt)
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

   return text[ds];
}

//--------------------------------------------------------------------------------------------------

Section::Section(SectionType dt, const uint8_t* start, std::size_t size, ByteOrder byte_order)
   : _type(dt)
   , _byte_order(byte_order)
   , _start(start)
   , _size(size)
{
}

SectionType Section::section_type() const
{
   return _type;
}

ByteOrder Section::byte_order() const
{
   return _byte_order;
}

const uint8_t* Section::start() const
{
   return _start;
}

std::size_t Section::size() const
{
   return _size;
}

//--------------------------------------------------------------------------------------------------

Reader::Reader(const uint8_t* data, std::size_t size, ByteOrder byte_order)
   : _current(data)
   , _remaining(size)
   , _byte_order(byte_order)
   , _underflow(false)
{
}

// Report an error for a DWARF buffer.
void Reader::error(const std::string& msg)
{
   /*
     char b[200];

     snprintf (b, sizeof b, "%s in %s at %d",
          msg, buf->name, (int) (buf->buf - buf->start));
     buf->error_callback (buf->data, b, 0);
   */
}

bool Reader::underflow() const
{
   return _underflow;
}

// Require at least count bytes in buffer.
bool Reader::require(size_t count)
{
   if (_remaining >= count)
      return true;

   if (not _underflow)
   {
      _underflow = true;
      error("DWARF underflow");
   }

   return false;
}

// Advance count bytes in buffer.
bool Reader::advance(std::size_t count)
{
   if (not require(buf, count))
      return false;

   _current += count;
   _remaining -= count;
   return true;
}

// Read one byte from buffer and advance 1 byte.
uint8_t Reader::read_byte()
{
   const uint8_t* p = _current;

   if (not advance(1))
      return 0;

   return p[0];
}

// Read a signed char from buffer and advance 1 byte.
int8_t Reader::read_sbyte()
{
   const uint8_t* p = _current;

   if (not advance(1))
      return 0;

   return (*p ^ 0x80) - 0x80;
}

// Read a uint16 from buffer and advance 2 bytes.
uint16_t Reader::read_uint16()
{
   const uint8_t* p = _current;

   if (not advance(2))
      return 0;

   return (_byte_order == ByteOrder::MSB) ? ((uint16_t)p[0] << 8) | (uint16_t)p[1]
                                          : ((uint16_t)p[1] << 8) | (uint16_t)p[0];
}

// Read a uint32 from buffer and advance 4 bytes.
uint32_t Reader::read_uint32()
{
   const uint8_t* p = _current;

   if (not advance(4))
      return 0;

   return (_byte_order == ByteOrder::MSB) ? (((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
                                             ((uint32_t)p[2] << 8)  | (uint32_t)p[3])
                                          : (((uint32_t)p[3] << 24) | ((uint32_t)p[2] << 16) |
                                             ((uint32_t)p[1] << 8)  | (uint32_t)p[0]);
}

// Read a uint64 from buffer and advance 8 bytes.
uint64_t Reader::read_uint64()
{
   const uint8_t* p = buf->buf;

   if (not advance(8))
      return 0;

   return (_byte_order == ByteOrder::MSB)
             ? (((uint64_t)p[0] << 56) | ((uint64_t)p[1] << 48) | ((uint64_t)p[2] << 40) |
                ((uint64_t)p[3] << 32) | ((uint64_t)p[4] << 24) | ((uint64_t)p[5] << 16) |
                ((uint64_t)p[6] << 8)  | (uint64_t)p[7])
             : (((uint64_t)p[7] << 56) | ((uint64_t)p[6] << 48) | ((uint64_t)p[5] << 40) |
                ((uint64_t)p[4] << 32) | ((uint64_t)p[3] << 24) | ((uint64_t)p[2] << 16) |
                ((uint64_t)p[1] << 8)  | (uint64_t)p[0]);
}

// Read an offset from buffer and advance the appropriate number of bytes.
uint64_t Reader::read_offset(int is_dwarf64)
{
   return (is_dwarf64) ? read_uint64() : read_uint32();
}

// Read an address from buffer and advance the appropriate number of bytes.
uint64_t Reader::read_address(int addrsize)
{
   switch (addrsize)
   {
      case 1:
         return read_byte();
      case 2:
         return read_uint16();
      case 4:
         return read_uint32();
      case 8:
         return read_uint64();
      default:
         error("Unrecognized address size");
         return 0;
   }
}

// Read an unsigned LEB128 number.
uint64_t Reader::read_uleb128()
{
   uint64_t ret       = 0;
   unsigned int shift = 0;
   int overflow       = 0;
   uint8_t b;

   do
   {
      const uint8_t* p = _current;

      if (not advance(1))
         return 0;

      b = *p;
      if (shift < 64)
         ret |= ((uint64_t)(b & 0x7f)) << shift;
      else if (not overflow)
      {
         overflow = 1;
         error("LEB128 overflows uint64_t");
      }

      shift += 7;
   } while ((b & 0x80) != 0);

   return ret;
}

// Read a signed LEB128 number.
int64_t Reader::read_sleb128()
{
   uint64_t val       = 0;
   unsigned int shift = 0;
   int overflow       = 0;
   uint8_t b;

   do
   {
      const uint8_t* p = _current;

      if (not advance(1))
         return 0;

      b = *p;
      if (shift < 64)
         val |= ((uint64_t)(b & 0x7f)) << shift;
      else if (not overflow)
      {
         overflow = 1;
         error("signed LEB128 overflows uint64_t");
      }

      shift += 7;
   } while ((b & 0x80) != 0);

   if ((b & 0x40) != 0 and shift < 64)
      val |= ((uint64_t)-1) << shift;

   return (int64_t)val;
}

Reader make_reader(const Section& section)
{
   return Reader{section.start(), section.size(), section.byte_order()};
}

//--------------------------------------------------------------------------------------------------

Dwarf::Dwarf(const ObjectAccess* obj_access)
   : _obj_access(obj_access)
   , _format(Format::Unknown)
{

}

void Dwarf::init() noexcept
{
   if (_obj_access == nullptr)
      return;

   uint8_t* data = nullptr;
   std::size_t size = 0;

   // Load .debug_info
   std::error_code ec = _obj_access->load_section(SectionType::debug_info, data, size);
   if (ec)
   {
      // Error
      return;
   }

   Section info_section{SectionType::debug_info, data, size, _obj_access->byte_order};


   Reader info = make_reader(info_section);

   // Skip length
   uint32_t len = info.read_uint32();
   if (len == 0xffffffff)
   {
      len     = info.read_uint64();
      _format = Format::Dwarf64;
   }

   // Get the version 
   uint16_t version = info.read_uint16();
   if (version < 2 or version > 4)
   {
      // Error
      return;
   }

   // Load .debug_abbrev
   std::error_code ec = _obj_access->load_section(SectionType::debug_abbrev, data, size);
   if (ec)
      return;

   Section abbrev_section{SectionType::debug_abbrev, data, size, _obj_access->byte_order};

   _sections.emplace(SectionType::debug_info, info_section);
   _sections.emplace(SectionType::debug_abbrev, abbrev_section);

   // Load Compilation units
   load_compilation_units();
}

const Section& Dwarf::section(SectionType t) const
{
   auto search = _sections.find(t);

   if (search == _sections.end())
      error

   return search->second;
}

std::error_code Dwarf::load_compilation_units()
{

}

std::error_code Dwarf::read_abbrevs()
{
   Section abbrev_section = ;

   // Section 7.5.3
   for (;;)
   {
      Abbrev a;

      if (abbrev_section.underflow())
         return error;

      a.code = abbrev_section.read_uleb128();
      if (code == 0)
         break;

      a.tag          = static_cast<Tag>(abbrev_section.read_uleb128());
      a.has_children = abbrev_section.read_byte() != 0;

      for (;;)
      {
         Attribute name = (Attribute)abbrev_section.read_uleb128();
         Form form      = (Form)abbrev_section.read_uleb128();
         if (name == (Attribute)0 and form == (Form)0)
            break;
         a.attrs.push_back(Attr{name, form});
      }

      _abbrevs.push_back(a);
   }
}

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------

/* Build a mapping from address ranges to the compilation units where
   the line number information for that range can be found.  Returns 1
   on success, 0 on failure.  */

static int build_address_map(struct backtrace_state* state,

                             uintptr_t base_address,

                             const uint8_t* dwarf_info,
                             size_t dwarf_info_size,
                             const uint8_t* dwarf_abbrev,
                             size_t dwarf_abbrev_size,
                             const uint8_t* dwarf_ranges,
                             size_t dwarf_ranges_size,
                             const uint8_t* dwarf_str,
                             size_t dwarf_str_size,

                             int is_bigendian,
                             backtrace_error_callback error_callback,
                             void* data,
                             struct unit_addrs_vector* addrs)
{
   struct dwarf_current info;
   struct abbrevs abbrevs;

   memset(&addrs->vec, 0, sizeof addrs->vec);
   addrs->count = 0;

   /* Read through the .debug_info section.  FIXME: Should we use the
      .debug_aranges section?  gdb and addr2line don't use it, but I'm
      not sure why.  */

   info.name               = ".debug_info";
   info.start              = dwarf_info;
   info.buf                = dwarf_info;
   info.left               = dwarf_info_size;
   info.is_bigendian       = is_bigendian;
   info.error_callback     = error_callback;
   info.data               = data;
   info.reported_underflow = 0;

   memset(&abbrevs, 0, sizeof abbrevs);
   while (info.left > 0)
   {
      const unsigned char* unit_data_start;
      uint64_t len;
      int is_dwarf64;
      struct dwarf_current unit_current;
      int version;
      uint64_t abbrev_offset;
      int addrsize;
      struct unit* u;

      if (info.reported_underflow)
         goto fail;

      unit_data_start = info.buf;

      is_dwarf64 = 0;
      len        = read_uint32(&info);
      if (len == 0xffffffff)
      {
         len        = read_uint64(&info);
         is_dwarf64 = 1;
      }

      unit_current      = info;
      unit_current.left = len;

      if (!advance(&info, len))
         goto fail;

      version = read_uint16(&unit_current);
      if (version < 2 || version > 4)
      {
         dwarf_current_error(&unit_current, "unrecognized DWARF version");
         goto fail;
      }

      abbrev_offset = read_offset(&unit_current, is_dwarf64);
      if (!read_abbrevs(state,
                        abbrev_offset,
                        dwarf_abbrev,
                        dwarf_abbrev_size,
                        is_bigendian,
                        error_callback,
                        data,
                        &abbrevs))
         goto fail;

      addrsize = read_byte(&unit_current);

      u = ((struct unit*)backtrace_alloc(state, sizeof *u, error_callback, data));
      if (u == NULL)
         goto fail;
      u->unit_data        = unit_current.buf;
      u->unit_data_len    = unit_current.left;
      u->unit_data_offset = unit_current.buf - unit_data_start;
      u->version          = version;
      u->is_dwarf64       = is_dwarf64;
      u->addrsize         = addrsize;
      u->filename         = NULL;
      u->comp_dir         = NULL;
      u->abs_filename     = NULL;
      u->lineoff          = 0;
      u->abbrevs          = abbrevs;
      memset(&abbrevs, 0, sizeof abbrevs);

      /* The actual line number mappings will be read as needed.  */
      u->lines                = NULL;
      u->lines_count          = 0;
      u->function_addrs       = NULL;
      u->function_addrs_count = 0;

      if (!find_address_ranges(state,
                               base_address,
                               &unit_current,
                               dwarf_str,
                               dwarf_str_size,
                               dwarf_ranges,
                               dwarf_ranges_size,
                               is_bigendian,
                               error_callback,
                               data,
                               u,
                               addrs))
      {
         free_abbrevs(state, &u->abbrevs, error_callback, data);
         backtrace_free(state, u, sizeof *u, error_callback, data);
         goto fail;
      }

      if (unit_current.reported_underflow)
      {
         free_abbrevs(state, &u->abbrevs, error_callback, data);
         backtrace_free(state, u, sizeof *u, error_callback, data);
         goto fail;
      }
   }
   if (info.reported_underflow)
      goto fail;

   return 1;

fail:
   free_abbrevs(state, &abbrevs, error_callback, data);
   free_unit_addrs_vector(state, addrs, error_callback, data);
   return 0;
}

//--------------------------------------------------------------------------------------------------

/* Initialize our data structures from the DWARF debug info for a
   file.  Return NULL on failure.  */

static struct dwarf_data* build_dwarf_data(struct backtrace_state* state,

                                           uintptr_t base_address,

                                           const uint8_t* dwarf_info,
                                           size_t dwarf_info_size,
                                           const uint8_t* dwarf_line,
                                           size_t dwarf_line_size,
                                           const uint8_t* dwarf_abbrev,
                                           size_t dwarf_abbrev_size,
                                           const uint8_t* dwarf_ranges,
                                           size_t dwarf_ranges_size,
                                           const uint8_t* dwarf_str,
                                           size_t dwarf_str_size,

                                           int is_bigendian,
                                           backtrace_error_callback error_callback,
                                           void* data)
{
   struct unit_addrs_vector addrs_vec;
   struct unit_addrs* addrs;
   size_t addrs_count;
   struct dwarf_data* fdata;

   if (!build_address_map(state,
                          base_address,
                          dwarf_info,
                          dwarf_info_size,
                          dwarf_abbrev,
                          dwarf_abbrev_size,
                          dwarf_ranges,
                          dwarf_ranges_size,
                          dwarf_str,
                          dwarf_str_size,
                          is_bigendian,
                          error_callback,
                          data,
                          &addrs_vec))
      return NULL;

   if (!backtrace_vector_release(state, &addrs_vec.vec, error_callback, data))
      return NULL;
   addrs       = (struct unit_addrs*)addrs_vec.vec.base;
   addrs_count = addrs_vec.count;
   backtrace_qsort(addrs, addrs_count, sizeof(struct unit_addrs), unit_addrs_compare);

   fdata =
      ((struct dwarf_data*)backtrace_alloc(state, sizeof(struct dwarf_data), error_callback, data));
   if (fdata == NULL)
      return NULL;

   fdata->next              = NULL;
   fdata->base_address      = base_address;
   fdata->addrs             = addrs;
   fdata->addrs_count       = addrs_count;
   fdata->dwarf_info        = dwarf_info;
   fdata->dwarf_info_size   = dwarf_info_size;
   fdata->dwarf_line        = dwarf_line;
   fdata->dwarf_line_size   = dwarf_line_size;
   fdata->dwarf_ranges      = dwarf_ranges;
   fdata->dwarf_ranges_size = dwarf_ranges_size;
   fdata->dwarf_str         = dwarf_str;
   fdata->dwarf_str_size    = dwarf_str_size;
   fdata->is_bigendian      = is_bigendian;
   memset(&fdata->fvec, 0, sizeof fdata->fvec);

   return fdata;
}

} // namespace dw
} // namespace debug
} // namespace orion
