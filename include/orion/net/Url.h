//  Url.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_URL_H
#define ORION_NET_URL_H

#include <orion/Orion-Stddefs.h>

#include <map>
#include <string>

namespace orion
{
namespace net
{
///
struct API_EXPORT UserInfo
{
   std::string username;
   std::string password;
};

/// A URL is a universal identifier.
/// To disambiguate from a URL string it can also be referred to as a URL record.
///
/// Example for: 'http://user:pass@host.com:8080/p/a/t/h?query=string#hash'
///┌─────────────────────────────────────────────────────────────────────────────┐
///│                                    href                                     │
///├──────────┬┬───────────┬─────────────────┬───────────────────────────┬───────┤
///│ protocol ││   auth    │      host       │           path            │ hash  │
///│          ││           ├──────────┬──────┼──────────┬────────────────┤       │
///│          ││           │ hostname │ port │ pathname │     search     │       │
///│          ││           │          │      │          ├─┬──────────────┤       │
///│          ││           │          │      │          │ │    query     │       │
///"  http:   // user:pass @ host.com : 8080   /p/a/t/h  ?  query=string   #hash "
///│          ││           │          │      │          │ │              │       │
///└──────────┴┴───────────┴──────────┴──────┴──────────┴─┴──────────────┴───────┘
///(all spaces in the "" line should be ignored -- they are purely for formatting)
class API_EXPORT Url
{
public:
   Url();
   Url(const std::string& value);
   Url(const Url& rhs);
   Url(Url&& rhs);
   ~Url();

   Url& operator=(const Url& rhs);
   Url& operator=(Url&& rhs);

   /// Identifies the URL's lower-cased protocol scheme.
   ///
   /// Example: 'http'
   std::string protocol() const;
   void protocol(const std::string& value);

   /// The username and password portion of the URL. This string subset follows the
   /// protocol and double slashes (if present) and precedes the host component, delimited
   /// by an ASCII "at sign" (@). The format of the string is {username}[:{password}],
   /// with the [:{password}] portion being optional.
   ///
   /// Example: 'user:pass'
   UserInfo userinfo() const;
   void userinfo(const UserInfo& value);

   ///  Lower-cased host name portion of the host component with the port included.
   ///
   /// Emple: host.com:80
   std::string host() const;

   /// Lower-cased host name portion of the host component without the port included.
   ///
   /// Example: 'host.com'
   std::string hostname() const;
   void hostname(const std::string& value);

   /// The numeric port portion of the host component.
   ///
   /// Example: '8080'
   int port() const;
   void port(int value);

   /// Consists of the entire path section of the URL and the search section.
   std::string path() const;

   /// Consists of the entire path section of the URL. This is everything following
   /// the host (including the port) and before the start of the query or hash components,
   /// delimited by either the ASCII question mark (?) or hash (#) characters.
   ///
   /// Example '/p/a/t/h'
   std::string pathname() const;
   void pathname(const std::string& value);

   /// Maps a string key to a list of values. It is typically used for query parameters
   /// and form values.
   std::string query(const std::string& key) const;
   void query(const std::string& key, const std::string& value);

   /// Consists of the "fragment" portion of the URL including the leading ASCII hash
   /// (#) character.
   ///
   /// Example: '#hash'
   std::string hash() const;
   void hash(const std::string& value);

   /// is the full URL string that was parsed with both the protocol and host components
   /// converted to lower-case.
   std::string href() const;

private:
   void parse(const std::string& value);

   UserInfo _userinfo;
   int _port;

   std::string _protocol;
   std::string _hostname;
   std::string _pathname;
   std::string _hash;

   std::map<std::string, std::string> _query;
};

} // namespace net
} // namespace orion
#endif // ORION_NET_URL_H
