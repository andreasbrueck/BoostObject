////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_IMPL_TCPSERVER_P_HPP
#define BOOST_OBJECT_NETWORK_IMPL_TCPSERVER_P_HPP

#include "../tcpserver.hpp"
#include "../tcpsocket.hpp"
#include "tcpconnection.hpp"

#include "../../detail/log.hpp"
#include "../../detail/asio_wrap.hpp"

#include <boost/asio/ip/tcp.hpp>

namespace boost {
namespace object {
namespace network {
namespace tcp {
namespace detail {

    inline boost::asio::ip::tcp IPVerToAsio ( IpVersion ipver )
    {
        if ( ipver == IPv4 )
        {
            return boost::asio::ip::tcp::v4 ();
        }
        else
        {
            return boost::asio::ip::tcp::v6 ();
        }
    }

    class server_p : public ::boost::object::detail::object_p
    {
        BOOST_OBJECT_DECLARE_PUBLIC(server)

        typedef asio::ip::tcp::acceptor acceptor_t;
        typedef asio::ip::tcp::endpoint endpoint_t;

    protected:
        server_p
        (
            unsafe::server * s,
            IpVersion ipver,
            uint16_t port,
            object * parent
        )
            :
              object_p ( s, parent ),
              m_acceptor ( io_service (),
                           endpoint_t ( IPVerToAsio ( ipver ), port ) ),
              m_isRunning ( false ),
              m_ipver ( ipver )
        {}

        virtual void accept ()
        {
            tcp::connection * conn = new tcp::connection ( o_ptr () );

            m_acceptor.async_accept
            (
                conn->sock(),
                ::boost::object::detail::asio_handler::wrap
                (
                    getData (),
                    & server_p::handleAccept,
                    this,
                    conn
                )
            );
        }

        predestruct_t predestruct () override
        {
            stop ();

            return object_p::predestruct ();
        }

    private:
        bool start ()
        {
            if ( m_isRunning )
                return true;

            if ( ! m_acceptor.is_open () )
            {
                boost::system::error_code ec;

                m_acceptor.open ( IPVerToAsio ( m_ipver ), ec );

                BOOST_OBJECT_LOG_ERROR_IF(ec) << "acceptor::open(): " << ec;

                if ( ec ) return false;
            }

            m_isRunning = true;

            accept ();

            return true;
        }

        void stop ()
        {
            if ( ! m_isRunning ) return;

            m_isRunning = false;

            boost::system::error_code ec;
            m_acceptor.close ( ec );

            BOOST_OBJECT_LOG_DEBUG_IF(ec) << "acceptor::close(): " << ec;
        }

        uint16_t port () const
        {
            return m_acceptor.local_endpoint ().port ();
        }

        bool reuseAddress ()
        {
            asio::socket_base::reuse_address opt;
            system::error_code ec;

            m_acceptor.get_option ( opt, ec );

            BOOST_OBJECT_LOG_DEBUG_IF(ec)
                    << "Failed set reuse_address option: " << ec;

            return opt.value ();
        }

        void setReuseAddress ( bool v )
        {
            system::error_code ec;

            m_acceptor.set_option ( asio::socket_base::reuse_address( v ), ec );

            BOOST_OBJECT_LOG_DEBUG_IF(ec)
                    << "Failed set reuse_address option: " << ec;
        }

        void handleAccept ( const boost::system::error_code & ec,
                            tcp::connection * conn )
        {
            if ( ! ec )
            {
                BOOST_OBJECT_OO;

                conn->setConnected ( true );

                oo->acceptedConnection ( conn );
            }
            else
            {
                delete conn;

                if ( ec == asio::error::operation_aborted )
                {
                    return;
                }
            }

            if ( m_isRunning )
            {
                accept ();
            }
        }

    protected:
        acceptor_t m_acceptor;

    private:
        bool m_isRunning;
        IpVersion m_ipver;
    };

} // namespace detail
} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_NETWORK_IMPL_TCPSERVER_P_HPP

