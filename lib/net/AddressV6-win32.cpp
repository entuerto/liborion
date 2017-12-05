//
// AddressV6-win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/AddressV6.h>

#include <orion/StringUtils.h>

#include <algorithm>
#include <cstring>
#include <system_error>

#include <mstcpip.h>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace orion
{
namespace net
{

std::string to_string(const AddressV6& addr)
{
   wchar_t buffer[256];

   RtlIpv6AddressToStringW(&addr._a, buffer);

   return wstring_to_utf8(buffer);
}

AddressV6 make_address_v6(const std::string& s)
{
   in6_addr addr{};
   uint32_t scope_id = 0;
   uint16_t port     = 0;

   // Get buffer size
   int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, nullptr, 0);

   wchar_t ws_buffer[len];

   MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, ws_buffer, len);

   RtlIpv6StringToAddressExW(ws_buffer, &addr, (PULONG)&scope_id, &port);

   auto ec = std::error_code(::GetLastError(), std::system_category());
   if (ec)
   {
      throw std::system_error(ec);
   }

   std::array<uint8_t, 16> b{};
   std::memcpy(b.data(), addr.s6_addr, 16);

   return AddressV6(b, scope_id);
}

} // namespace net
} // namespace orion
