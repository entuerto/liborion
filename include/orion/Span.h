//
// Span.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_SPAN_H
#define ORION_SPAN_H

#include <orion/Orion-Stddefs.h>

//#include <orion/Utils.h>

#include <cstddef>

namespace orion
{
#if 0
template <class ElementType>
class Span;

// implementation details
namespace details
{

template<class T>
struct is_span_oracle : std::false_type
{
};

template<class ElementType>
struct is_span_oracle<Span<ElementType>> : std::true_type
{
};

template<class T>
struct is_span : public is_span_oracle<std::remove_cv_t<T>>
{
};

template<class T>
struct is_std_array_oracle : std::false_type
{
};

template<class ElementType, std::size_t Extent>
struct is_std_array_oracle<std::array<ElementType, Extent>> : std::true_type
{
};

template<class T>
struct is_std_array : public is_std_array_oracle<std::remove_cv_t<T>>
{
};

} // namespace details

//-------------------------------------------------------------------------------------------------
//
// Span<> - A view of contiguous T's, (T*, len).
//
template<class T>
class Span
{
   template<class U>
   friend class Span;

public:
   // constants and types
   using index_type = std::ptrdiff_t;

   using element_type = T;
   using value_type = std::remove_cv_t<T>;

   using reference = T&;
   using pointer = T*;
   using const_pointer = const T*;
   using const_reference = const T&;

   using iterator = pointer;
   using const_iterator = const_pointer;

   using reverse_iterator = std::reverse_iterator<iterator>;
   using const_reverse_iterator = std::reverse_iterator<const_iterator>;

   using difference_type = typename std::iterator_traits<iterator>::difference_type;

   // 26.7.3.2 Constructors, copy, and assignment [span.cons]

   constexpr Span() noexcept
      : _first(nullptr)
      , _last(nullptr)
   {
   }

   //constexpr span(nullptr_t);

   constexpr Span(pointer ptr, index_type count)
      : _first(ptr)
      , _last(ptr + count)
   {
      Expects(count == 0 or (count > 0 and ptr != nullptr));
   }

   constexpr Span(pointer first_elem, pointer last_elem)
      : _first(first_elem)
      , _last(last_elem)
   {
      Expects(first_elem <= last_elem);
   }

   template<std::size_t N>
   constexpr explicit Span(element_type (&arr)[N]) noexcept
      : _first(std::addressof(arr[0]))    
      , _last(std::addressof(arr[0]) + N)
   {
   }

   template<std::size_t N, class = std::enable_if_t<(N > 0)>>
   constexpr explicit Span(std::array<std::remove_const_t<element_type>, N>& arr) noexcept
      : _first(arr.data())    
      , _last(arr.data() + N)
   {
   }

   constexpr explicit Span(std::array<std::remove_const_t<element_type>, 0>& /*unused*/) noexcept
      : _first(nullptr)    
      , _last(nullptr)
   {
   }

   template<std::size_t N, class = std::enable_if_t<(N > 0)>>
   constexpr explicit Span(const std::array<std::remove_const_t<element_type>, N>& arr) noexcept
      : _first(arr.data())    
      , _last(arr.data() + N)
   {
   }

   constexpr explicit Span(const std::array<std::remove_const_t<element_type>, 0>& /*unused*/) noexcept
      : _first(nullptr)    
      , _last(nullptr)
   {
   }

   template<class Container,
            class = std::enable_if_t<
               !details::is_span<Container>::value && !details::is_std_array<Container>::value &&
               std::is_convertible<typename Container::pointer, pointer>::value &&
               std::is_convertible<typename Container::pointer,
                                   decltype(std::declval<Container>().data())>::value>>
   constexpr explicit Span(Container& cont)
      : Span(cont.data(), narrow<index_type>(cont.size()))
   {
   }

   template<class Container,
            class = std::enable_if_t<
               std::is_const<element_type>::value && !details::is_span<Container>::value &&
               std::is_convertible<typename Container::pointer, pointer>::value &&
               std::is_convertible<typename Container::pointer,
                                   decltype(std::declval<Container>().data())>::value>>
   constexpr explicit Span(const Container& cont)
      : Span(cont.data(), narrow<index_type>(cont.size()))
   {
   }

   constexpr Span(const Span&)     = default;
   constexpr Span(Span&&) noexcept = default;

   template<class U,
            class = typename std::enable_if<
               std::is_convertible<U (*)[], element_type (*)[]>::value>::type>
   constexpr explicit Span(const Span<U>& other)
      : _first(other.begin())
      , _last(other.end())
   {
   }

   ~Span() = default;

   constexpr Span& operator=(const Span&) noexcept = default;
   constexpr Span& operator=(Span&&) noexcept = default;

   //
   // 26.7.3.3 Subviews [span.sub]
   //

   constexpr Span first(index_type count) const noexcept
   {
      Expects(0 <= count and count <= this->size());
      return {this->data(), count};
   }

   constexpr Span last(index_type count) const noexcept
   {
      Expects(0 <= count and count <= this->size());
      return {this->data() + this->size() - count, count};
   }

   constexpr Span subspan(index_type offset) const noexcept
   {
      Expects(0 <= offset and offset <= this->size());
      return {this->data() + offset, this->size() - offset};
   }

   constexpr Span subspan(index_type offset, index_type count) const noexcept
   {
      Expects(0 <= offset and offset <= this->size() and 0 <= count and
              count <= this->size() - offset);
      return {this->data() + offset, count};
   }

   //
   // 26.7.3.4 Observers [span.obs]
   //

   constexpr index_type size() const noexcept { return narrow_cast<index_type>(_last - _first); }

   constexpr index_type size_bytes() const noexcept
   {
      return size() * narrow_cast<index_type>(sizeof(element_type));
   }

   constexpr bool empty() const noexcept { return size() == 0; }

   //
   // 26.7.3.5 Element access [span.elem]
   //

   constexpr reference operator[](index_type index) const { return at(index); }

   constexpr reference operator()(index_type index) const { return at(index); }

   constexpr reference at(index_type index) const
   {
      Expects(index < size());
      return _first[index];
   }

   constexpr pointer data() const noexcept { return _first; }

   //
   // 26.7.3.6 Iterator support [span.iterators]
   //

   constexpr iterator begin() const noexcept { return iterator(_first); }

   constexpr iterator end() const noexcept { return iterator(_last); }

   constexpr const_iterator cbegin() const noexcept { return {begin()}; }

   constexpr const_iterator cend() const noexcept { return {end()}; }

   constexpr reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }

   constexpr reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }

   constexpr const_reverse_iterator crbegin() const noexcept
   {
      return const_reverse_iterator{cend()};
   }

   constexpr const_reverse_iterator crend() const noexcept
   {
      return const_reverse_iterator{cbegin()};
   }

   void swap(Span& other) noexcept
   {
      using std::swap;
      swap(_first, other._first);
      swap(_last, other._last);
   }

   template<class U>
   Span<U> as_span() const noexcept
   {
      Expects((this->size_bytes() % sizeof(U)) == 0);
      return Span<U>(reinterpret_cast<U*>(this->data()), this->size_bytes() / sizeof(U)); // NOLINT
   }

private:
   pointer _first;
   pointer _last;
};

// [span.comparison], span comparison operators

template<class T>
constexpr bool operator==(Span<T> lhs, Span<T> rhs)
{
   return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T>
constexpr bool operator!=(Span<T> lhs, Span<T> rhs)
{
   return not(lhs == rhs);
}

template<class T>
constexpr bool operator<(Span<T> lhs, Span<T> rhs)
{
   return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T>
constexpr bool operator<=(Span<T> lhs, Span<T> rhs)
{
   return not(lhs > rhs);
}

template<class T>
constexpr bool operator>(Span<T> lhs, Span<T> rhs)
{
   return rhs < lhs;
}

template<class T>
constexpr bool operator>=(Span<T> lhs, Span<T> rhs)
{
   return not(lhs < rhs);
}

//--------------------------------------------------------------------------------------------------
//
// make_span() - Utility functions for creating spans
//

template<class T>
constexpr Span<T> make_span(T* ptr, typename Span<T>::index_type count)
{
   return Span<T>{ptr, count};
}

template<class T>
constexpr Span<T> make_span(T* first_elem, T* last_elem)
{
   return Span<T>{first_elem, last_elem};
}

template<class T, std::size_t N>
constexpr Span<T> make_span(T (&arr)[N]) noexcept
{
   return Span<T>{arr, N};
}

template<class Container>
constexpr Span<typename Container::value_type> make_span(Container& cont)
{
   return Span<typename Container::value_type>{cont};
}

template<class Container>
constexpr Span<const typename Container::value_type> make_span(const Container& cont)
{
   return Span<const typename Container::value_type>{cont};
}

template<class Ptr>
constexpr Span<typename Ptr::element_type> make_span(Ptr& cont, std::ptrdiff_t count)
{
   return Span<typename Ptr::element_type>{cont, count};
}

template<class Ptr>
constexpr Span<typename Ptr::element_type> make_span(Ptr& cont)
{
   return Span<typename Ptr::element_type>{cont};
}

//-------------------------------------------------------------------------------------------------
// Specialization of gsl::at for span

template<class T>
constexpr T& at(Span<T> s, std::ptrdiff_t i)
{
   // No bounds checking here because it is done in Span::operator[] 
   return s[i];
}
#endif
} // namespace orion

#endif // ORION_SPAN_H
