////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_SLOT_CONTEX_HPP
#define BOOST_OBJECT_DETAIL_SLOT_CONTEX_HPP

#include "config.hpp"

namespace boost {
namespace object {

    class object;

namespace detail {

    template < typename, typename... > class auto_slot_impl;
    template < typename, typename... > class async_slot_impl;
    template < typename, typename... > class direct_slot_impl;
    template < typename, typename... > class signal_impl_base;
    class sender_scope;

    class slot_context
    {
        template < typename, typename... > friend class auto_slot_impl;
        template < typename, typename... > friend class async_slot_impl;
        template < typename, typename... > friend class direct_slot_impl;
        template < typename, typename... > friend class signal_impl_base;
        friend class sender_scope;

    private:
        slot_context ( object * signal_owner = 0 )
            :
              signal_owner ( signal_owner )
        {}

    private:
        object * signal_owner;
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_SLOT_CONTEX_HPP

