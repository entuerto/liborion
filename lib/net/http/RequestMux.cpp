//
// RequestMux.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http/RequestMux.h>

#include <orion/Exception.h>
#include <orion/Log.h>
#include <orion/String.h>
#include <orion/Throw.h>

#include <algorithm>

namespace orion
{
namespace net
{
namespace http
{

static bool path_match(const std::string& pattern, const std::string& path)
{
   if (pattern.back() != '/')
      return pattern == path;

   return starts_with(path, pattern);
}

static HandlerFunc default_handler(StatusCode status_code)
{
   return [status_code](const Request& req, Response& res) {
      
      res = Response(status_code);
      res.header(Field::Connection, "close");
      res.header(Field::XContentTypeOptions, "nosniff");

      return std::error_code();
   };
}

static HandlerFunc redirect_handler(StatusCode status_code, const std::string& path)
{
   return [status_code, path](const Request& req, Response& res) {
      res = Response(status_code);
      res.header(Field::Connection, "close");
      res.header(Field::XContentTypeOptions, "nosniff");

      return std::error_code();
   };
}

//--------------------------------------------------------------------------------------------------

RequestMux::RequestMux() = default;

RequestMux::~RequestMux() = default;

void RequestMux::handle(Method method, const std::string& pattern, HandlerFunc h)
{
   throw_if<ArgumentException>(pattern.empty(), "Pattern is empty", _src_loc);

   throw_if<ArgumentException>(not h, "Handler function not valid", _src_loc);

   auto it = _mux.find(pattern);

   throw_if<ArgumentException>((it != _mux.end() and it->second.user_defined),
                               "Multiple registrations for " + pattern,
                               _src_loc);

   // Helpful behavior:
   // If pattern is /tree/, insert an implicit permanent redirect for /tree.
   // It can be overridden by an explicit registration.
   if (pattern.size() >= 2 and pattern.back() == '/')
   {
      auto redirect_pattern = pattern.substr(0, pattern.size() - 1);

      it = _mux.find(redirect_pattern);

      if (it == _mux.end() or not it->second.user_defined)
      {
         // If pattern contains a host name, strip it and use remaining
         // path for redirect.
         std::string path = (pattern[0] == '/') ? pattern : pattern.substr(pattern.find('/'));

         if (it == _mux.end())
         {
            Entry entry{false, pattern};

            entry.handlers.emplace(method, redirect_handler(StatusCode::MovedPermanently, path));

            _mux.emplace(std::move(redirect_pattern), std::move(entry));
         }
         else
         {
            Entry entry{false, pattern};

            entry.handlers.emplace(method, redirect_handler(StatusCode::MovedPermanently, path));

            it->second = entry;
         }
      }
   }

   Entry entry{false, pattern};

   entry.handlers.emplace(method, std::move(h));

   _mux.emplace(pattern, std::move(entry));
}

void RequestMux::handle(StatusCode status_code, HandlerFunc h)
{
   throw_if<ArgumentException>(not h, "Handler function not valid", _src_loc);

   _status_handlers.emplace(status_code, std::move(h));
}

HandlerFunc RequestMux::handler(const Request& req) const
{
   auto path = req.url().pathname();

   //if (req.method() != Method::CONNECT)
   //{
   //
   //}

   // Host-specific pattern takes precedence over generic ones
   auto host = req.url().hostname();

   auto h = match(req.method(), host + path);
   if (h)
      return h;

   h = match(req.method(), path);
   if (h)
      return h;

   return status_handler(StatusCode::NotFound);
}

HandlerFunc RequestMux::match(Method method, const std::string& path) const
{
   std::size_t best        = 0;
   const Entry* best_entry = nullptr;

   for (auto& item : _mux)
   {
      auto& entry = item.second;

      if (not path_match(entry.pattern, path))
         continue;

      if (best_entry == nullptr and best < entry.pattern.size())
      {
         best       = entry.pattern.size();
         best_entry = &entry;
      }
   }

   if (best_entry != nullptr)
   {
      auto it = best_entry->handlers.find(method);
      return (it != std::end(best_entry->handlers)) ? it->second : HandlerFunc();
   }

   return HandlerFunc();
}

std::error_code RequestMux::operator()(const Request& req, Response& res)
{
   auto h = handler(req);

   return h(req, res);
}

HandlerFunc RequestMux::status_handler(StatusCode status_code) const
{
   auto it = _status_handlers.find(status_code);

   if (it == _status_handlers.end())
      return default_handler(status_code);

   return it->second;
}

} // namespace http
} // namespace net
} // namespace orion
