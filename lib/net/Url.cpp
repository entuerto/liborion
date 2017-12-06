//
//  Url.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/Url.h>

#include <orion/String.h>

#include <http-parser/http_parser.h>

namespace orion
{
namespace net
{
//---------------------------------------------------------------------------------------
// Private name space
namespace priv
{
inline bool ascii_alpha(char ch)
{
   return (ch >= 0x41 and ch <= 0x5a) or (ch >= 0x61 and ch <= 0x7a);
}

inline bool ascii_digit(char ch)
{
   return ch >= 0x30 and ch <= 0x39;
}

inline bool ascii_alphanumeric(char ch)
{
   return ascii_digit(ch) or ascii_alpha(ch);
}

inline bool ascii_hex_digit(char ch)
{
   return ascii_digit(ch) or (ch >= 0x41 and ch <= 0x46) or (ch >= 0x61 and ch <= 0x66);
}

inline uint16_t hex2bin(const char ch)
{
   if (ch >= '0' and ch <= '9')
      return ch - '0';

   if (ch >= 'A' and ch <= 'F')
      return 10 + (ch - 'A');

   if (ch >= 'a' and ch <= 'f')
      return 10 + (ch - 'a');

   return static_cast<uint16_t>(-1);
}

std::string percent_decode(const char* input, std::size_t len)
{
   if (len == 0)
      return "";

   std::string dest;

   dest.reserve(len);

   const char* ptr = input;
   const char* end = input + len;

   std::size_t remaining = ptr - end - 1;

   while (ptr < end)
   {
      const char ch = ptr[0];

      remaining = (end - ptr) + 1;

      if (ch == '+')
      {
         dest += ' ';
         ptr++;
         continue;
      }

      if (ch != '%' or remaining < 2 or
          (ch == '%' and (not ascii_hex_digit(ptr[1]) or not ascii_hex_digit(ptr[2]))))
      {
         dest += ch;
         ptr++;
         continue;
      }

      uint16_t a = hex2bin(ptr[1]);
      uint16_t b = hex2bin(ptr[2]);

      char c = static_cast<char>(a * 16 + b);
      dest += static_cast<char>(c);

      ptr += 3;
   }

   return dest;
}

const char UPPER_XDIGITS[] = "0123456789ABCDEF";

struct EncodePathChar
{
   bool should_encode(char c) const
   {
      // §2.3 Unreserved characters (alphanum)
      if (ascii_alphanumeric(c))
         return false;

      switch (c)
      {
         case '-': // §2.3 Unreserved characters (mark)
         case '_':
         case '.':
         case '~':
            return false;
         case '$': // §2.2 Reserved characters (reserved)
         case '&':
         case '+':
         case ',':
         case '/':
         case ':':
         case ';':
         case '=':
         case '?':
         case '@':
            // §3.3
            // The RFC allows : @ & = + $ but saves / ; , for assigning
            // meaning to individual path segments. This package
            // only manipulates the path as a whole. That leaves
            // only ? to escape.
            return c == '?';
      }
      // Everything else must be escaped.
      return true;
   }

   std::string operator()(char c) const
   {
      std::string out;

      if (not should_encode(c))
         out = c;
      else
      {
         out += '%';
         out += UPPER_XDIGITS[c >> 4];
         out += UPPER_XDIGITS[(c & 0x0f)];
      }
      return out;
   }
};

struct EncodeUserPasswordChar
{
   bool should_encode(char c) const
   {
      // §2.3 Unreserved characters (alphanum)
      if (ascii_alphanumeric(c))
         return false;

      switch (c)
      {
         case '-': // §2.3 Unreserved characters (mark)
         case '_':
         case '.':
         case '~':
            return false;
         case '$': // §2.2 Reserved characters (reserved)
         case '&':
         case '+':
         case ',':
         case '/':
         case ':':
         case ';':
         case '=':
         case '?':
         case '@':
            // §3.2.1
            // The RFC allows ';', ':', '&', '=', '+', '$', and ',' in
            // userinfo, so we must escape only '@', '/', and '?'.
            // The parsing of userinfo treats ':' as special so we must escape
            // that too.
            return c == '@' or c == '/' or c == '?' or c == ':';
      }
      // Everything else must be escaped.
      return true;
   }

   std::string operator()(char c) const
   {
      std::string out;

      if (not should_encode(c))
         out = c;
      else
      {
         out += '%';
         out += UPPER_XDIGITS[c >> 4];
         out += UPPER_XDIGITS[(c & 0x0f)];
      }
      return out;
   }
};

struct EncodeQueryChar
{
   bool should_encode(char c) const
   {
      // §2.3 Unreserved characters (alphanum)
      if (ascii_alphanumeric(c))
         return false;

      switch (c)
      {
         case '-': // §2.3 Unreserved characters (mark)
         case '_':
         case '.':
         case '~':
            return false;
         case '$': // §3.4
         case '&': // The RFC reserves (so we must escape) everything.
         case '+':
         case ',':
         case '/':
         case ':':
         case ';':
         case '=':
         case '?':
         case '@':
            return true;
      }
      // Everything else must be escaped.
      return true;
   }

   std::string operator()(char c) const
   {
      std::string out;

      if (not should_encode(c))
         out = c;
      else if (c == ' ')
         out = '+';
      else
      {
         out += '%';
         out += UPPER_XDIGITS[c >> 4];
         out += UPPER_XDIGITS[(c & 0x0f)];
      }
      return out;
   }
};

struct EncodeFragmentChar
{
   bool should_encode(char c) const
   {
      // §2.3 Unreserved characters (alphanum)
      if (ascii_alphanumeric(c))
         return false;

      switch (c)
      {
         case '-': // §2.3 Unreserved characters (mark)
         case '_':
         case '.':
         case '~':
            return false;
         case '$': // §4.1
         case '&': // The RFC text is silent but the grammar allows
         case '+': // everything, so escape nothing.
         case ',':
         case '/':
         case ':':
         case ';':
         case '=':
         case '?':
         case '@':
            return false;
      }
      // Everything else must be escaped.
      return true;
   }

   std::string operator()(char c) const
   {
      std::string out;

      if (not should_encode(c))
         out = c;
      else
      {
         out += '%';
         out += UPPER_XDIGITS[c >> 4];
         out += UPPER_XDIGITS[(c & 0x0f)];
      }
      return out;
   }
};

template<typename T>
std::string encode(const std::string& str)
{
   T e;

   std::string out;

   for (const auto& c : str)
   {
      out += e(c);
   }

   return out;
}

int normalize_port(const std::string& scheme, int port)
{
   if (port > 0)
      return port;

   if (scheme == "ftp")
      return 21;
   else if (scheme == "file")
      return -1;
   else if (scheme == "gopher")
      return 70;
   else if (scheme == "http")
      return 80;
   else if (scheme == "https")
      return 443;
   else if (scheme == "ws")
      return 80;
   else if (scheme == "wss")
      return 443;

   return -1;
}

} // namespace priv

//---------------------------------------------------------------------------------------

Url::Url()
   : _userinfo()
   , _port(-1)
   , _protocol()
   , _hostname()
   , _pathname()
   , _hash()
   , _query()
{
}

Url::Url(const std::string& value)
   : _userinfo()
   , _port(-1)
   , _protocol()
   , _hostname()
   , _pathname()
   , _hash()
   , _query()
{
   parse(value);
}

Url::Url(const Url& rhs)
   : _userinfo(rhs._userinfo)
   , _port(rhs._port)
   , _protocol(rhs._protocol)
   , _hostname(rhs._hostname)
   , _pathname(rhs._pathname)
   , _hash(rhs._hash)
   , _query(rhs._query)
{
}

Url::Url(Url&& rhs)
   : _userinfo(std::move(rhs._userinfo))
   , _port(std::move(rhs._port))
   , _protocol(std::move(rhs._protocol))
   , _hostname(std::move(rhs._hostname))
   , _pathname(std::move(rhs._pathname))
   , _hash(std::move(rhs._hash))
   , _query(std::move(rhs._query))
{
}

Url::~Url()
{
}

Url& Url::operator=(const Url& rhs)
{
   if (this == &rhs)
      return *this;

   _userinfo = rhs._userinfo;
   _port     = rhs._port;
   _protocol = rhs._protocol;
   _hostname = rhs._hostname;
   _pathname = rhs._pathname;
   _hash     = rhs._hash;
   _query    = rhs._query;

   return *this;
}

Url& Url::operator=(Url&& rhs)
{
   _userinfo = std::move(rhs._userinfo);
   _port     = std::move(rhs._port);
   _protocol = std::move(rhs._protocol);
   _hostname = std::move(rhs._hostname);
   _pathname = std::move(rhs._pathname);
   _hash     = std::move(rhs._hash);
   _query    = std::move(rhs._query);

   return *this;
}

std::string Url::protocol() const
{
   return _protocol;
}

void Url::protocol(const std::string& value)
{
   _protocol = value;
   to_lower(_protocol);
}

UserInfo Url::userinfo() const
{
   return _userinfo;
}

void Url::userinfo(const UserInfo& value)
{
   _userinfo = value;
}

std::string Url::host() const
{
   return _hostname + ":" + std::to_string(port());
}

std::string Url::hostname() const
{
   return _hostname;
}

void Url::hostname(const std::string& value)
{
   _hostname = value;
   to_lower(_hostname);
}

int Url::port() const
{
   return priv::normalize_port(_protocol, _port);
}

void Url::port(int value)
{
   _port = value;
}

std::string Url::path() const
{
   std::string out;

   out += priv::encode<priv::EncodePathChar>(_pathname);
   if (not _query.empty())
   {
      out += "?";
      for (const auto& item : _query)
      {
         out += priv::encode<priv::EncodeQueryChar>(item.first);
         out += "=";
         out += priv::encode<priv::EncodeQueryChar>(item.second);
         out += "&";
      }
      out.erase(--out.end());
   }

   return out;
}

std::string Url::pathname() const
{
   return _pathname;
}

void Url::pathname(const std::string& value)
{
   _pathname = value;
}

std::string Url::query(const std::string& key) const
{
   auto item = _query.find(key);

   if (item != _query.end())
      return item->second;

   return "";
}

void Url::query(const std::string& key, const std::string& value)
{
   _query.emplace(std::make_pair(key, value));
}

std::string Url::hash() const
{
   return _hash;
}

void Url::hash(const std::string& value)
{
   _hash = value;
}

std::string Url::href() const
{
   // 'http://user:pass@host.com:8080/p/a/t/h?query=string#hash'
   std::string out;

   if (not _protocol.empty())
   {
      out += _protocol;
      out += "://";
   }
   if (not _userinfo.username.empty())
   {
      out += priv::encode<priv::EncodeUserPasswordChar>(_userinfo.username);
      if (not _userinfo.password.empty())
      {
         out += ":";
         out += priv::encode<priv::EncodeUserPasswordChar>(_userinfo.password);
      }
      out += "@";
   }
   out += _hostname;
   if (_port > 0)
   {
      out += ":";
      out += std::to_string(_port);
   }
   out += priv::encode<priv::EncodePathChar>(_pathname);
   if (not _query.empty())
   {
      out += "?";
      for (const auto& item : _query)
      {
         out += priv::encode<priv::EncodeQueryChar>(item.first);
         out += "=";
         out += priv::encode<priv::EncodeQueryChar>(item.second);
         out += "&";
      }
      out.erase(--out.end());
   }
   if (not _hash.empty())
   {
      out += '#';
      out += priv::encode<priv::EncodeFragmentChar>(_hash.substr(1));
   }
   return out;
}

void Url::parse(const std::string& value)
{
   struct http_parser_url u;

   http_parser_url_init(&u);

   int result = http_parser_parse_url(value.c_str(), value.length(), 0, &u);

   if (result != 0)
      return;

   _port = u.port;

   if (u.field_set & (1 << UF_SCHEMA))
      _protocol = value.substr(u.field_data[UF_SCHEMA].off, u.field_data[UF_SCHEMA].len);

   if (u.field_set & (1 << UF_HOST))
      _hostname =
         priv::percent_decode(value.c_str() + u.field_data[UF_HOST].off, u.field_data[UF_HOST].len);

   if (u.field_set & (1 << UF_PATH))
      _pathname =
         priv::percent_decode(value.c_str() + u.field_data[UF_PATH].off, u.field_data[UF_PATH].len);

   if (u.field_set & (1 << UF_FRAGMENT))
      _hash = "#" + priv::percent_decode(value.c_str() + u.field_data[UF_FRAGMENT].off,
                                         u.field_data[UF_FRAGMENT].len);

   if (u.field_set & (1 << UF_USERINFO))
   {
      std::string userinfo = priv::percent_decode(value.c_str() + u.field_data[UF_USERINFO].off,
                                                  u.field_data[UF_USERINFO].len);

      auto pos = userinfo.find(":");
      if (pos != std::string::npos)
      {
         _userinfo.username = userinfo.substr(0, pos);
         _userinfo.password = userinfo.substr(pos + 1);
      }
      else
         _userinfo.username = userinfo;
   }

   if (u.field_set & (1 << UF_QUERY))
   {
      std::string p = priv::percent_decode(value.c_str() + u.field_data[UF_QUERY].off,
                                           u.field_data[UF_QUERY].len);

      auto query_items = split(p, '&');

      if (not query_items.empty())
      {
         for (const auto& item : query_items)
         {
            auto q = split(item, '=');
            if (not q.empty())
               query(q.front(), q.back());
         }
      }
      else
      {
         auto q = split(p, '=');
         if (not q.empty())
            query(q.front(), q.back());
      }
   }
   to_lower(_protocol);
   to_lower(_hostname);
}

} // namespace net
} // namespace orion
