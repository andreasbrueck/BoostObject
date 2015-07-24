////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_IMPL_TCPCONNECTION_HPP
#define BOOST_OBJECT_NETWORK_IMPL_TCPCONNECTION_HPP

#include "tcpconnection_p.hpp"
#include "../tcpsocket.hpp"

#include <boost/asio/ip/tcp.hpp>

namespace boost {
namespace object {
namespace network {
namespace tcp {

    BOOST_OBJECT_FORWARD_PRIVATE(server)

    namespace unsafe
    {
        class connection : public tcp::socket
        {
            B_OBJECT(connection)

        public:
            asio::ip::tcp::socket::lowest_layer_type & sock ()
            {
                BOOST_OBJECT_PP;
                return pp->sock ();
            }

        protected:
            connection ( object * parent )
                :
                  tcp::socket ( *new detail::socket_imbue
                                  < detail::connection_p > ( this, parent ) )
            {}

        private:
            BOOST_OBJECT_DECLARE_PRIVATE(connection)
            friend class detail::server_p;

            void _slot_setConnected ( bool b )
            {
                BOOST_OBJECT_PP;
                pp->m_isConnected = b;
            }

        public:
            BOOST_OBJECT_AUTO_SLOT(setConnected)
        };
    }

    BOOST_SAFE_OBJECT(connection)

} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_NETWORK_IMPL_TCPCONNECTION_HPP

