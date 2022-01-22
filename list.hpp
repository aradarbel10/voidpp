#pragma once

#include <type_traits>
#include "peano.hpp"
#include "arithmetic.hpp"

namespace vpp {
    // cons constructor
    template<typename V1, typename V2>
    using cons = sum<std::add_const_t<V2>, V1>;

    // variadic constructor
    template<typename... Vs>
    struct list_impl;

    template<typename V, typename... Vs>
    struct list_impl<V, Vs...> {
        using type = cons<V, typename list_impl<Vs...>::type>;
    };

    template<typename V1, typename V2>
    struct list_impl<V1, V2> {
        using type = cons<V1, V2>;
    };

    template<typename V>
    struct list_impl<V> {
        using type = V;
    };

    template<>
    struct list_impl<> {
        using type = void;
    };

    template<typename... Vs>
    using list = typename list_impl<Vs...>::type;

    static_assert(std::is_same_v<
        void ***const*const*****const****,
        list<itov<4>, itov<5>, itov<1>, itov<3>>
    >);

    static_assert(std::is_same_v<
        void,
        list<>
    >);

    static_assert(std::is_same_v<
        void***,
        list<itov<3>>
    >);



    // first element
    template<typename Vlist, typename Acc>
    struct head_impl {
        using type = std::conditional_t<
            std::is_const_v<Vlist>,
            Acc,
            typename head_impl<pred<Vlist>, succ<Acc>>::type
        >;
    };

    template<typename Acc>
    struct head_impl<void, Acc> {
        using type = Acc;
    };

    template<typename Vlist>
    using head = typename head_impl<Vlist, void>::type;

    static_assert(std::is_same_v<
        itov<3>,
        head<
            cons<itov<3>, itov<4>>
        >
    >);



    // rest of the elements
    template<typename Vlist>
    struct tail_impl {
        using type = std::conditional_t<
            std::is_const_v<Vlist>,
            std::remove_const_t<Vlist>,
            typename tail_impl<pred<Vlist>>::type
        >;
    };

    template<>
    struct tail_impl<void> {
        using type = void;
    };

    template<typename Vlist>
    using tail = typename tail_impl<Vlist>::type;

    static_assert(std::is_same_v<
        itov<6>,
        tail<
            cons<itov<3>, itov<6>>
        >
    >);
}