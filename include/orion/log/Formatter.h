//
// Formatter.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_FORMATTER_H
#define ORION_LOG_FORMATTER_H

#include <orion/Orion-Stddefs.h>

#include <orion/SourceLocation.h>

#include <memory>
#include <string>

namespace orion
{
namespace log
{
class Record;

/// A Formatter is used to convert a Record to text.
/// 
/// A Formatter class is responsible for converting a Record to a readable form that 
/// will be interpreted by either a human or an external system.
/// 
class API_EXPORT Formatter
{
public:
   virtual ~Formatter() = default;

   /// Formats the Record into a reable form
   virtual std::string format(const Record& record) = 0;
};

/// One line log formatter
///
/// Formats the log record on a single line.
///
class API_EXPORT OnelineFormatter : public Formatter
{
public:
   /// Constructor
   /// 
   /// @param with_src_loc Adds source line information
   /// 
   explicit OnelineFormatter(bool with_src_loc = false);

   std::string format(const Record& record) override;

private:
   bool _with_src_loc;
};

API_EXPORT std::unique_ptr<OnelineFormatter> make_one_line_formatter(bool with_src_loc);

/// Multi line formatter
///
/// Formats the information on more than one line.
///
class API_EXPORT MultilineFormatter : public Formatter
{
public:
   std::string format(const Record& record) override;

private:
   std::string format_source_info(const std::string& prefix, const SourceLocation& src_loc);
};

API_EXPORT std::unique_ptr<MultilineFormatter> make_multiline_formatter();

} // namespace log
} // namespace orion
#endif /* ORION_LOG_FORMATTER_H */
