//  Path.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/fs/Path.h>

#include <iostream>
#include <boost/format.hpp>

#include <windows.h>

#include <orion/StringUtils.h>

namespace orion
{
namespace fs
{
size_t root_name_end(const std::string& p);
//---------------------------------------------------------------------------------------
static const Path::value_type* separators = "/\\";

inline bool is_separator(char c)    { return c == '/' or c == '\\'; }
inline bool is_separator(wchar_t c) { return c == L'/' or c == L'\\'; }

size_t root_name_start(const std::string& p) {
   auto i = root_name_end(p);

   return (i == std::string::npos) ? 0 : std::string::npos;
}

size_t root_name_end(const std::string& p)
{
   // case  "c:/" 
   if (p.size() > 2 and p[1] == ':' and is_separator(p[2])) 
      return 1;

   // case "//" or "c:"
   if (p.size() == 2) 
   {
      if ((is_separator(p[0]) and is_separator(p[1])) or
          (not is_separator(p[0]) and p[1] == ':'))
         return 1;
   }
   
   // case "//net {/}" (UNC)
   if (p.size() > 3       and
       is_separator(p[0]) and
       is_separator(p[1]) and
       not is_separator(p[2]))
   {
      size_t pos = p.find_first_of(separators, 2);
      return (pos < p.size()) ? pos - 1 : p.size() - 1;
   }

   // case "/"
   return (p.size() > 0 and is_separator(p[0])) ? 0 : std::string::npos;
}

// Returns npos if no root_directory found
size_t root_directory_start(const std::string& p)
{
   auto i = root_name_end(p);

   if (i == std::string::npos)
      return is_separator(p[0]) ? 0 : std::string::npos;

   if (i + 1 < p.size())
      return (is_separator(p[i + 1])) ? i + 1 : i;

   return std::string::npos;
}

size_t root_directory_end(const std::string& p)
{
   size_t start_pos = root_directory_start(p);

   if (start_pos == std::string::npos)
      return std::string::npos;

   if (is_separator(p[start_pos]))
   {
      size_t i = p.find_first_not_of(separators, start_pos);

      return (i != std::string::npos) ? i - 1 : start_pos;
   }

   return start_pos;
}

std::pair<std::string, std::string> parse_filename(const std::string& name)
{
   if (name == "." or name == ".." or name.empty()) 
      return std::make_pair(name, "");

   auto pos = name.find_last_of('.');

   if (pos == std::string::npos) 
      return std::make_pair(name, "");

   return std::make_pair(name.substr(0, pos), name.substr(pos));
}

void debug(const std::string& p)
{
   std::cout << boost::format("%d = root_name_start(p)\n"
                              "%d = root_name_end(p)\n"
                              "%d = root_directory_start(p)\n"
                              "%d = root_directory_end(p)\n")
                  % root_name_start(p)
                  % root_name_end(p)
                  % root_directory_start(p)
                  % root_directory_end(p);
}

void debug(const std::string& f, 
           const std::string& value, 
                 size_t s, size_t c, size_t pos,
           const std::string& p)
{
   std::cout << boost::format("%s: %s - _pathname(s:%d, c:%d, pos:%d, %s)\n") 
                   % f
                   % value
                   % s
                   % c
                   % pos
                   % p;
}

//---------------------------------------------------------------------------------------
Path::Path():
   _pathname()
{  
}

Path::Path(const std::string& str):
   _pathname(str)
{
}

Path::Path(const Path& Other):
   _pathname(Other._pathname)
{  
}

Path::Path(Path&& Other):
   _pathname(std::move(Other._pathname))
{  
}

Path::~Path()
{  
}

Path& Path::operator=(const Path& Rhs)
{  
   if (this == &Rhs)
      return *this;

   _pathname = Rhs._pathname;

   return *this;
}

Path& Path::operator=(Path&& Rhs)
{  
   _pathname = std::move(Rhs._pathname);
   return *this;
}

Path& Path::operator/=(const Path& p)
{  
   join(p);
   return *this;
}

Path& Path::operator/=(const std::string& str)
{
   join(str);
   return *this;
}

void Path::join(const Path& p)
{
   if (p.empty())
      return;

   if (not is_separator(p._pathname[0]))
      _pathname += separator;

   _pathname += p._pathname;
}

void Path::join(const std::string& str)
{
   if (str.empty())
      return;

   if (not is_separator(str[0]))
      _pathname += separator;

   _pathname += str;
}

bool Path::empty() const
{
   return _pathname.empty();
}

Path& Path::normalize(value_type s /* = separator */)
{
   std::replace(_pathname.begin(), _pathname.end(), slash, s);
   return *this;
}

int Path::compare(const Path& p) const
{
   return _pathname.compare(p._pathname);
}

int Path::compare(const std::string& s) const
{
   return _pathname.compare(s);
}

Path::operator std::string()
{
   return _pathname;
}

Path::operator const std::string() const
{
   return _pathname;
}

Path::operator std::wstring()
{
   return utf8_to_wstring(_pathname);
}

Path::operator const std::wstring() const
{
   return utf8_to_wstring(_pathname);
}

///
/// Returns the root path of the path. If the path does not include root path, 
/// returns Path().
///
/// Effectively, returns the following: root_name() / root_directory()
///
Path Path::root_path() const
{  
   //debug(_pathname);

   size_t pos = root_directory_start(_pathname);

   auto c = (pos != std::string::npos) ? pos + 1 : std::string::npos; 

   //debug("root_path()", _pathname.substr(0, c), 0, c, pos, _pathname);

   return Path(_pathname.substr(0, c));
}

///
/// Returns the root name of the path. If the path does not include root name, 
/// returns Path().
///
Path Path::root_name() const
{  
   size_t pos = root_name_end(_pathname);

   if (pos == std::string::npos)
      return Path();

   auto c = (pos == 0) ? 0 : pos + 1;

   //debug("root_name()", _pathname.substr(0, c), 0, c, pos, _pathname);

   return Path(_pathname.substr(0, c));
}

///
/// Returns the root directory of the path. If the path does not include root 
/// name, returns Path().
///
Path Path::root_directory() const
{  
   size_t pos = root_directory_start(_pathname);

   if (pos != std::string::npos)
   {
      //debug("root_directory()", _pathname.substr(pos, 1), pos, 1, pos, _pathname);

      return Path(_pathname.substr(pos, 1));
   }
   //std::cout << "root_directory(): \n";
   return Path();
}

///
/// Returns path relative to root path. If *this is an empty path, returns 
/// an empty path.
///
Path Path::relative_path() const
{  
   size_t pos = root_directory_end(_pathname);

   if (pos != std::string::npos)
   {
      //std::cout << boost::format("relative_path(): %s\n") 
      //             % _pathname.substr(pos + 1);
      return Path(_pathname.substr(pos + 1));
   }
   //std::cout << "relative_path(): \n"; 
   return Path();
}

Path Path::parent_path() const
{  
   //std::cout << boost::format("parent_path(): %s\n") 
   //                % _pathname;
   return Path();
}

Path Path::filename() const
{  
   std::cout << boost::format("filename(): %s\n") 
                   % _pathname;
   //return empty() ? Path() : *--end();
   return Path();
}

Path Path::stem() const
{  
   std::cout << boost::format("stem(): %s\n") 
                   % _pathname;
   return Path();
}

Path Path::extension() const
{  
   std::cout << boost::format("extension(): %s\n") 
                   % _pathname;
   return Path();
}

} // fs
} // orion
