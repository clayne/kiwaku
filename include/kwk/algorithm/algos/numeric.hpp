//======================================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/context/context.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

// ALGO:
/*
partial_sum -> à voir si duplicata inclusive scan

adjacent_difference -> à voir à part
*/

// TODO: put back kwk:: namespace before every function call

namespace kwk
{
  template<typename Context, typename Func_R, typename Func_T, concepts::container In1, concepts::container In2>
  constexpr auto transform_reduce(Context& ctx, In1 const& in1, In2 const& in2, auto init, Func_R R, Func_T T)
  {
    // return error for in1.shape() != in2.shape()
    ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
    return init;
  }

  template<typename Func_R, typename Func_T, concepts::container In1, concepts::container In2>
  constexpr auto transform_reduce(In1 const& in1, In2 const& in2, auto init, Func_R R, Func_T T)
  {
    return transform_reduce(cpu, in1, in2, init, R, T);
  }

  // voir pour avoir version func_T et init sans func_R ?
  template<typename Context, typename Func_T, concepts::container In1, concepts::container In2>
  constexpr auto transform_reduce(Context& ctx, In1 const& in1, In2 const& in2, auto init, Func_T T)
  {
    // return error for in1.shape() != in2.shape()
    return transform_reduce(ctx, in1, in2, init, [](auto r, auto d){ return (r+d); },  T);
  }

  template<typename Func_T, concepts::container In1, concepts::container In2>
  constexpr auto transform_reduce(In1 const& in1, In2 const& in2, auto init, Func_T T)
  {
    return transform_reduce(cpu, in1, in2, init, T);
  }

  template<typename Context, concepts::container In1, concepts::container In2>
  constexpr auto transform_reduce(Context& ctx, In1 const& in1, In2 const& in2, auto init)
  {
    // return error for in1.shape() != in2.shape()
    //By default, the transform function of the standard is (*) multiply
    return transform_reduce(ctx, in1, in2, init, [](auto i1, auto i2){ return (i1 * i2); });
  }

  template<concepts::container In1, concepts::container In2>
  constexpr auto transform_reduce(In1 const& in1, In2 const& in2, auto init)
  {
    return transform_reduce(cpu, in1, in2, init);
  }

  // Not present in the standard library
  template<typename Context, concepts::container In1, concepts::container In2>
  constexpr auto transform_reduce(Context& ctx, In1 const& in1, In2 const& in2)
  {
    // According to cppreference
    using t1 = typename In1::value_type;
    using t2 = typename In2::value_type;
    auto R = [](auto const& e1, auto const& e2) { return e1 + e2; };
    auto T = [](auto const& e1, auto const& e2) { return e1 * e2; };
    using return_type = decltype(R(t1{}, T(t1{}, t2{})));

    // return error for in1.shape() != in2.shape()
    return transform_reduce(ctx, in1, in2, return_type{});
  }

  template<concepts::container In1, concepts::container In2>
  constexpr auto transform_reduce(In1 const& in1, In2 const& in2)
  {
    return transform_reduce(cpu, in1, in2);
  }

  template<typename Context, typename Func_1, typename Func_2, concepts::container In1, concepts::container In2>
  constexpr auto inner_product(Context& ctx, In1 const& in1, In2 const& in2, auto init, Func_1 f1, Func_2 f2)
  {
    KIWAKU_ASSERT(  in1.shape() == in2.shape()
                  , "[KWK] - Inner product dimensions do not match, container one dimensions are "
                    << in1.shape() << ",  container two dimensions are "
                    << in2.shape() << "."
                  );
    ctx.map([&](auto const& i1, auto const& i2) { init = f1(init, f2(i1, i2)); }, ctx.in(in1), ctx.in(in2));
    return init;
  }

  template<typename Func_1, typename Func_2, concepts::container In1, concepts::container In2>
  constexpr auto inner_product(In1 const& in1, In2 const& in2, auto init, Func_1 f1, Func_2 f2)
  {
    return inner_product(cpu, in1, in2, init, f1, f2);
  }

  template<typename Context, concepts::container In1, concepts::container In2>
  constexpr auto inner_product(Context& ctx, In1 const& in1, In2 const& in2, auto init)
  {
    return inner_product(ctx, in1, in2, init,
          [](auto r, auto d){ return (r+d); },
          [](auto i1, auto i2){ return (i1*i2); });
  }

  template<concepts::container In1, concepts::container In2>
  constexpr auto inner_product(In1 const& in1, In2 const& in2, auto init)
  {
    return inner_product(cpu, in1, in2, init);
  }

  template<typename Context, concepts::container In1, concepts::container In2>
  constexpr auto inner_product(Context& ctx, In1 const& in1, In2 const& in2)
  {
    // According to cppreference
    using t1 = typename In1::value_type;
    using t2 = typename In2::value_type;
    auto sum = [](auto const& e1, auto const& e2) { return e1 + e2; };
    auto product = [](auto const& e1, auto const& e2) { return e1 * e2; };
    using return_type = decltype(sum(t1{}, product(t1{}, t2{})));

    return inner_product(ctx, in1, in2, return_type{});
  }

  template<concepts::container In1, concepts::container In2>
  constexpr auto inner_product(In1 const& in1, In2 const& in2)
  {
    return inner_product(cpu, in1, in2);
  }

  template<typename Context, typename Func1, typename Func2, concepts::container In, concepts::container Out >
  constexpr auto transform_exclusive_scan(Context& ctx, In const& in, Out& out, auto init, Func1 f1, Func2 f2)
  {
    auto sum = init;
    ctx.map([&](auto const& i, auto& o) { o = sum; sum = f1(sum, f2(i)); }, ctx.in(in), ctx.out(out));
  }

  template< typename Func1, typename Func2, concepts::container In, concepts::container Out >
  constexpr auto transform_exclusive_scan(In const& in, Out& out, auto init, Func1 f1, Func2 f2)
  {
    transform_exclusive_scan(cpu, in, out, init, f1, f2);
  }

  template<typename Context, typename Func, concepts::container In, concepts::container Out >
  constexpr auto exclusive_scan(Context& ctx, In const& in, Out& out, auto init, Func f)
  {
    transform_exclusive_scan(ctx, in, out, init, f, [](auto e){return e;});
  }

  template< typename Func, concepts::container In, concepts::container Out >
  constexpr auto exclusive_scan(In const& in, Out& out, auto init, Func f)
  {
    exclusive_scan(cpu, in, out, init, f);
  }

  template<typename Context, concepts::container In, concepts::container Out >
  constexpr auto exclusive_scan(Context& ctx, In const& in, Out& out, auto init)
  {
    exclusive_scan(ctx, in, out, init, [](auto e, auto i){return (e + i);});
  }

  template< concepts::container In, concepts::container Out >
  constexpr auto exclusive_scan(In const& in, Out& out, auto init)
  {
    exclusive_scan(cpu, in, out, init);
  }

  // (Sasa?): "a corriger"
  template<typename Context, typename Func1, typename Func2, concepts::container In, concepts::container Out >
  constexpr auto transform_inclusive_scan(Context& ctx, In const& in, Out& out, auto init, Func1 f1, Func2 f2)
  {
    // Will not work with sycl_context
    auto sum = init;
    ctx.map([&](auto const& i, auto& o) { sum = f1(sum, f2(i)); o = sum; }, ctx.in(in), ctx.out(out));
  }

  template< typename Func1, typename Func2, concepts::container In, concepts::container Out >
  constexpr auto transform_inclusive_scan(In const& in, Out& out, auto init, Func1 f1, Func2 f2)
  {
    transform_inclusive_scan(cpu, in, out, init, f1, f2);
  }

  template<typename Context, typename Func, concepts::container In, concepts::container Out >
  constexpr auto inclusive_scan(Context& ctx, In const& in, Out& out, auto init, Func f)
  {
    transform_inclusive_scan(ctx, in, out, init, f, [](auto e){return e;});
  }

  template< typename Func, concepts::container In, concepts::container Out >
  constexpr auto inclusive_scan(In const& in, Out& out, auto init, Func f)
  {
    inclusive_scan(cpu, in, out, init, f);
  }

  template<typename Context, concepts::container In, concepts::container Out >
  constexpr auto inclusive_scan(Context& ctx, In const& in, Out& out, auto init)
  {
    inclusive_scan(ctx, in, out, init, [](auto e, auto i){return (e + i);});
  }

  template< concepts::container In, concepts::container Out >
  constexpr auto inclusive_scan(In const& in, Out& out, auto init)
  {
    inclusive_scan(cpu, in, out, init);
  }

} // namespace kwk
