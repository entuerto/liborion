//
// Common.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_COMMON_H
#define ORION_COMMON_H

#include <orion/Config.h>
#include <orion/Macros.h>

#include <exception>
#include <string>
#include <string_view>

#ifndef NO_COPY
#define NO_COPY(klass) \
   klass(const klass&) = delete; \
   klass& operator=(const klass&) = delete;
#endif

#ifndef NO_MOVE
#define NO_MOVE(klass) \
   klass(klass&&) noexcept = delete; \
   klass& operator=(klass&&) noexcept = delete;   
#endif

#ifndef DEFAULT_COPY
#define DEFAULT_COPY(klass) \
   klass(const klass&) = default; \
   klass& operator=(const klass&) = default;
#endif

#ifndef DEFAULT_MOVE
#define DEFAULT_MOVE(klass) \
   klass(klass&&) = default; \
   klass& operator=(klass&&) = default;   
#endif

namespace orion
{
//-------------------------------------------------------------------------------------------------
// Forward declarations

template <class ElementType>
class Span;

//-------------------------------------------------------------------------------------------------

struct NonCopyable
{
   NonCopyable() = default;
   ~NonCopyable() = default;
   
   NonCopyable(NonCopyable const&) = delete;
   NonCopyable(NonCopyable&&) = delete;

   NonCopyable& operator=(NonCopyable const&) = delete;
   NonCopyable& operator=(NonCopyable&&) = delete;
};

//-------------------------------------------------------------------------------------------------

template<typename... Ts>
inline constexpr void ignore_unused(const Ts&...)
{
}

template<typename... Ts>
inline constexpr void ignore_unused()
{
}

//-------------------------------------------------------------------------------------------------
// -- Source file location 

#define DbgSrcLoc \
   SourceLocation{__FILE__, __LINE__}

class SourceLocation
{
public:
   SourceLocation() = default;
   
   SourceLocation(std::string_view fn, int ln);
   SourceLocation(std::string_view fn, int ln, std::string_view fun);

   const std::string& file_name() const;

   constexpr int line_number() const;

   const std::string& function_name() const;

private:
   std::string _file_name{"(unknown)"};
   int _line_number{0};
   std::string _function_name{"(unknown)"};
};

//-------------------------------------------------------------------------------------------------
//  Base exception

/// Generic exception
///
/// Generic exception that all library exceptions inherits.
///
class Exception : public std::exception
{
public:
   explicit Exception(std::string text);
   Exception(std::string text, SourceLocation src_loc);

   Exception(const Exception&) = default;
   Exception(Exception&&) noexcept = default;

   ~Exception() override = default;

   Exception& operator=(const Exception&) = default;
   Exception& operator=(Exception&&) noexcept = default;

   //! Returns the recorded source location
   const SourceLocation& source_location() const;

   const char* what() const noexcept override;

private:
   std::string _text;

   SourceLocation _src_location;
};

// ------------------------------------------------------------------------------------------------
// -- InvalidArgumentError 

/// Invalid argument error
///
/// The error is thrown when one of the arguments provided to a method is not valid.
///
class InvalidArgumentError : public Exception
{
public:
   explicit InvalidArgumentError(std::string text)
      : Exception(std::move(text))
   {
   }
   InvalidArgumentError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

// ------------------------------------------------------------------------------------------------
// -- NullArgumentError

/// Null argument error
///
/// The error is thrown when a null pointer is passed to a method that does not accept 
/// it as a valid argument.
///
class NullArgumentError : public Exception
{
public:
   explicit NullArgumentError(std::string text)
      : Exception(std::move(text))
   {
   }
   NullArgumentError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

// ------------------------------------------------------------------------------------------------
// -- OutOfRangeError 

/// Out of range error
///
/// It reports errors that are consequence of attempt to access elements out of defined range.
///
class OutOfRangeError : public Exception
{
public:
   explicit OutOfRangeError(std::string text)
      : Exception(std::move(text))
   {
   }
   OutOfRangeError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

// ------------------------------------------------------------------------------------------------
// -- NotImplementedError 

/// Not implemented error
///
/// The error is thrown when a requested method or operation is not implemented.
///
class NotImplementedError : public Exception
{
public:
   explicit NotImplementedError(std::string text)
      : Exception(std::move(text))
   {
   }
   NotImplementedError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

// ------------------------------------------------------------------------------------------------
// -- SymbolNotFoundError 

/// Symbol not found error
///
/// This error is thrown when we cannot find a symbol in a module.
///
class SymbolNotFoundError : public Exception
{
public:
   explicit SymbolNotFoundError(std::string text)
      : Exception(std::move(text))
   {
   }
   SymbolNotFoundError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

//-------------------------------------------------------------------------------------------------
// -- Throw functions

template<typename E>
[[noreturn]] void throw_exception(const std::string& msg)
{
   throw E(msg);
}

template<typename E>
[[noreturn]] void throw_exception(const std::string& msg, const SourceLocation& src_loc)
{
   throw E(msg, src_loc);
}

template<typename E>
void throw_if(bool condition,
              const std::string& msg,
              const SourceLocation& src_loc = SourceLocation{})
{
   if (condition)
   {
      throw_exception<E>(msg, src_loc);
   }
}

template<typename E, typename T>
void throw_if_null(const T* value,
                   const std::string& msg,
                   const SourceLocation& src_loc = SourceLocation{})
{
   if (value == nullptr)
   {
      throw_exception<E>(msg, src_loc);
   }
}

//-------------------------------------------------------------------------------------------------

//
// There are three configuration options for this GSL implementation's behavior
// when pre/post conditions on the GSL types are violated:
//
// 1. ORION_TERMINATE_ON_CONTRACT_VIOLATION: std::terminate will be called (default)
// 2. ORION_THROW_ON_CONTRACT_VIOLATION: an exception will be thrown
// 3. ORION_UNENFORCED_ON_CONTRACT_VIOLATION: nothing happens
//
#if !(defined(ORION_THROW_ON_CONTRACT_VIOLATION) ||     \
      defined(ORION_TERMINATE_ON_CONTRACT_VIOLATION) || \
      defined(ORION_UNENFORCED_ON_CONTRACT_VIOLATION))
#   define ORION_THROW_ON_CONTRACT_VIOLATION
#endif

#if !(defined(ORION_THROW_ON_ASSERT_VIOLATION) ||     \
      defined(ORION_TERMINATE_ON_ASSERT_VIOLATION) || \
      defined(ORION_UNENFORCED_ON_ASSERT_VIOLATION))
#   define ORION_THROW_ON_ASSERT_VIOLATION
#endif

//--------------------------------------------------------------------------------------------------
// Expects
// Ensures

class FailFast : public orion::Exception
{
public:
   FailFast(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

#if defined(ORION_TERMINATE_ON_CONTRACT_VIOLATION)

#   define ORION_CONTRACT_CHECK(type, cond) \
      (ORION_LIKELY(cond) ? static_cast<void>(0) : std::terminate())

#elif defined(ORION_THROW_ON_CONTRACT_VIOLATION)

#   define ORION_CONTRACT_CHECK(type, cond) \
      (ORION_LIKELY(cond) ? static_cast<void>(0) : throw FailFast(type, DbgSrcLoc))

#elif defined(ORION_UNENFORCED_ON_CONTRACT_VIOLATION)

#   define ORION_CONTRACT_CHECK(type, cond) ORION_ASSUME(cond)

#endif

#define Expects(cond) ORION_CONTRACT_CHECK("Precondition", cond)
#define Ensures(cond) ORION_CONTRACT_CHECK("Postcondition", cond)

//--------------------------------------------------------------------------------------------------
// AssertUnreachable

class AssertError : public orion::Exception
{
public:
   AssertError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

#if defined(ORION_TERMINATE_ON_ASSERT_VIOLATION)

#   define ORION_ASSERT_CHECK(type, cond) \
      (ORION_LIKELY(cond) ? static_cast<void>(0) : std::terminate())

#elif defined(ORION_THROW_ON_ASSERT_VIOLATION)

#   define ORION_ASSERT_CHECK(type, cond) \
      (ORION_LIKELY(cond) ? static_cast<void>(0) : throw AssertError(type, DbgSrcLoc))

#elif defined(ORION_UNENFORCED_ON_ASSERT_VIOLATION)

#   define ORION_ASSERT_CHECK(type, cond) ORION_ASSUME(cond)

#endif

#define AssertUnreachable(msg) ORION_ASSERT_CHECK("Unreachable code " msg, false)

//-------------------------------------------------------------------------------------------------
// at() - Bounds-checked access for static arrays, std::array, std::vector.

template<class T, std::size_t N>
inline constexpr T& at(T (&arr)[N], std::size_t index)
{
   Expects(index < N);
   return arr[index];
}

template<class T, std::size_t N>
inline constexpr T& at(std::array<T, N>& arr, std::size_t index)
{
   Expects(index < N);
   return arr[index];
}

template<class Container>
inline constexpr typename Container::value_type& at(Container& c, std::size_t index)
{
   Expects(index < c.size());
   return c[index];
}

template<class Container>
inline constexpr const typename Container::value_type& at(const Container& c, std::size_t index)
{
   Expects(index < c.size());
   return c[index];
}

template<class T>
inline constexpr const T& at(std::initializer_list<T> l, std::size_t index)
{
   Expects(index < l.size());
   return *(l.begin() + index);
}

//--------------------------------------------------------------------------------------------------
// -- Final action in scope

/// 
/// FinalAction allows you to ensure something gets run at the end of a scope
///
template<class F>
class FinalAction
{
public:
   explicit FinalAction(F action) noexcept
      : _action(std::move(action))
      , _invoke(true)
   {
   }

   FinalAction(FinalAction&& other) noexcept
      : _action(std::move(other._action))
      , _invoke(other._invoke)
   {
      other._invoke = false;
   }

   virtual ~FinalAction() noexcept
   {
      if (_invoke)
      {
         _action();
      }
   }

   FinalAction(const FinalAction&) = delete;
   FinalAction& operator=(const FinalAction&) = delete;
   FinalAction& operator=(FinalAction&&) = delete;

protected:
   void dismiss() noexcept { _invoke = false; }

private:
   F _action;
   bool _invoke;
};

template<class F>
inline FinalAction<F> finally(F const& action) noexcept
{
   return FinalAction<F>{action};
}

template<class F>
inline FinalAction<F> finally(F&& action) noexcept
{
   return FinalAction<F>{std::forward<F>(action)};
}

//--------------------------------------------------------------------------------------------------
// narrow_cast

template<class T, class U>
inline constexpr T narrow_cast(U&& u) noexcept
{
   return static_cast<T>(std::forward<U>(u));
}

class NarrowingError : public orion::Exception
{
public:
   explicit NarrowingError(std::string text)
      : Exception(std::move(text))
   {
   }
};

namespace details
{
template<class T, class U>
struct is_same_signedness
   : public std::integral_constant<bool, std::is_signed<T>::value == std::is_signed<U>::value>
{
};

template<typename T>
struct identity
{
   using type = T;
};

template<typename T>
using identity_t = typename identity<T>::type;

} // namespace details

// narrow() : a checked version of narrow_cast() that throws if the cast changed the value
template<class T, class U>
T narrow(U u) 
{
   T t = narrow_cast<T>(u);
   if (static_cast<U>(t) != u)
   {
      throw NarrowingError{"Lost information"};
   }
   if (not details::is_same_signedness<T, U>::value and ((t < T{}) != (u < U{})))
   {
      throw NarrowingError{"To small"};
   }
   return t;
}

//-------------------------------------------------------------------------------------------------
// As functions

template<class T>
inline std::string as_string(const T& v)
{
   return static_cast<std::string>(v);
}

//-------------------------------------------------------------------------------------------------
// Views: Span

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
   constexpr Span(element_type (&arr)[N]) noexcept
      : _first(std::addressof(arr[0]))    
      , _last(std::addressof(arr[0]) + N)
   {
   }

   template<std::size_t N, class = std::enable_if_t<(N > 0)>>
   constexpr Span(std::array<std::remove_const_t<element_type>, N>& arr) noexcept
      : _first(arr.data())    
      , _last(arr.data() + N)
   {
   }

   constexpr Span(std::array<std::remove_const_t<element_type>, 0>& /*unused*/) noexcept
      : _first(nullptr)    
      , _last(nullptr)
   {
   }

   template<std::size_t N, class = std::enable_if_t<(N > 0)>>
   constexpr Span(const std::array<std::remove_const_t<element_type>, N>& arr) noexcept
      : _first(arr.data())    
      , _last(arr.data() + N)
   {
   }

   constexpr Span(const std::array<std::remove_const_t<element_type>, 0>& /*unused*/) noexcept
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
   constexpr Span(Container& cont)
      : Span(cont.data(), narrow<index_type>(cont.size()))
   {
   }

   template<class Container,
            class = std::enable_if_t<
               std::is_const<element_type>::value && !details::is_span<Container>::value &&
               std::is_convertible<typename Container::pointer, pointer>::value &&
               std::is_convertible<typename Container::pointer,
                                   decltype(std::declval<Container>().data())>::value>>
   constexpr Span(const Container& cont)
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

   constexpr Span first(index_type count) const 
   {
      Expects(0 <= count and count <= this->size());
      return {this->data(), count};
   }

   constexpr Span last(index_type count) const 
   {
      Expects(0 <= count and count <= this->size());
      return {this->data() + this->size() - count, count};
   }

   constexpr Span subspan(index_type offset) const 
   {
      Expects(0 <= offset and offset <= this->size());
      return {this->data() + offset, this->size() - offset};
   }

   constexpr Span subspan(index_type offset, index_type count) const 
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
   Span<U> as_span() const 
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

} // namespace orion

#include <orion/impl/Common.ipp>

#endif // ORION_COMMON_H
