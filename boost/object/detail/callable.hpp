////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_CALLABLE_HPP
#define BOOST_OBJECT_DETAIL_CALLABLE_HPP

#include "config.hpp"
#include "func_traits.hpp"
#include "slot_type.hpp"
#include "signal_type.hpp"

namespace boost {
namespace object {
namespace detail {

    template < typename TYPE, typename = void >
    struct is_callable_functor : std::false_type {};

    template < typename TYPE >
    struct is_callable_functor
    < TYPE, typename std::enable_if < std::is_class < TYPE >::value
            && !is_slot < TYPE >::value
            && !is_signal < TYPE >::value >::type >
    {
#       ifdef BOOST_MSVC
        __if_exists(TYPE::operator()) { static const bool value = true; }
        __if_not_exists(TYPE::operator()) { static const bool value = false; }
#       else
        struct big { char a[2]; };
        template < class T > static big check ( decltype ( &T::operator () ) );
        template < class T > static char check (...);

        static const bool value = sizeof ( check < TYPE > ( 0 ) ) > 1;
#       endif
    };

    template < typename TYPE >
    struct is_callable_function : std::false_type {};

    template < typename RET, typename... ARGS >
    struct is_callable_function < RET (*) ( ARGS... ) >
    : std::true_type {};

    template < typename TYPE, typename = void >
    struct is_callable : std::false_type {};

    template < typename TYPE >
    struct is_callable
    < TYPE, typename std::enable_if
            < is_callable_functor < TYPE >::value >::type >
    : std::true_type {};

    template < typename TYPE >
    struct is_callable
    < TYPE, typename std::enable_if <
            is_callable_function < TYPE >::value >::type >
    : std::true_type {};

    ////////////////////////////////////////////////////////////////////////////

    // TODO: is_callable_with_signature

    ////////////////////////////////////////////////////////////////////////////

    template < typename TYPE, typename = void >
    struct callable_signature
    {
        static_assert(is_callable<TYPE>::value,"Not a callable");
    };

    template < typename TYPE >
    struct callable_signature
    < TYPE, typename std::enable_if
            < is_callable_function < TYPE >::value >::type >
    {
        typedef typename std::remove_pointer < TYPE >::type type;
    };

    template < typename TYPE >
    struct callable_signature
    < TYPE, typename std::enable_if
            < is_callable_functor < TYPE >::value >::type >
    {
        typedef typename detail::remove_member_pointer
        < decltype ( & TYPE::operator () ) >::type type;
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_CALLABLE_HPP

