#pragma once

#include <type_traits>
#include "peano.hpp"

/*
* Arithmetic
* operations that take natural numbers as parameters
*/

namespace vpp {
    // recursive addition
    template<typename V1, typename V2>
    struct sum_impl {
        // V1 + V2 == (V1 + 1) + (V2 - 1)
        using type = typename sum_impl<succ<V1>, pred<V2>>::type;
    };

    template<typename V1>
    struct sum_impl<V1, void> {
        // V1 + 0 == V1
        using type = V1;
    };

    template<typename V1, typename V2>
    using sum = typename sum_impl<V1, V2>::type;

    static_assert(std::is_same_v<
        itov<25>,
        sum<itov<15>, itov<10>>
    >);



    // recursive multiplication
    template<typename V1, typename V2>
    struct prod_impl {
        // V1 * V2 == V1 * (V2 - 1) + V1
        using type = sum<
            typename prod_impl<V1, pred<V2>>::type,
            V1
        >;
    };

    template<typename V1>
    struct prod_impl<V1, void> {
        // V1 * 0 == 0
        using type = void;
    };

    template<typename V1, typename V2>
    using prod = typename prod_impl<V1, V2>::type;

    static_assert(std::is_same_v<
        itov<30>,
        prod<itov<5>, itov<6>>
    >);


    // comparison & absolute-value subtraction
    template<typename V1, typename V2>
    struct diff_impl {
        using sign_type = typename diff_impl<pred<V1>, pred<V2>>::sign_type;
        using diff_type = typename diff_impl<pred<V1>, pred<V2>>::diff_type;
    };

    template<>
    struct diff_impl<void, void> {
        using sign_type = void;
        using diff_type = void;
    };

    template<typename V1>
    struct diff_impl<V1, void> {
        using sign_type = void;
        using diff_type = V1;
    };

    template<typename V2>
    struct diff_impl<void, V2> {
        using sign_type = void*;
        using diff_type = V2;
    };

    template<typename V1, typename V2>
    using compare = typename diff_impl<V1, V2>::sign_type;

    template<typename V1, typename V2>
    using diff = typename diff_impl<V1, V2>::diff_type;

    template<typename V1, typename V2>
    using signed_diff = sum<std::add_const_t<
        diff<V1, V2>
    >,
        compare<V1, V2>
    >;

    static_assert(std::is_same_v<
        void *****const,
        signed_diff<itov<9>, itov<4>>
    >);

    static_assert(std::is_same_v<
        void *****const*,
        signed_diff<itov<4>, itov<9>>
    >);




    template<typename V1, typename V2>
    struct zero_diff_impl {
        using type = typename zero_diff_impl<pred<V1>, pred<V2>>::type;
    };

    template<typename V1>
    struct zero_diff_impl<V1, void> {
        using type = V1;
    };

    template<typename V1, typename V2>
    using zero_diff = typename zero_diff_impl<V1, V2>::type;



    // integer division
    template<typename V1, typename V2>
    struct idiv_impl {
        using type = succ<typename idiv_impl<zero_diff<V1, V2>, V2>::type>;
    };

    template<typename V2>
    struct idiv_impl<void, V2> {
        using type = void;
    };

    template<typename V1, typename V2>
    using idiv = typename idiv_impl<V1, V2>::type;


    static_assert(std::is_same_v<
        itov<29>,
        idiv<itov<464>, itov<16>>
    >);


    // exponentiation
    template<typename V1, typename V2>
    struct exp_impl {
        // V1 ^ V2 = V1 ^ (V2 - 1) * V1
        using type = prod<
            typename exp_impl<V1, pred<V2>>::type,
            V1
        >;
    };

    template<typename V1>
    struct exp_impl<V1, void> {
        // V1 ^ 0 = 1
        using type = void*;
    };

    template<typename V1, typename V2>
    using exp = typename exp_impl<V1, V2>::type;

    static_assert(std::is_same_v<
        itov<81>,
        exp<itov<3>, itov<4>>
    >);



    // factorial
    template<typename V>
    struct fact_impl {
        // V! = (V - 1)! * V
        using type = prod<
            typename fact_impl<pred<V>>::type,
            V
        >;
    };

    template<>
    struct fact_impl<void> {
        // 0! = 1
        using type = void*;
    };

    template<typename V>
    using factorial = typename fact_impl<V>::type;

    static_assert(std::is_same_v<
        itov<120>,
        factorial<itov<5>>
    >);



    // modulo
    template<typename V1, typename V2>
    struct mod_impl {
        using type = std::conditional_t<
            std::is_same_v< // if V1 < V2
                compare<V1, V2>,
                void*
            >,
            V1,
            typename mod_impl<zero_diff<V1, V2>, V2>::type
        >;
    };

    template<>
    struct mod_impl<void, void> {
        using type = void;
    };

    template<typename V2>
    struct mod_impl<void, V2> {
        using type = void;
    };

    template<typename V1>
    struct mod_impl<V1, void> {
        using type = void;
    };

    template<typename V1, typename V2>
    using mod = typename mod_impl<V1, V2>::type;

    static_assert(std::is_same_v<
        itov<7>,
        mod<itov<127>, itov<10>>
    >);



    // greatest common divisor
    template<typename V1, typename V2>
    struct gcd_impl {
        using type = std::conditional_t<
            is_zero<V2>,
            V1,
            typename gcd_impl<
                V2,
                mod<V1, V2>
            >::type
        >;
    };

    template<>
    struct gcd_impl<void, void> {
        using type = void;
    };

    template<typename V1>
    struct gcd_impl<V1, void> {
        using type = V1;
    };

    template<typename V2>
    struct gcd_impl<void, V2> {
        using type = V2;
    };

    template<typename V1, typename V2>
    using gcd = typename gcd_impl<V1, V2>::type;

    static_assert(std::is_same_v<
        itov<42>,
        gcd<itov<462>, itov<336>>
    >);
}