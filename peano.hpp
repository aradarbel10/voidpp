#pragma once

#include <type_traits>

namespace vpp {
    template<typename V>
    using succ = std::add_pointer_t<V>; // V + 1

    template<typename V>
    using pred = std::remove_pointer_t<V>; // V - 1




    template<typename V>
    static constexpr bool is_zero = false;

    template<>
    constexpr bool is_zero<void> = true;


    // void numeral -> integer
    template<typename V>
    constexpr int vtoi = vtoi<pred<V>> + 1;

    template<>
    constexpr int vtoi<void> = 0;


    // integer -> void numeral
    template<int I>
    struct itov_impl {
        using type = succ<typename itov_impl<I - 1>::type>;
    };

    template<>
    struct itov_impl<0> {
        using type = void;
    };

    template<int I>
    using itov = typename itov_impl<I>::type;
}