////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_CONNECTION_INTERFACE_HPP
#define BOOST_OBJECT_DETAIL_CONNECTION_INTERFACE_HPP

#include "config.hpp"

namespace boost {
namespace object {

    class connection;

namespace detail {

    class connection_interface
    {
        friend class connection;

        typedef bool (*fnDisconnect_t)(connection_interface*);
        typedef bool (*fnConnected_t)(connection_interface*);

        fnDisconnect_t fnDisconnect;
        fnConnected_t fnConnected;

    protected:
        connection_interface ( fnDisconnect_t fnDisconnect_,
                               fnConnected_t fnConnected_ )
            :
              fnDisconnect ( fnDisconnect_ ),
              fnConnected ( fnConnected_ )
        {}

    public:
        // Make sure connectionMutex is locked before you call any of below
        // functions

        bool disconnect ()
        {
            return fnDisconnect ( this );
        }

        bool connected ()
        {
            return fnConnected ( this );
        }
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_CONNECTION_INTERFACE_HPP

