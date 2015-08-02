////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_OBJECT_PRIMITIVE_HPP
#define BOOST_OBJECT_DETAIL_OBJECT_PRIMITIVE_HPP

#include "config.hpp"

namespace boost {
namespace object {
namespace detail {

    class object_primitive
    {
    public:
        virtual ~ object_primitive () = default;
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_OBJECT_PRIMITIVE_HPP

