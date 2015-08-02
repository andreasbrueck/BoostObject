////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_IMPL_TCPCLIENT_P_HPP
#define BOOST_OBJECT_DETAIL_IMPL_TCPCLIENT_P_HPP

#include "../tcpclient.hpp"
#include "tcpsocket_p.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>

namespace boost {
namespace object {
namespace network {
namespace tcp {
namespace detail {

    class client_p : public socket_p
    {
        BOOST_OBJECT_DECLARE_PUBLIC(client)

    protected:
        client_p ( unsafe::client * c, object * parent )
            :
              socket_p ( c, parent )
        {}

        void connect ( const std::string & ip, uint16_t port )
        {
            if ( m_isConnected )
                return;

            namespace bai = asio::ip;

            system::error_code ec;

            auto addr = bai::address::from_string ( ip.c_str (), ec );

            if ( ! ec )
            {
                sock ().async_connect
                (
                    bai::tcp::endpoint ( addr, port ),
                    ::boost::object::detail::asio_handler::wrap
                    (
                        getData (),
                        & client_p::handle_connect,
                        this
                    )
                );
            }
            else
            {
                bai::tcp::resolver resolver ( io_service () );
                bai::tcp::resolver::query query ( ip, std::to_string ( port ) );

                auto it = resolver.resolve ( query, ec );

                boost::asio::async_connect
                (
                    sock (),
                    it,
                    ::boost::object::detail::asio_handler::wrap
                    (
                        getData (),
                        & client_p::handle_connect_resolved,
                        this
                    )
                );
            }
        }

        void handle_connect ( const system::error_code & ec )
        {
            m_isConnected = true;

            if ( ec )
            {
                disconnect ( ec );
            }
            else
            {
                BOOST_OBJECT_OO;
                oo->connected.emit ();
            }
        }

        void handle_connect_resolved ( const system::error_code & ec,
                                       asio::ip::tcp::resolver::iterator it )
        {
            BOOST_OBJECT_UNUSED(it);
            handle_connect ( ec );
        }
    };

} // namespace detail
} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_IMPL_TCPCLIENT_P_HPP

