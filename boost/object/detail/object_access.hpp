////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_OBJECT_ACCESS_HPP
#define BOOST_OBJECT_DETAIL_OBJECT_ACCESS_HPP

#include "config.hpp"

namespace boost {
namespace object {

    class base;

namespace detail {

    class base_p;
    class object_internals;

    class object_access
    {
    public:
        BOOST_OBJECT_DECL static object_internals * internals ( base * );
        BOOST_OBJECT_DECL static object_internals * internals ( base_p * );
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_OBJECT_ACCESS_HPP

