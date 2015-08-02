////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_DEFAULT_LOG_HANDLER_HPP
#define BOOST_OBJECT_DETAIL_DEFAULT_LOG_HANDLER_HPP

#include "config.hpp"
#include "defs/mutex.hpp"

#include <iostream>

namespace boost {
namespace object {
namespace detail {

    inline void default_log_handler_impl ( std::ostream & c,
                                           const std::string & msg )
    {
        static mutex_t logMutex;

        mutex_locker_t l ( logMutex );
        BOOST_OBJECT_UNUSED(l);

        c << msg << std::endl;
    }

    inline void default_log_handler ( LogLevel ll, const std::string & msg )
    {
        std::string l ( "[Boost.Object][" );

        switch ( ll )
        {
            case LogDebug:
            {
                default_log_handler_impl ( std::cout, l + "Debug]" + msg );
            }
            break;
            case LogError:
            {
                default_log_handler_impl ( std::cerr, l + "Error]" + msg );
            }
            break;
            case LogFatal:
            {
                default_log_handler_impl ( std::cerr, l + "Fatal]" + msg );
                std::terminate ();
            }
            break;
        }
    }

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_DEFAULT_LOG_HANDLER_HPP

