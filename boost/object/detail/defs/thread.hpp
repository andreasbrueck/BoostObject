////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_DEFS_THREAD_HPP
#define BOOST_OBJECT_DETAIL_DEFS_THREAD_HPP

#include "../config.hpp"
#ifdef BOOST_OBJECT_USE_STL
#include <thread>
#else
#include <boost/thread/thread.hpp>
#endif

namespace boost {
namespace object {
namespace detail {

    typedef BOOST_OBJECT_STL::thread thread_t;

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_DEFS_THREAD_HPP

