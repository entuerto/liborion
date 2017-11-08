//
// net-example.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <codecvt>
#include <cstdio>
#include <iostream>
#include <locale>
#include <sstream>

#include <iphlpapi.h>
#include <winsock2.h>

std::string wstring_to_utf8(const std::wstring& str)
{
   std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
   return myconv.to_bytes(str);
}

int main()
{
   IP_ADAPTER_ADDRESSES* addresses = nullptr;

   std::cout << "Adapters Addresses\n";

   DWORD ret       = 0;
   ULONG outBufLen = 15000;

   int i = 0;
   do
   {
      addresses = (IP_ADAPTER_ADDRESSES*)HeapAlloc(GetProcessHeap(), 0, outBufLen);
      if (addresses == nullptr)
      {
         std::cout << "Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n";
         return EXIT_FAILURE;
      }

      ret =
         GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, addresses, &outBufLen);

      if (ret != ERROR_BUFFER_OVERFLOW)
         break;

      HeapFree(GetProcessHeap(), 0, addresses);
      addresses = nullptr;

      i++;
   } while ((ret == ERROR_BUFFER_OVERFLOW) and (i < 3));

   if (ret != NO_ERROR)
   {
      std::cout << "Call to GetAdaptersAddresses failed with error: " << ret << "\n";
      return EXIT_FAILURE;
   }

   // If successful, output some information from the data we received
   IP_ADAPTER_ADDRESSES* cur_addr = addresses;
   while (cur_addr)
   {
      std::cout << "Adapter name: " << cur_addr->AdapterName << "\n";

      std::cout << "  DNS Suffix:    " << wstring_to_utf8(cur_addr->DnsSuffix) << "\n"
                << "  Description:   " << wstring_to_utf8(cur_addr->Description) << "\n"
                << "  Friendly name: " << wstring_to_utf8(cur_addr->FriendlyName) << "\n"
                << "  Flags:         " << cur_addr->Flags << "\n"
                << "  Mtu:           " << cur_addr->Mtu << "\n"
                << "  IfType:        " << cur_addr->IfType << "\n"
                << "  OperStatus:    " << cur_addr->OperStatus << "\n";

      cur_addr = cur_addr->Next;
   }

   if (addresses)
      HeapFree(GetProcessHeap(), 0, addresses);

   return EXIT_SUCCESS;
}
