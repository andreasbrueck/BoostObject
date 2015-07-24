////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_OBJECT_INSTALL_HPP
#define BOOST_OBJECT_DETAIL_OBJECT_INSTALL_HPP

#include "config.hpp"
#include "default_log_handler.hpp"

#include "defs/mutex.hpp"

#include <atomic>

namespace boost {
namespace object {

    class base;

    namespace unsafe
    {
        class application;
    }

    namespace detail
    {
        struct BOOST_OBJECT_RESOURCE_API boost_object_installation
        {
            static base * default_base;
            static unsafe::application * globalInstance;

            static mutex_t * connectionMutex;

            static std::atomic < log_handler_t > * log_handler;
        };
    }

} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_OBJECT_INSTALL_HPP

