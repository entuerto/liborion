//
// AddressV4-win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/AddressV4.h>

#include <UtilWin32.h>

#include <system_error>

#include <mstcpip.h>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace orion
{
namespace net
{

AddressV4::AddressV4(uint32_t value)
   : _a()
{
   _a.s_addr = htonl(value);
}

uint32_t AddressV4::to_ulong() const
{
   return ntohl(_a.s_addr);
}

std::string to_string(const AddressV4& addr)
{
   wchar_t buffer[256];

   RtlIpv4AddressToStringW(&addr._a, buffer);

   return win32::wstring_to_utf8(buffer);
}

AddressV4 make_address_v4(const std::string& value)
{
   in_addr addr{};
   uint16_t port = 0;

   std::wstring ws_value = win32::utf8_to_wstring(value);

   RtlIpv4StringToAddressExW(ws_value.c_str(), TRUE, &addr, &port);

   auto ec = std::error_code(::GetLastError(), std::system_category());
   if (ec)
   {
      throw std::system_error(ec);
   }

   return AddressV4({addr.s_net, addr.s_host, addr.s_lh, addr.s_impno});
}

} // namespace net
} // namespace orion
