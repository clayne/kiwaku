//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_DYNAMIC_SHAPE_HPP_INCLUDED
#define KIWAKU_DETAIL_DYNAMIC_SHAPE_HPP_INCLUDED

#include <kiwaku/misc/shape.hpp>
#include <kiwaku/misc/stride.hpp>
#include <kiwaku/detail/dynamic_stride.hpp>
#include <utility>
#include <cstddef>

namespace kwk::detail
{
  template<std::size_t Dimensions> struct dynamic_shape_option
  {
    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using shape_type  = shape<Dimensions>;
    using stride_type = typename shape_type::stride_type;
    using option_tag = shape_option;

    static constexpr bool is_dynamic_option = true;
    static constexpr std::size_t static_size = Dimensions;

    constexpr auto as_stride() const
    {
      return dynamic_stride_option<Dimensions>{};
    }

    //==============================================================================================
    // Pseudo-constructor
    //==============================================================================================
    template<typename... D>
    constexpr auto operator()(D... s) const noexcept requires requires { shape_type{s...}; }
    {
      return shape_type{s...};
    }

    constexpr std::ptrdiff_t numel() const noexcept { return -1;}
  };
}

#endif
