////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_OBJECT_INTERFACE_HPP
#define BOOST_OBJECT_DETAIL_OBJECT_INTERFACE_HPP

#include "config.hpp"
#include "slot.hpp"
#include "signal.hpp"
#include "safe_object.hpp"

#ifdef BOOST_OBJECT_DEBUG
#include <type_traits>
#endif

namespace boost {
namespace object {
namespace detail {

#   ifdef BOOST_OBJECT_DEBUG
    template < typename USER_INPUT_CLASS >
    struct verify_boost_object_classname
    {
        template < typename DEDUCED_CLASS >
        verify_boost_object_classname ( DEDUCED_CLASS * )
        {
            static_assert ( std::is_same
            < DEDUCED_CLASS, USER_INPUT_CLASS >::value,
            "Seems like you made a typo in your B_OBJECT macro" );
        }
    };
#   endif // BOOST_OBJECT_DEBUG

    template < typename OBJECT_CLASS >
    class object_interface
    {
        typedef OBJECT_CLASS object_type;

        friend object_type;

        template < typename... FUNC_TYPE >
        using async_slot_t = typename boost::object::detail::slot_traits
        < boost::object::detail::async_slot_impl,
          OBJECT_CLASS, FUNC_TYPE... >::type;

        template < typename... FUNC_TYPE >
        using direct_slot_t = typename boost::object::detail::slot_traits
        < boost::object::detail::direct_slot_impl,
          OBJECT_CLASS, FUNC_TYPE... >::type;

        template < typename... FUNC_TYPE >
        using auto_slot_t = typename boost::object::detail::slot_traits
        < boost::object::detail::auto_slot_impl,
          OBJECT_CLASS, FUNC_TYPE... >::type;

        template < typename... FUNC_TYPE >
        using signal_t = typename boost::object::detail::signal_traits
        < OBJECT_CLASS, FUNC_TYPE... >::type;

        template < typename... ARGS >
        static object_type * createSafe ( ARGS&&... args )
        {
            return new safe_object < object_type >
                    ( std::forward < ARGS > ( args ) ... );
        }
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_OBJECT_INTERFACE_HPP

