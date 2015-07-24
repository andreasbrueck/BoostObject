////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_BIND_HELPER_HPP
#define BOOST_OBJECT_DETAIL_BIND_HELPER_HPP

#include "config.hpp"

#include <boost/function_types/result_type.hpp>

#include <functional>

namespace boost {
namespace object {
namespace detail {

    template < int I >
    struct arg {};

    namespace detail
    {    
        template < int... I > struct int_sequence {};
        template < int N, int... I > struct make_int_sequence
            : make_int_sequence < N-1, N-1, I... > {};
        template < int... I > struct make_int_sequence
            < 0, I... >
            : int_sequence < I... > {};

        template < class... args, int... I, class func, class obj,
                   class... default_args >
        inline auto _bind_memfnc_by_args ( obj * o, func f,
                                           int_sequence < I... >,
                                           default_args&&... d )
        -> std::function
        < typename function_types::result_type < func >::type ( args... ) >
        {
            return std::bind
            ( f, o, arg < I + 1 > () ...,
              std::forward < default_args > ( d ) ... );
        }

        template < class... args, int... I, class func,
                   class... default_args >
        inline auto _bind_by_args ( func&& f, int_sequence < I... >,
                                    default_args&&... d )
        -> std::function
        <
            decltype
            (
                std::declval < func > ()
                (
                    std::declval < args > () ...,
                    std::declval < default_args > () ...
                )
            ) ( args ... )
        >
        {
            return std::bind
            ( f, arg < I + 1 > () ...,
              std::forward < default_args > ( d ) ... );
        }
    }

    template < class... args, class func, class obj, class... default_args >
    inline auto bind_memfnc_by_args ( obj * o, func f, default_args&&... d )
    -> std::function
    < typename function_types::result_type < func >::type ( args... ) >
    {
        return detail::_bind_memfnc_by_args < args ... >
        ( o, f, detail::make_int_sequence < sizeof... ( args ) > {},
          std::forward < default_args > ( d ) ... );
    }

    template < class... args, class func, class... default_args >
    inline auto bind_by_args ( func&& f, default_args&&... d )
    -> std::function
    <
        decltype
        (
            std::declval < func > ()
            (
                std::declval < args > () ...,
                std::declval < default_args > () ...
            )
        ) ( args ... )
    >
    {
        return detail::_bind_by_args < args ... >
        ( f, detail::make_int_sequence < sizeof... ( args ) > {},
          std::forward < default_args > ( d ) ... );
    }

} // namespace detail
} // namespace object
} // namespace boost

namespace std
{
    template < int I >
    struct is_placeholder < boost::object::detail::arg < I > >
    : public integral_constant < int, I >
    {};
}

#endif // BOOST_OBJECT_DETAIL_BIND_HELPER_HPP
