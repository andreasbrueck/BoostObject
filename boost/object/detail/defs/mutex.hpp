////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_DEFS_MUTEX_HPP
#define BOOST_OBJECT_DETAIL_DEFS_MUTEX_HPP

#include "../config.hpp"
#ifdef BOOST_OBJECT_USE_STL
#include <mutex>
#else
#include <boost/thread/mutex.hpp>
#endif

namespace boost {
namespace object {
namespace detail {

    typedef BOOST_OBJECT_STL::mutex mutex_t;
    typedef BOOST_OBJECT_STL::unique_lock < mutex_t > mutex_locker_t;

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_DEFS_MUTEX_HPP

