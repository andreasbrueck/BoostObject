////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_CONNECTABLE_TYPE_HPP
#define BOOST_OBJECT_DETAIL_CONNECTABLE_TYPE_HPP

#include "config.hpp"

#include <type_traits>

namespace boost {
namespace object {
namespace detail {

    struct generic_connectable_type {};

    template < typename TYPE >
    struct is_generic_connectable_type
      :
        std::is_base_of
            < generic_connectable_type, TYPE >
    {};

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_CONNECTABLE_TYPE_HPP

