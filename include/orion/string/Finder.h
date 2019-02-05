//
// Finder.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_FINDER_H
#define ORION_FINDER_H

#include <orion/Config.h>

#include <orion/Iterator.h>

#include <functional>
#include <iterator>
#include <string>
#include <utility>

namespace orion
{

//--------------------------------------------------------------------------------------------------
// Functors

class any_of
{
public:
   using Iterator = typename std::string::iterator;

   explicit any_of(std::string values)
      : _d(std::move(values))
   {
   }

   Range<Iterator> operator()(Iterator f, Iterator l) const
   {
      auto it = std::find_if(
         f, l, [&](std::string::value_type c) { return _d.find(c) != std::string::npos; });

      if (it == l)
      {
         return {l, l};
      }

      auto it2 = std::find_if(
         it, l, [&](std::string::value_type c) { return _d.find(c) == std::string::npos; });

      return {it, it2};
   }

private:
   std::string _d;
};

class by_char
{
public:
   using Iterator = typename std::string::iterator;

   explicit by_char(char d)
      : _d(d)
   {
   }

   Range<Iterator> operator()(Iterator f, Iterator l) const
   {
      auto it = std::find(f, l, _d);

      if (it == l)
      {
         return {l, l};
      }
      auto it2 = it;

      // Advance by one position
      ++it2;

      return {it, it2};
   }

private:
   char _d;
};

class by_string
{
public:
   using ValueType         = std::string;
   using IteratorType      = typename ValueType::iterator;
   using ConstIteratorType = typename ValueType::const_iterator;
   using PredicateType     = std::function<bool(ValueType::value_type, ValueType::value_type)>;

   explicit by_string(std::string pattern)
      : _pattern(std::move(pattern))
      , _predicate(is_equal())
   {
   }

   by_string(std::string pattern, PredicateType p)
      : _pattern(std::move(pattern))
      , _predicate(std::move(p))
   {
   }

   template<typename ForwardIteratorT>
   Range<ForwardIteratorT> operator()(ForwardIteratorT f, ForwardIteratorT l) const
   {
      return find(f, l, _pattern.begin(), _pattern.end(), _predicate);
   }

private:
   Range<IteratorType> find(IteratorType first1,
                            IteratorType last1,
                            ConstIteratorType pat_first,
                            ConstIteratorType pat_last,
                            const PredicateType& pred) const
   {
      if (pat_first == pat_last)
      {
         return make_range(first1, first1); // Everything matches an empty sequence
      }

      while (true)
      {
         // Find first element in sequence 1 that matchs *pat_first, with a mininum of loop checks
         while (true)
         {
            if (first1 == last1) // return last1 if no element matches *pat_first
            {
               return make_range(last1, last1);
            }
            if (pred(*first1, *pat_first))
            {
               break;
            }
            ++first1;
         }
         // *first1 matches *pat_first, now match elements after here
         IteratorType m1      = first1;
         ConstIteratorType m2 = pat_first;
         while (true)
         {
            if (++m2 == pat_last) // If pattern exhausted, first1 is the answer (works for 1
            {                     // element pattern)
               return make_range(first1, ++m1);
            }
            if (++m1 == last1) // Otherwise if source exhaused, pattern not found
            {
               return make_range(last1, last1);
            }
            if (not pred(*m1, *m2)) // if there is a mismatch, restart with a new first1
            {
               ++first1;
               break;
            } // else there is a match, check next elements
         }
      }
   }

private:
   std::string _pattern;
   PredicateType _predicate;
};

} // namespace orion
#endif // ORION_FINDER_H
