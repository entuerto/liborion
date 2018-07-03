//
// COM.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_WIN32_COM_H
#define ORION_HOST_WIN32_COM_H

#include <orion/Orion-Stddefs.h>

#include <guiddef.h>
#include <objbase.h>

namespace orion
{
namespace win32
{

template<typename T>
class ComPtr
{
public:
   using InterfaceType = T;

   ComPtr() noexcept
      : _ptr(nullptr)
   { /* EMPTY. */
   }

   ~ComPtr() { internal_release(); }

   // No copy
   ComPtr(const ComPtr&) = delete;
   ComPtr& operator=(const ComPtr&) = delete;

   InterfaceType* get() const noexcept { return _ptr; }
   InterfaceType* operator->() const noexcept { return _ptr; }

   operator bool() const noexcept { return get() != nullptr; }

   InterfaceType* detach() noexcept
   {
      InterfaceType* ptr = _ptr;
      _ptr               = nullptr;
      return ptr;
   }

   void attach(InterfaceType* other) noexcept
   {
      if (_ptr != other)
      {
         internal_release();
         _ptr = other;
         internal_addref();
      }
   }

   unsigned long reset() { return internal_release(); }

   const InterfaceType** address_of() const noexcept { return &_ptr; }

   InterfaceType** address_of() noexcept { return &_ptr; }

   InterfaceType** release_and_address_of() noexcept
   {
      internal_release();
      return &_ptr;
   }

   template<typename U>
   HRESULT as(ComPtr<U>& p) const noexcept
   {
      return _ptr->QueryInterface(__uuidof(U),
                                  reinterpret_cast<void**>(p.release_and_address_of()));
   }
/*
   HRESULT as_iid(REFIID riid, ComPtr<IUnknown>& p) const noexcept
   {
      return _ptr->QueryInterface(riid, reinterpret_cast<void**>(p.release_and_address_of()));
   }
*/
private:
   void internal_addref() const noexcept
   {
      if (_ptr != nullptr)
         _ptr->AddRef();
   }

   unsigned long internal_release() noexcept
   {
      InterfaceType* tmp = _ptr;
      
      if (tmp == nullptr)
         return 0;

      _ptr = nullptr;
      return tmp->Release();
   }

private:
   InterfaceType* _ptr;
};

//-------------------------------------------------------------------------------------------------

class ComInitializer
{
public:
   ComInitializer() noexcept
      : _ok(false)
   {
      // COINIT_MULTITHREADED means that we must serialize access to the objects manually.
      // This is the fastest way to work. If user calls CoInitializeEx before us - we 
      // can end up with other mode (which is OK for us).
      //
      // If we call CoInitializeEx befire user - user may end up with different mode, which is a problem.
      // So we need to call that initialization function as late as possible.
      const auto res = ::CoInitializeEx(0, COINIT_MULTITHREADED);

      _ok = (res == S_OK or res == S_FALSE);
   }

   ~ComInitializer() noexcept
   {
      if (_ok) 
      {
         ::CoUninitialize();
      }
   }

private:
   bool _ok;
};

} // namespace win32
} // namespace orion

#endif // ORION_HOST_WIN32_COM_H
