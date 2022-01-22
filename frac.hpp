#pragma once

#include <type_traits>
#include "peano.hpp"
#include "arithmetic.hpp"
#include "list.hpp"

namespace vpp {
    // fraction simplification
    template<typename V>
    struct simplify_impl {
        using sgn = head<V>;
        using num = head<tail<V>>;
        using den = head<tail<tail<V>>>;

        using divisor = gcd<num, den>;

        using type = list<
            sgn,
            idiv<num, divisor>,
            idiv<den, divisor>
        >;
    };

    template<typename V>
    using simplify = typename simplify_impl<V>::type;


    static_assert(std::is_same_v<
        list<
            void,
            itov<51>,
            itov<22
        >,
        simplify<list<
            void,
            itov<1071>,
            itov<462>
        >>
    >);

    // fraction sum
    template<typename V1, typename V2>
    struct frac_sum_impl {
        using s1 = head<V1>;
        using n1 = head<tail<V1>>;
        using d1 = head<tail<tail<V1>>>;

        using s2 = head<V2>;
        using n2 = head<tail<V2>>;
        using d2 = head<tail<tail<V2>>>;

        using type = std::conditional_t<
            is_void<s1> == is_void<s2>, // pos + pos | neg + neg
            list< // (n1 * d2 + d1 * n2) / (d1 * d2)
                is_void<s1>,
                sum<prod<n1, d2>, prod<d1, n2>>,
                prod<d1, d2>
            >,
        
        std::conditional_t<
            is_void<s1> != is_void<s2>, // pos + neg | neg + pos
            list< // (n1 * d2 - d1 * n2) / (d1 * d2)
                signed_diff<prod<n1, d2>, prod<d1, n2>>,
                prod<d1, d2>
            >
        >>>;
    };

    template<typename V1, typename V2>
    using frac_sum = typename frac_sum_impl<V1, V2>::type;

    static_assert(std::is_same_v<
        list<void, itov<19>, itov<20>>,
        frac_sum<
            list<void, itov<3>, itov<4>>,
            list<void, itov<1>, itov<5>>
        >
    >);

    static_assert(std::is_same_v<
        list<void, itov<19>, itov<20>>,
        frac_sum<
            list<void, itov<3>, itov<4>>,
            list<void, itov<1>, itov<5>>
        >
    >);
}