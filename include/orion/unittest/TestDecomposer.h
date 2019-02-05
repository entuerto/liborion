//
// TestDecomposer.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Based on Catch2
//
#ifndef ORION_UNITTEST_TESTDECOMPOSER_H
#define ORION_UNITTEST_TESTDECOMPOSER_H

#include <orion/Config.h>

#include <orion/TypeTraits.h>

#include <fmt/ostream.h>

#include <ostream>
#include <string>
#include <type_traits>

namespace orion
{
namespace unittest
{
namespace detail
{
// Specialised comparison functions to handle equality comparisons between ints and pointers (NULL
// deduces as an int)
template<typename LhsT, typename RhsT>
bool equal(const LhsT& lhs, const RhsT& rhs) 
{
   return static_cast<bool>(lhs == rhs);
}
template<typename T>
bool equal(T* const& lhs, int rhs)
{
   return lhs == reinterpret_cast<void const*>(rhs);
}
template<typename T>
bool equal(T* const& lhs, long rhs) 
{
   return lhs == reinterpret_cast<void const*>(rhs);
}
template<typename T>
bool equal(int lhs, const T*& rhs) 
{
   return reinterpret_cast<void const*>(lhs) == rhs;
}
template<typename T>
bool equal(long lhs, const T*& rhs) 
{
   return reinterpret_cast<void const*>(lhs) == rhs;
}

template<typename LhsT, typename RhsT>
bool not_equal(const LhsT& lhs, RhsT&& rhs) 
{
   return static_cast<bool>(lhs != rhs);
}
template<typename T>
bool not_equal(const T*& lhs, int rhs) 
{
   return lhs != reinterpret_cast<void const*>(rhs);
}
template<typename T>
bool not_equal(const T*& lhs, long rhs)
{
   return lhs != reinterpret_cast<void const*>(rhs);
}
template<typename T>
bool not_equal(int lhs, const T*& rhs) 
{
   return reinterpret_cast<void const*>(lhs) != rhs;
}
template<typename T>
bool not_equal(long lhs, const T*& rhs) 
{
   return reinterpret_cast<void const*>(lhs) != rhs;
}
} // namespace detail

class BaseExpression
{
public:   
   BaseExpression() = default;

   BaseExpression(bool is_binary, bool result)
      : _is_binary(is_binary)
      , _result(result)
   {
   }
   // We don't actually need a virtual destructor, but many static analysers
   // complain if it's not here :-(
   virtual ~BaseExpression() = default;
   
   bool is_binary() const { return _is_binary; }
   bool result() const { return _result; }

   virtual void to_stream(std::ostream& /*unused*/) const {};

private:
   bool _is_binary{false};
   bool _result{false};
};

template<typename LhsT>
class UnaryExpression : public BaseExpression
{
public:
   explicit UnaryExpression(LhsT lhs)
      : BaseExpression{false, static_cast<bool>(lhs)}
      , _lhs(lhs)
   {
   }

   void to_stream(std::ostream& os) const override
   {
      os << _lhs;
   }

private:
   LhsT _lhs;
};

template<typename LhsT, typename RhsT>
class BinaryExpression : public BaseExpression
{
public:
   BinaryExpression(bool cmp_result, LhsT lhs, std::string op, RhsT rhs)
      : BaseExpression(true, cmp_result)
      , m_lhs(lhs)
      , m_op(op)
      , m_rhs(rhs)
   {
   }

   template<typename T>
   const BinaryExpression<LhsT, const RhsT&> operator&&(T /*unused*/) const
   {
      static_assert(always_false<T>::value,
                    "Chained comparisons are not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   template<typename T>
   const BinaryExpression<LhsT, const RhsT&> operator||(T /*unused*/) const 
   {
      static_assert(always_false<T>::value,
                    "Chained comparisons are not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   template<typename T>
   const BinaryExpression<LhsT, const RhsT&> operator==(T /*unused*/) const
   {
      static_assert(always_false<T>::value,
                    "Chained comparisons are not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   template<typename T>
   const BinaryExpression<LhsT, const RhsT&> operator!=(T /*unused*/) const 
   {
      static_assert(always_false<T>::value,
                    "Chained comparisons are not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   template<typename T>
   const BinaryExpression<LhsT, const RhsT&> operator>(T /*unused*/) const 
   {
      static_assert(always_false<T>::value,
                    "Chained comparisons are not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   template<typename T>
   const BinaryExpression<LhsT, const RhsT&> operator<(T /*unused*/) const 
   {
      static_assert(always_false<T>::value,
                    "Chained comparisons are not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   template<typename T>
   const BinaryExpression<LhsT, const RhsT&> operator>=(T /*unused*/) const 
   {
      static_assert(always_false<T>::value,
                    "Chained comparisons are not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   template<typename T>
   const BinaryExpression<LhsT, const RhsT&> operator<=(T /*unused*/) const 
   {
      static_assert(always_false<T>::value,
                    "Chained comparisons are not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   void to_stream(std::ostream& os) const override
   {
      fmt::print(os, "{} {} {}", m_lhs, m_op, m_rhs);
   }

private:
   LhsT m_lhs;
   std::string m_op;
   RhsT m_rhs;
};

template<typename LhsT, typename RhsT>
BinaryExpression<LhsT, const RhsT&> make_binary_expression(bool result, LhsT lhs, std::string op, RhsT rhs)
{
   return {result, lhs, op, rhs};
}

template<typename LhsT>
class ExpressionLhs
{
public:
   explicit ExpressionLhs(LhsT lhs)
      : m_lhs(lhs)
   {
   }

   template<typename RhsT>
   BinaryExpression<LhsT, const RhsT&> operator==(const RhsT& rhs) const
   {
      return {detail::equal(m_lhs, rhs), m_lhs, "==", rhs};
   }

   BinaryExpression<LhsT, bool> operator==(bool rhs) const
   {
      return {m_lhs == rhs, m_lhs, "==", rhs};
   }

   template<typename RhsT>
   BinaryExpression<LhsT, const RhsT&> operator!=(const RhsT& rhs) const
   {
      return {detail::not_equal(m_lhs, rhs), m_lhs, "!=", rhs};
   }

   BinaryExpression<LhsT, bool> operator!=(bool rhs) const
   {
      return {m_lhs != rhs, m_lhs, "!=", rhs};
   }

   template<typename RhsT>
   BinaryExpression<LhsT, const RhsT&> operator>(const RhsT& rhs) const
   {
      return {static_cast<bool>(m_lhs > rhs), m_lhs, ">", rhs};
   }

   template<typename RhsT>
   BinaryExpression<LhsT, const RhsT&> operator<(const RhsT& rhs) const
   {
      return {static_cast<bool>(m_lhs < rhs), m_lhs, "<", rhs};
   }

   template<typename RhsT>
   BinaryExpression<LhsT, const RhsT&> operator>=(const RhsT& rhs) const
   {
      return {static_cast<bool>(m_lhs >= rhs), m_lhs, ">=", rhs};
   }

   template<typename RhsT>
   BinaryExpression<LhsT, const RhsT&> operator<=(const RhsT& rhs) const
   {
      return {static_cast<bool>(m_lhs <= rhs), m_lhs, "<=", rhs};
   }

   template<typename RhsT>
   BinaryExpression<LhsT, const RhsT&> operator&&(const RhsT& /*unused*/) const
   {
      static_assert(always_false<RhsT>::value,
                    "operator&& is not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   template<typename RhsT>
   BinaryExpression<LhsT, const RhsT&> operator||(const RhsT& /*unused*/)  const
   {
      static_assert(always_false<RhsT>::value,
                    "operator|| is not supported inside assertions, "
                    "wrap the expression inside parentheses, or decompose it");
   }

   UnaryExpression<LhsT> make_unary_expression() const 
   { 
      return UnaryExpression<LhsT>{m_lhs}; 
   }

private:
   LhsT m_lhs;
};

struct ExpressionDecomposer
{
   ExpressionDecomposer() = default;

   //DECLARE_DEFAULTS(ExpressionDecomposer);
   //DELETE_COPIES(ExpressionDecomposer);

   // The right operator for capturing expressions is "<=" instead of "<<" (based on the operator
   // precedence table) but then there will be warnings from GCC about "-Wparentheses" and since
   // "_Pragma()" is problematic this will stay for now...
   // https://github.com/philsquared/Catch/issues/870
   // https://github.com/philsquared/Catch/issues/565
   template<typename T>
   ExpressionLhs<const T&> operator<<(const T& lhs)
   {
      return ExpressionLhs<const T&>{lhs};
   }

   ExpressionLhs<bool> operator<<(bool value)
   {
      return ExpressionLhs<bool>{value};
   }
};

} // namespace unittest
} // namespace orion

#endif // ORION_UNITTEST_TESTDECOMPOSER_H
