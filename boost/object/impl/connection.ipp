////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_CONNECTION_IPP
#define BOOST_OBJECT_IMPL_CONNECTION_IPP

#include "../connection.hpp"
#include "../detail/object_install.hpp"

namespace boost {
namespace object {

    connection::connection ( connection_interface_weakptr_t && i )
        :
          m_i ( i )
    {}

    bool connection::disconnect ()
    {
        try
        {
            std::shared_ptr < connection_interface_t > tmp ( m_i );

            detail::mutex_locker_t l
                    ( *detail::boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED ( l );

            return tmp->disconnect ();
        }
        catch ( std::bad_weak_ptr const & )
        {}

        // must already have been disconnected if it doesnt exist anymore.
        return false;
    }

    bool connection::connected () const
    {
        try
        {
            std::shared_ptr < connection_interface_t > tmp ( m_i );

            detail::mutex_locker_t l
                    ( *detail::boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED ( l );

            return tmp->connected ();
        }
        catch ( std::bad_weak_ptr const & )
        {}

        // must already have been disconnected if it doesnt exist anymore.
        return false;
    }

    bool connection::operator_bool () const
    {
        return connected ();
    }

} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_CONNECTION_IPP

