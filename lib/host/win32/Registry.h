//
// Registry.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_WIN32_REGISTRY_H
#define ORION_HOST_WIN32_REGISTRY_H

#include <orion/String.h>

#include <host/win32/Error.h>
#include <host/win32/String.h>

#include <system_error>

#include <Windows.h>

namespace orion
{
namespace win32
{
//
// Windows defines some predefined root keys that are always open.
//
// HKEY_CLASSES_ROOT
// HKEY_CURRENT_USER
// HKEY_LOCAL_MACHINE
// HKEY_USERS
// HKEY_CURRENT_CONFIG

//
// Key access rights
//
// KEY_ALL_ACCESS
// KEY_CREATE_LINK
// KEY_CREATE_SUB_KEY
// KEY_ENUMERATE_SUB_KEYS
// KEY_EXECUTE
// KEY_NOTIFY
// KEY_QUERY_VALUE
// KEY_READ
// KEY_SET_VALUE
// KEY_WOW64_32KEY
// KEY_WOW64_64KEY
// KEY_WRITE

class Registry
{
public:
   Registry()
      : _subkey(nullptr)
   {
   }

   ~Registry() 
   { 
      close_key(); 
   }

   std::error_code create_key(const std::string& path, REGSAM access, HKEY& new_subkey)
   {
      DWORD disposition          = 0;
      std::wstring key_to_create = utf8_to_wstring(path);

      uint32_t ret = RegCreateKeyExW(_subkey,
                                     key_to_create.c_str(),
                                     0,
                                     nullptr,
                                     REG_OPTION_NON_VOLATILE,
                                     access,
                                     nullptr,
                                     &new_subkey,
                                     &disposition);

      if (ret != ERROR_SUCCESS)
      {
         return win32::make_error_code(ret);
      }

      return std::error_code();
   }

   /// open_key opens a new key with path relative to key.
   /// It accepts any open key, including HKEY_CURRENT_USER and others.
   /// The access parameter specifies desired access rights to the key to be opened.
   std::error_code open_key(HKEY key, const std::string& path, REGSAM access)
   {
      std::wstring key_to_open = utf8_to_wstring(path);

      uint32_t ret = RegOpenKeyExW(HKEY_LOCAL_MACHINE, key_to_open.c_str(), 0, KEY_READ, &_subkey);

      if (ret != ERROR_SUCCESS)
      {
         return win32::make_error_code(ret);
      }

      return std::error_code();
   }

   /// Deletes the subkey path of the open subkey and its values.
   std::error_code delete_key(const std::string& path)
   {
      std::wstring key_to_delete = utf8_to_wstring(path);

      uint32_t ret = RegDeleteKeyW(_subkey, key_to_delete.c_str());

      if (ret != ERROR_SUCCESS)
      {
         return win32::make_error_code(ret);
      }

      return std::error_code();
   }

   // Close closes open registry key.
   std::error_code close_key()
   {
      uint32_t ret = RegCloseKey(_subkey);

      if (ret != ERROR_SUCCESS)
      {
         return win32::make_error_code(ret);
      }

      return std::error_code();
   }

   // std::error_code set_value();

   // std::error_code enum_value();

   /// Queries the value of a subkey name.
   /// value must reserve storage to retreive data.
   std::error_code query_value(const std::string& name, std::wstring& value)
   {
      std::wstring value_name = utf8_to_wstring(name);

      DWORD type   = REG_SZ;
      DWORD length = value.capacity();

      LPBYTE data = reinterpret_cast<LPBYTE>(const_cast<wchar_t*>(value.data()));

      uint32_t ret = RegQueryValueExW(_subkey, value_name.c_str(), nullptr, &type, data, &length);

      if (ret != ERROR_SUCCESS)
      {
         return win32::make_error_code(ret);
      }

      return std::error_code();
   }

   /// Queries the value of a subkey name.
   /// value must reserve storage to retreive data.
   std::error_code query_value(const std::string& name, uint32_t& value)
   {
      std::wstring value_name = utf8_to_wstring(name);

      DWORD type   = REG_DWORD;
      DWORD length = sizeof(value);

      LPBYTE data = reinterpret_cast<LPBYTE>(&value);

      uint32_t ret = RegQueryValueExW(_subkey, value_name.c_str(), nullptr, &type, data, &length);

      if (ret != ERROR_SUCCESS or type != REG_DWORD)
      {
         return win32::make_error_code(ret);
      }

      return std::error_code();
   }

   /// Queries the value of a subkey name.
   /// value must reserve storage to retreive data.
   std::error_code query_value(const std::string& name, uint64_t& value)
   {
      std::wstring value_name = utf8_to_wstring(name);

      DWORD type   = REG_QWORD;
      DWORD length = sizeof(value);

      LPBYTE data = reinterpret_cast<LPBYTE>(&value);

      uint32_t ret = RegQueryValueExW(_subkey, value_name.c_str(), nullptr, &type, data, &length);

      if (ret != ERROR_SUCCESS or type != REG_QWORD)
      {
         return win32::make_error_code(ret);
      }

      return std::error_code();
   }

   // delete_value()

   // load_mui_string();

   // ExpandString expands environment-variable strings and replaces
   // them with the values defined for the current user.
   // Use ExpandString to expand EXPAND_SZ strings.
   std::error_code expand_string(const std::wstring& value, std::wstring& result)
   {
      if (value.empty())
         return std::error_code();

      result.reserve(value.capacity() * 2);
   
      for (;;) 
      {
         wchar_t* data = const_cast<wchar_t*>(value.data());

         auto char_count = ExpandEnvironmentStringsW(value.c_str(), data, result.capacity());
      
         // Error
         if (char_count == 0)
            return win32::make_error_code(GetLastError());

         // Success
         if (char_count <= result.size())
            return std::error_code();
         
         // More buffer
         result.reserve(char_count);
      }

      return std::error_code();
   }

   std::error_code query_info_key()
   {
#if 0
      struct KeyInfo  
      {
         uint32_t SubKeyCount     
         uint32_t MaxSubKeyLen     // size of the key's subkey with the longest name
         uint32_t ValueCount      
         uint32_t MaxValueNameLen  // size of the key's longest value name
         uint32_t MaxValueLen      // longest data component among the key's values, in bytes
         lastWriteTime   
      }

      uint32_t ret = RegQueryInfoKey(
         _subkey,
         nullptr /* lpClass */,
         nullptr /* lpcClass */,
         nullptr /* lpReserved */,
         _Out_opt_   LPDWORD   lpcSubKeys,
         _Out_opt_   LPDWORD   lpcMaxSubKeyLen,
         _Out_opt_   LPDWORD   lpcMaxClassLen,
         _Out_opt_   LPDWORD   lpcValues,
         _Out_opt_   LPDWORD   lpcMaxValueNameLen,
         _Out_opt_   LPDWORD   lpcMaxValueLen,
         _Out_opt_   LPDWORD   lpcbSecurityDescriptor,
         _Out_opt_   PFILETIME lpftLastWriteTime
      );

      if (ret != ERROR_SUCCESS)
      {
         return win32::make_error_code(ret);
      }
#endif
      return std::error_code();
   }

private:
   HKEY _subkey;
};

} // namespace win32
} // namespace orion

#endif // ORION_HOST_WIN32_REGISTRY_H
