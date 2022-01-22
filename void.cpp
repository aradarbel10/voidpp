#include <type_traits>
#include <iostream>

#include "peano.hpp"
#include "arithmetic.hpp"
#include "list.hpp"
#include "frac.hpp"

int main() {
    using v12ptr = void***** ***** **               ;
    using v13ptr = void              *** ***** *****;
    using v25ptr = void***** ***** ***** ***** *****;

    static_assert(25 == vpp::vtoi<v25ptr>);

    static_assert(std::is_same_v<
        vpp::itov<25>,
        v25ptr
    >);

    static_assert(std::is_same_v<
        vpp::sum<v12ptr, v13ptr>,
        v25ptr
    >);

    static_assert(std::is_const_v<
        void**const
    >);

    return 0;
}