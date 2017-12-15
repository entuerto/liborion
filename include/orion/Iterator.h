//
// Iterator.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_ITERATOR_H
#define ORION_ITERATOR_H

#include <orion/Orion-Stddefs.h>

#include <iterator>
#include <utility>

namespace orion
{

template<class IteratorT>
class Range
{
public:
   // These are provides to fulfill container interface
   using iterator       = IteratorT;
   using const_iterator = IteratorT;

   Range() = default;

   Range(IteratorT b, IteratorT e)
      : _begin(std::move(b))
      , _end(std::move(e))
   {
   }

   constexpr IteratorT begin() const { return _begin; }

   constexpr IteratorT end() const { return _end; }

   constexpr bool empty() const { return _begin == _end; }

private:
   IteratorT _begin;
   IteratorT _end;
};

template<class IteratorT>
inline bool operator==(const Range<IteratorT>& r1, const Range<IteratorT>& r2)
{
   return r1.begin() == r2.begin() and r1.end() == r2.end();
}

template<class IteratorT>
inline bool operator!=(const Range<IteratorT>& r1, const Range<IteratorT>& r2)
{
   return not(r1 == r2);
}

template<typename IteratorT>
inline Range<IteratorT> make_range(IteratorT begin, IteratorT end)
{
   return {begin, end};
}

template<class Container>
inline auto make_range(Container& c) -> decltype(c.begin())
{
   return {std::begin(c), std::end(c)};
}

//-------------------------------------------------------------------------------------------------

class IteratorOp
{
public:
   template<class T>
   static void increment(T& v)
   {
      v.increment();
   }

   template<class T>
   static typename T::reference value(const T& v)
   {
      return v.value();
   }
};

//-------------------------------------------------------------------------------------------------

template<class Derived,
         class Value,
         class Pointer    = Value*,
         class Reference  = Value&,
         class Difference = std::ptrdiff_t>
class InputIterator
{
public:
   using iterator_category = std::input_iterator_tag;
   using value_type        = Value;
   using pointer           = Pointer;
   using reference         = Reference;
   using difference_type   = Difference;

   reference operator*() const { return IteratorOp::value(this->derived()); }

   pointer operator->() const { return std::addressof(IteratorOp::value(this->derived())); }

   Derived& operator++()
   {
      IteratorOp::increment(this->derived());
      return this->derived();
   }

protected:
   Derived& derived() { return *static_cast<Derived*>(this); }

   const Derived& derived() const { return *static_cast<Derived const*>(this); }
};

} // namespace orion
#endif // ORION_ITERATOR_H
