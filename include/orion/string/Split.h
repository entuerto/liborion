//
// Split.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_SPLIT_H
#define ORION_SPLIT_H

#include <orion/Orion-Stddefs.h>

#include <orion/Iterator.h>

#include <functional>
#include <iterator>
#include <utility>
#include <vector>

namespace orion
{

template<typename StringT, typename FinderT>
class SplitIterator : public InputIterator<SplitIterator<StringT, FinderT>, const StringT>
{
public:
   friend class IteratorOp;

   using BaseType      = InputIterator<SplitIterator<StringT, FinderT>, const StringT>;
   using ValueType     = StringT;
   using ValueIterator = typename BaseType::value_type::iterator;

   /// Default constructor
   SplitIterator()
      : _eof(true)
   {
   }

   /// Construct a SplitIterator for a given range and finder.
   SplitIterator(ValueIterator begin, ValueIterator end, FinderT finder)
      : _pos(std::move(begin))
      , _end(std::move(end))
      , _finder(std::move(finder))
      , _eof(false)
   {
      increment();
   }

   /// Copy constructor
   SplitIterator(const SplitIterator& rhs)
      : BaseType(rhs)
      , _pos(rhs._pos)
      , _end(rhs._end)
      , _current(rhs._current)
      , _finder(rhs._finder)
      , _eof(rhs._eof)
   {
   }

   /// Destructor
   ~SplitIterator() = default;

   bool eof() const { return not _finder or _eof; }

   /// Copy assignable
   SplitIterator& operator=(const SplitIterator& rhs) = default;

   friend bool operator==(const SplitIterator& si1, const SplitIterator& si2)
   {
      return si1.eof() or si2.eof()
                ? si1.eof() == si2.eof()
                : (si1._current == si2._current and si1._pos == si2._pos and si1._end == si2._end);
   }

   friend bool operator!=(const SplitIterator& si1, const SplitIterator& si2)
   {
      return not(si1 == si2);
   }

private:
   const ValueType& value() const { return _current; }

   void increment()
   {
      auto match = _finder(_pos, _end);

      if (match.begin() == _end and match.end() == _end)
      {
         _eof = (_pos == _end);
      }

      _current = ValueType{_pos, match.begin()};

      _pos = match.end();
   }

private:
   ValueIterator _pos;
   ValueIterator _end;

   ValueType _current;

   FinderT _finder;

   bool _eof = true;
};

template<typename StrintT>
class Split
{
public:
   using ValueType     = StrintT;
   using ValueIterator = typename ValueType::iterator;

   using FinderType     = std::function<Range<ValueIterator>(ValueIterator, ValueIterator)>;
   using iterator       = SplitIterator<ValueType, FinderType>;
   using const_iterator = SplitIterator<ValueType, FinderType>;

   explicit Split(FinderType f)
      : _finder(std::move(f))
   {
   }

   Split(ValueType text, FinderType f)
      : _text(std::move(text))
      , _finder(std::move(f))
   {
   }

   //
   // Range functions that iterate the split substrings.
   // These methods enable a Split object to be used in a range-based for loop.
   //
   iterator begin() { return {_text.begin(), _text.end(), _finder}; }
   iterator end() { return {}; }

   Split& operator|(const ValueType& text)
   {
      _text = text;
      return *this;
   }

   // An implicit conversion operator that is restricted to only those containers
   // that constructs with first, last InputIterators
   template<typename Container>
   operator Container()
   {
      return Container{begin(), end()};
   }

private:
   ValueType _text;
   FinderType _finder;
};

template<typename FinderT>
class MaxSplits
{
public:
   using Iterator = typename std::string::iterator;

   MaxSplits(FinderT finder, int limit)
      : _finder(std::move(finder))
      , _limit(limit)
      , _count(0)
   {
   }

   Range<Iterator> operator()(Iterator f, Iterator l) const
   {
      _count++;

      if (_count >= _limit)
         return {l, l};

      return _finder(f, l);
   }

private:
   FinderT _finder;
   int _limit;

   mutable int _count;
};

//-------------------------------------------------------------------------------------------------

/// Splits a string into various substrings.
template<typename StrintT, typename FinderT>
inline Split<StrintT> split(const StrintT& text, FinderT f)
{
   return Split<StrintT>{text, std::move(f)};
}

/// Splits a string into various substrings.
template<typename FinderT>
inline Split<std::string> split(const std::string& text, FinderT f)
{
   return Split<std::string>{text, std::move(f)};
}

template<typename StrintT, typename FinderT>
inline Split<StrintT> split(FinderT f)
{
   return Split<StrintT>{std::move(f)};
}

template<typename FinderT>
inline Split<std::string> split(FinderT f)
{
   return Split<std::string>{std::move(f)};
}

//-------------------------------------------------------------------------------------------------

/// Splits a string into various substrings.
template<typename StrintT, typename FinderT>
inline Split<StrintT> split_max(const StrintT& text, FinderT f, int limit)
{
   return Split<StrintT>{text, MaxSplits<FinderT>{std::move(f), limit}};
}

/// Splits a string into various substrings.
template<typename FinderT>
inline Split<std::string> split_max(const std::string& text, FinderT f, int limit)
{
   return Split<std::string>{text, MaxSplits<FinderT>{std::move(f), limit}};
}

template<typename StrintT, typename FinderT>
inline Split<StrintT> split_max(FinderT f, int limit)
{
   return Split<StrintT>{MaxSplits<FinderT>{std::move(f), limit}};
}

template<typename FinderT>
inline Split<std::string> split_max(FinderT f, int limit)
{
   return Split<std::string>{MaxSplits<FinderT>{std::move(f), limit}};
}

} // namespace orion
#endif // ORION_SPLIT_H
