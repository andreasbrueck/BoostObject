////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_IMPL_TCPSERVER_IPP
#define BOOST_OBJECT_NETWORK_IMPL_TCPSERVER_IPP

#include "tcpserver_p.hpp"

namespace boost {
namespace object {
namespace network {
namespace tcp {
namespace unsafe {

    server::server ( object * parent )
        :
          object ( *new detail::server_p ( this, IPv4, 0, parent ) )
    {}

    server::server ( uint16_t port, object * parent )
        :
          object ( *new detail::server_p ( this, IPv4, port, parent ) )
    {}

    server::server ( IpVersion ipver, uint16_t port, object * parent )
        :
          object ( *new detail::server_p ( this, ipver, port, parent ) )
    {}

    server::server ( detail::server_p & p )
        :
          object ( p )
    {}

    bool server::_slot_start ()
    {
        BOOST_OBJECT_PP;
        return pp->start ();
    }

    void server::_slot_stop ()
    {
        BOOST_OBJECT_PP;
        pp->stop ();
    }

    uint16_t server::_slot_port () const
    {
        BOOST_OBJECT_PPC;
        return pp->port ();
    }

    bool server::_slot_reuseAddress ()
    {
        BOOST_OBJECT_PP;
        return pp->reuseAddress ();
    }

    void server::_slot_setReuseAddress ( bool v )
    {
        BOOST_OBJECT_PP;
        pp->setReuseAddress ( v );
    }

    void server::acceptedConnection ( tcp::socket * sock )
    {
        newConnection ( sock );
    }

} // namespace unsafe
} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_NETWORK_IMPL_TCPSERVER_IPP

