////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_IMPL_TCPCLIENT_IPP
#define BOOST_OBJECT_NETWORK_IMPL_TCPCLIENT_IPP

#include "tcpclient_p.hpp"

namespace boost {
namespace object {
namespace network {
namespace tcp {
namespace unsafe {

    client::client ( object * parent )
        :
          tcp::socket ( *new detail::socket_imbue
                          < detail::client_p > ( this, parent ) )
    {}

    client::client ( detail::client_p & p )
        :
          tcp::socket ( p )
    {}

    void client::_slot_connect ( const std::string & ip, uint16_t port )
    {
        BOOST_OBJECT_PP;
        pp->connect ( ip, port );
    }

} // namespace unsafe
} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_NETWORK_IMPL_TCPCLIENT_IPP

