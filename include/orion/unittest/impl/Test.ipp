//
// Test.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TEST_IPP
#define ORION_UNITTEST_TEST_IPP

#include <orion/Common.h>
#include <orion/Utils.h>
#include <orion/unittest/TestSuite.h>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <sstream>
#include <string>
#include <type_traits>

namespace orion
{
namespace unittest
{
//-------------------------------------------------------------------------------------------------
// StringConcat

struct StringConcat
{
   std::string& text;

   template<typename T>
   void operator()(T /*unused*/)
   {
   }

   void operator()(const char* v) { text += v; }

   void operator()(const std::string& v) { text += v; }
};

//-------------------------------------------------------------------------------------------------

template<typename LhsT>
class UnaryPredicate
{
public:
   UnaryPredicate(const LhsT& lhs,
                  std::string expr_text,
                  std::string eval_text,
                  Expected ex,
                  UnaryPredFunc<LhsT> cmp_func)
      : _lhs(lhs)
      , _expr_text(std::move(expr_text))
      , _eval_text(std::move(eval_text))
      , _actual(_eval_text.value)
      , _expected(std::move(ex))
      , _cmp_func(std::move(cmp_func))
   {
   }

   Expression expression() const { return _expr_text; }
   Expression evaluated() const { return _eval_text; }

   Actual actual() const { return _actual; }
   Expected expected() const { return _expected; }

   bool compare() const { return _cmp_func(_lhs); }

private:
   LhsT _lhs;
   Expression _expr_text;
   Expression _eval_text;
   Actual _actual;
   Expected _expected;
   UnaryPredFunc<LhsT> _cmp_func;
};

template<typename LhsT>
UnaryPredicate<LhsT> eval_true(const LhsT& lhs, const std::string& expr_text)
{
   return UnaryPredicate<LhsT>{
      lhs, expr_text, fmt::format("{}", lhs), Expected{"true"}, [](const LhsT& lhs) {
         return lhs;
      }};
}

template<typename LhsT>
UnaryPredicate<LhsT> eval_false(const LhsT& lhs, const std::string& expr_text)
{
   return UnaryPredicate<LhsT>{
      lhs, expr_text, fmt::format("{}", lhs), Expected{"false"}, [](const LhsT& lhs) {
         return not lhs;
      }};
}

//-------------------------------------------------------------------------------------------------

template<typename LhsT, typename RhsT>
class BinaryPredicate
{
public:
   BinaryPredicate(LhsT lhs,
                   RhsT rhs,
                   std::string expr_text,
                   std::string eval_text,
                   BinPredFunc<LhsT, RhsT> cmp_func)
      : _lhs(std::move(lhs))
      , _rhs(std::move(rhs))
      , _expr_text(std::move(expr_text))
      , _eval_text(std::move(eval_text))
      , _cmp_func(std::move(cmp_func))
   {
   }

   Expression expression() const { return _expr_text; }
   Expression evaluated() const { return _eval_text; }

   bool compare() const { return _cmp_func(_lhs, _rhs); }

private:
   LhsT _lhs;
   RhsT _rhs;
   Expression _expr_text;
   Expression _eval_text;
   BinPredFunc<LhsT, RhsT> _cmp_func;
};

template<typename LhsT, typename RhsT>
BinaryPredicate<LhsT, RhsT> eval_eq(const LhsT& lhs, const RhsT& rhs, const std::string& expr_text)
{
   return BinaryPredicate<LhsT, RhsT>{lhs,
                                      rhs,
                                      expr_text,
                                      fmt::format("{} == {}", lhs, rhs),
                                      [](const LhsT& lhs, const RhsT& rhs) { return lhs == rhs; }};
}

template<typename LhsT, typename RhsT>
BinaryPredicate<LhsT, RhsT> eval_ne(const LhsT& lhs, const RhsT& rhs, const std::string& expr_text)
{
   return BinaryPredicate<LhsT, RhsT>{lhs,
                                      rhs,
                                      expr_text,
                                      fmt::format("{} != {}", lhs, rhs),
                                      [](const LhsT& lhs, const RhsT& rhs) { return lhs != rhs; }};
}

template<typename LhsT, typename RhsT>
BinaryPredicate<LhsT, RhsT> eval_ge(const LhsT& lhs, const RhsT& rhs, const std::string& expr_text)
{
   return BinaryPredicate<LhsT, RhsT>{lhs,
                                      rhs,
                                      expr_text,
                                      fmt::format("{} >= {}", lhs, rhs),
                                      [](const LhsT& lhs, const RhsT& rhs) { return lhs >= rhs; }};
}

template<typename LhsT, typename RhsT>
BinaryPredicate<LhsT, RhsT> eval_gt(const LhsT& lhs, const RhsT& rhs, const std::string& expr_text)
{
   return BinaryPredicate<LhsT, RhsT>{lhs,
                                      rhs,
                                      expr_text,
                                      fmt::format("{} > {}", lhs, rhs),
                                      [](const LhsT& lhs, const RhsT& rhs) { return lhs > rhs; }};
}

template<typename LhsT, typename RhsT>
BinaryPredicate<LhsT, RhsT> eval_le(const LhsT& lhs, const RhsT& rhs, const std::string& expr_text)
{
   return BinaryPredicate<LhsT, RhsT>{lhs,
                                      rhs,
                                      expr_text,
                                      fmt::format("{} <= {}", lhs, rhs),
                                      [](const LhsT& lhs, const RhsT& rhs) { return lhs <= rhs; }};
}

template<typename LhsT, typename RhsT>
BinaryPredicate<LhsT, RhsT> eval_lt(const LhsT& lhs, const RhsT& rhs, const std::string& expr_text)
{
   return BinaryPredicate<LhsT, RhsT>{lhs,
                                      rhs,
                                      expr_text,
                                      fmt::format("{} < {}", lhs, rhs),
                                      [](const LhsT& lhs, const RhsT& rhs) { return lhs < rhs; }};
}

//-------------------------------------------------------------------------------------------------

inline Test::Test(std::string name)
   : TestInfo(std::move(name))
{
}

inline constexpr const TestResult& Test::result() const
{
   return _result;
}

inline const TestResult& Test::invoke() const
{
   if (not enabled())
   {
      _result.log(AssertionSkipped{Message{disabled()}});
      return _result;
   }

   _result.on_start();
   try
   {
      invoke_setup_func();
      do_invoke();
      invoke_teardown_func();
   }
   catch (const std::exception& e)
   {
      std::string msg{"An unexpected exception was thrown: "};

      msg += e.what();

      _result.log(AssertionException{std::make_exception_ptr(e), Message{msg}, DbgSrcLoc});
   }
   catch (...)
   {
      std::string msg{"An unexpected, unknown exception was thrown: "};

      auto cur_exp = std::current_exception();

      msg += type_name(cur_exp);

      _result.log(AssertionException{cur_exp, Message{msg}, DbgSrcLoc});
   }
   _result.on_end();

   return _result;
}

template<typename LhsT, typename RhsT, typename... Args>
inline void Test::asserter(BinaryExpression<LhsT, RhsT> expr, const std::string& expr_text, Args... args) const
{
   auto t = std::make_tuple(args...);

   if (has_type<Disabled>(t))
   {
      auto d = get_value<Disabled>(t, Disabled{});

      _result.log(AssertionSkipped{Message{d}, get_value<SourceLocation>(t, SourceLocation{})});
      return;
   }

   if (expr.result())
   {
      _result.log(AssertionPassed{});
      return;
   }

   std::stringstream stream;

   expr.to_stream(stream);

   Actual actual{fmt::format("false ({})", stream.str())};

   _result.log(AssertionFailed{Expression{expr_text},
                               actual,
                               Expected{"true"},
                               get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename... Args>
inline void Test::asserter(ExpressionLhs<bool> expr, const std::string& expr_text, Args... args) const
{
   auto t = std::make_tuple(args...);

   if (has_type<Disabled>(t))
   {
      auto d = get_value<Disabled>(t, Disabled{});

      _result.log(AssertionSkipped{Message{d}, get_value<SourceLocation>(t, SourceLocation{})});
      return;
   }

   auto unary_expr = expr.make_unary_expression();

   if (unary_expr.result())
   {
      _result.log(AssertionPassed{});
      return;
   }

   std::stringstream stream;

   unary_expr.to_stream(stream);

   Actual actual{fmt::format("false ({})", stream.str())};

   _result.log(AssertionFailed{Expression{expr_text},
                               actual,
                               Expected{"true"},
                               get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename T, typename... Args>
inline void Test::asserter(UnaryPredicate<T> pred, Args... args) const
{
   auto t = std::make_tuple(args...);

   if (has_type<Disabled>(t))
   {
      auto d = get_value<Disabled>(t, Disabled{});

      _result.log(AssertionSkipped{Message{d}, get_value<SourceLocation>(t, SourceLocation{})});
      return;
   }

   if (pred.compare())
   {
      _result.log(AssertionPassed{});
      return;
   }

   _result.log(AssertionFailed{pred.expression(),
                               pred.actual(),
                               pred.expected(),
                               get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename T1, typename T2, typename... Args>
inline void Test::asserter(BinaryPredicate<T1, T2> pred, Args... args) const
{
   auto t = std::make_tuple(args...);

   if (has_type<Disabled>(t))
   {
      auto d = get_value<Disabled>(t, Disabled{});

      _result.log(AssertionSkipped{Message{d}, get_value<SourceLocation>(t, SourceLocation{})});
      return;
   }

   if (pred.compare())
   {
      _result.log(AssertionPassed{});
      return;
   }

   Actual actual{fmt::format("false ({})", pred.evaluated())};

   _result.log(AssertionFailed{
      pred.expression(), actual, Expected{"true"}, get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename... Args>
inline void Test::fail(Args... args) const
{
   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(StringConcat{msg}, t);

   _result.log(AssertionFailed{Message{msg}, get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename... Args>
inline void Test::fail_if(bool value, Args... args) const
{
   auto t = std::make_tuple(args...);

   if (has_type<Disabled>(t))
   {
      auto d = get_value<Disabled>(t, Disabled{});

      _result.log(AssertionSkipped{Message{d}, get_value<SourceLocation>(t, SourceLocation{})});
      return;
   }

   if (not value)
   {
      _result.log(AssertionPassed{});
      return;
   }

   _result.log(AssertionFailed{Expression{},
                               Actual{std::to_string(value)},
                               Expected{std::to_string(not value)},
                               get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename... Args>
inline void Test::fail_if(const std::error_code& ec, Args... args) const
{
   auto t = std::make_tuple(args...);

   if (has_type<Disabled>(t))
   {
      auto d = get_value<Disabled>(t, Disabled{});

      _result.log(AssertionSkipped{Message{d}, get_value<SourceLocation>(t, SourceLocation{})});
      return;
   }

   if (not ec)
   {
      _result.log(AssertionPassed{});
      return;
   }

   std::string msg = ec.message();

   get_all_values(StringConcat{msg}, t);

   _result.log(AssertionFailed{Message{msg}, get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename... Args>
inline void Test::expected_exception_not_thrown(std::string expr, Args... args) const
{
   auto t = std::make_tuple(args...);

   if (has_type<Disabled>(t))
   {
      auto d = get_value<Disabled>(t, Disabled{});

      _result.log(AssertionSkipped{Message{d}, get_value<SourceLocation>(t, SourceLocation{})});
      return;
   }

   _result.log(AssertionFailed{Expression{std::move(expr)},
                               Actual{},
                               Expected{},
                               Message{"Expected exception not thrown: "},
                               get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename ExceptionT, typename... Args>
inline void Test::exception_thrown_as_expected(const ExceptionT& e, Args... args) const
{
   auto t = std::make_tuple(args...);

   if (has_type<Disabled>(t))
   {
      auto d = get_value<Disabled>(t, Disabled{});

      _result.log(AssertionSkipped{Message{d}, get_value<SourceLocation>(t, SourceLocation{})});
      return;
   }

   _result.log(AssertionPassed{Message{"Exception thrown as expected: " + type_name(e)}});
}

template<typename... Args>
inline void Test::exception_thrown_as_expected(const std::exception_ptr& eptr, Args... args) const
{
   auto t = std::make_tuple(args...);

   if (has_type<Disabled>(t))
   {
      auto d = get_value<Disabled>(t, Disabled{});

      _result.log(AssertionSkipped{Message{d}, get_value<SourceLocation>(t, SourceLocation{})});
      return;
   }

   _result.log(AssertionPassed{Message{"Exception thrown as expected: " + type_name(eptr)}});
}

template<typename ExceptionT, typename... Args>
inline void Test::unexpected_exception_thrown(const ExceptionT& e, Args... args) const
{
   auto t = std::make_tuple(args...);

   _result.log(AssertionException{std::make_exception_ptr(e),
                                  Message{"Unexpected exception thrown: "},
                                  get_value<SourceLocation>(t, SourceLocation{})});
}

template<typename... Args>
inline void Test::unexpected_exception_thrown(const std::exception_ptr& eptr, Args... args) const
{
   auto t = std::make_tuple(args...);

   _result.log(AssertionException{eptr,
                                  Message{"Unexpected exception thrown: "},
                                  get_value<SourceLocation>(t, SourceLocation{})});
}

//-------------------------------------------------------------------------------------------------

template<typename TestT, typename... Args>
inline TestT make_test(TestSuite& suite, Args... args)
{
   try
   {
      TestT test{};

      suite.add_test(test);

      set_options(test, args...);

      return test;
   }
   catch(const std::exception& e)
   {
      log::fatal("An unexpected, exception was thrown: ", e, DbgSrcLoc);
   }
   
   return {};
}

} // namespace unittest
} // namespace orion
#endif // ORION_UNITTEST_TEST_IPP