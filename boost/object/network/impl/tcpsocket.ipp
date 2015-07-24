////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_IMPL_TCPSOCKET_IPP
#define BOOST_OBJECT_NETWORK_IMPL_TCPSOCKET_IPP

#include "tcpsocket_p.hpp"

namespace boost {
namespace object {
namespace network {
namespace tcp {
namespace unsafe {

    socket::socket ( detail::socket_p & p )
        :
          object ( p )
    {}

    void socket::_slot_start ()
    {
        BOOST_OBJECT_PP;
        pp->start ();
    }

    void socket::_slot_write ( const utils::bytearray & data )
    {
        BOOST_OBJECT_PP;
        pp->write ( data );
    }

    void socket::_slot_disconnect ()
    {
        BOOST_OBJECT_PP;
        pp->disconnect ();
    }

    bool socket::_slot_noDelay ()
    {
        BOOST_OBJECT_PP;
        return pp->noDelay ();
    }

    void socket::_slot_setNoDelay ( bool v )
    {
        BOOST_OBJECT_PP;
        pp->setNoDelay ( v );
    }

} // namespace unsafe
} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_NETWORK_IMPL_TCPSOCKET_IPP

