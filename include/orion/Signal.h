// Signal.h
//
// Copyright 2014 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#ifndef ORION_SIGNAL_H
#define ORION_SIGNAL_H

#include <orion/Orion-Stddefs.h>

#include <functional>
#include <memory>
#include <vector>

namespace orion
{
/*
   Declare Signal as a class template.  It will be specialized
   later for all number of arguments.
 */
template<typename Signature>
class Signal;

/*
   This is a simple signal class.

   Example:

      Signal<void(int)> signal;

      signal.connect(print_num);
      signal.connect(std::bind(&Object::print, o, std::placeholders::_1));
      signal.connect(&o, &Object::print);
      signal.emit(7);
 */
template<typename R, class... Args>
class Signal<R(Args...)>
{
public:
   typedef std::function<R(Args...)> Slot;

   Signal() {}

   ~Signal() { _slots.clear(); }

   void connect(Slot slot) { _slots.push_back(slot); }

   template<typename T>
   void connect(T* obj, void (T::*func)(Args...))
   {
      _slots.push_back(std::bind(func, obj, std::placeholders::_1));
   }

   template<typename T>
   void connect(std::shared_ptr<T> obj, void (T::*func)(Args...))
   {
      _slots.push_back(std::bind(func, obj, std::placeholders::_1));
   }

   void emit(Args... args) const
   {
      for (const Slot& s : _slots)
      {
         s(args...);
      }
   }

   void emit() const
   {
      for (const Slot& s : _slots)
      {
         s();
      }
   }

private:
   std::vector<Slot> _slots;
};

/*
   Specialisation class for void return type
 */
template<>
class Signal<void()>
{
public:
   typedef std::function<void()> Slot;

   Signal() {}

   ~Signal() { _slots.clear(); }

   void connect(Slot slot) { _slots.push_back(slot); }

   template<typename T>
   void connect(T* obj, void (T::*func)())
   {
      _slots.push_back(std::bind(func, obj));
   }

   template<typename T>
   void connect(std::shared_ptr<T> obj, void (T::*func)())
   {
      _slots.push_back(std::bind(func, obj));
   }

   void emit() const
   {
      for (const Slot& s : _slots)
      {
         s();
      }
   }

private:
   std::vector<Slot> _slots;
};

} // namespace orion
#endif /* ORION_SIGNAL_H */
