////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_DETAIL_NETWORK_HPP
#define BOOST_OBJECT_NETWORK_DETAIL_NETWORK_HPP

#ifdef BOOST_OBJECT_DISABLE_HIERARCHY
#error Boost.Object/Network not supported without hierarchy
#endif

#include "../../detail/config.hpp"

namespace boost {
namespace object {
namespace network {

    enum IpVersion
    {
        IPv4,
        IPv6
    };

} // namespace network
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_NETWORK_DETAIL_NETWORK_HPP

