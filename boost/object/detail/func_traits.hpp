////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_FUNC_TRAITS_HPP
#define BOOST_OBJECT_DETAIL_FUNC_TRAITS_HPP

#include "config.hpp"

#include <type_traits>

namespace boost {
namespace object {
namespace detail {

    namespace detail
    {
        template < typename RET, typename... ARGS >
        struct func_traits_default_delegate
        {
            typedef RET ret_t;
            static const int argc = sizeof... ( ARGS );

            template < typename CLASS >
            using member_func_t =
            RET ( CLASS::* ) ( ARGS ... );

            template < typename CLASS >
            using member_constfunc_t =
            RET ( CLASS::* ) ( ARGS ... ) const;
        };

        template < template < typename, typename... > class DELEGATE,
                   typename RET,
                   typename... ARGS >
        struct func_traits_helper_2 : DELEGATE < RET, ARGS ... >
        {};

        template < template < typename, typename... > class DELEGATE,
                   typename RET,
                   typename... ARGS >
        struct func_traits_helper_2 < DELEGATE, RET ( ARGS ... ) >
            : func_traits_helper_2 < DELEGATE, RET, ARGS... >
        {};

        template < int ARG_COUNT,
                   template < typename, typename... > class DELEGATE,
                   typename... FUNCTION_TYPE >
        struct func_traits_helper
            : func_traits_helper_2 < DELEGATE, FUNCTION_TYPE ... >
        {};

        template < template < typename, typename... > class DELEGATE,
                   typename FUNCTION_TYPE >
        struct func_traits_helper < 1, DELEGATE, FUNCTION_TYPE >
            : func_traits_helper_2 < DELEGATE, FUNCTION_TYPE >
        {};

        template < template < typename, typename... > class DELEGATE >
        struct func_traits_helper < 0, DELEGATE >
            : DELEGATE < void >
        {};

        template < template < typename, typename... > class DELEGATE,
                   typename... FUNCTION_TYPE >
        struct func_traits_delegate
            :
              func_traits_helper
                < sizeof... ( FUNCTION_TYPE ), DELEGATE, FUNCTION_TYPE ... >
        {};

        template < typename TYPE > struct remove_member_pointer;
        template < typename CLASS, typename RET, class... ARGS >
        struct remove_member_pointer < RET ( CLASS::* ) ( ARGS ... ) >
        {
            using type =
#           ifdef BOOST_MSVC
            typename std::remove_pointer < RET (*) ( ARGS ... ) >::type;
#           else
            RET ( ARGS ... );
#           endif
        };

        template < typename CLASS, typename RET, class... ARGS >
        struct remove_member_pointer < RET ( CLASS::* ) ( ARGS ... ) const >
            : remove_member_pointer < RET ( CLASS::* ) ( ARGS ... ) >
        {};

    } // namespace detail

    template < class... FUNCTION_TYPE >
    struct func_traits
        :
          detail::func_traits_delegate
            < detail::func_traits_default_delegate, FUNCTION_TYPE ... >
    {};

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_FUNC_TRAITS_HPP

