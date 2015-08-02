////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_IMPL_TCPSOCKET_P_HPP
#define BOOST_OBJECT_NETWORK_IMPL_TCPSOCKET_P_HPP

#include "../tcpsocket.hpp"
#include "../../impl/object_p.hpp"
#include "../../detail/asio_wrap.hpp"
#include "../../detail/log.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

namespace boost {
namespace object {
namespace network {
namespace tcp {
namespace detail {

    template < typename BASE >
    class socket_imbue : public BASE
    {
    public:
        template < typename... ARGS >
        socket_imbue ( ARGS&&... args )
            :
              BASE ( std::forward < ARGS > ( args ) ... ),
              m_sock ( this->io_service () )
        {}

        asio::ip::tcp::socket & sock () final override
        {
            return m_sock;
        }

    private:
        asio::ip::tcp::socket m_sock;
    };

    class socket_p : public ::boost::object::detail::object_p
    {
        BOOST_OBJECT_DECLARE_PUBLIC(socket)

    protected:
        socket_p ( unsafe::socket * s, object * parent )
            :
              object_p ( s, parent ),
              m_isReading ( false ),
              m_isWriting ( false ),
              m_isConnected ( false )
        {}

        void start ()
        {
            if ( m_isReading ) return;
            m_isReading = true;

            do_read ();
        }

        void do_read ()
        {
            if ( ! m_isConnected )
            {
                m_isReading = false;
                return;
            }

            sock ().async_read_some
            (
                asio::buffer ( m_readBuffer ),
                ::boost::object::detail::asio_handler::wrap
                (
                    getData (),
                    & socket_p::handle_read,
                    this
                )
            );
        }

        void handle_read ( const boost::system::error_code & ec,
                           size_t bytes_transfered )
        {
            if ( ! ec )
            {
                BOOST_OBJECT_OO;
                oo->received.emit
                (
                    utils::bytearray
                    (
                        m_readBuffer.data (),
                        bytes_transfered
                    )
                );
            }
            else
            {
                disconnect ( ec );
            }

            do_read ();
        }

        void write ( const utils::bytearray & data )
        {
            if ( ! m_isConnected ) return;

            start ();

            m_writeData += data;
            do_write ();
        }

        void do_write ()
        {
            if ( m_isWriting ) return;
            if ( m_writeData.isEmpty () ) return;

            m_isWriting = true;

            utils::bytearray datacopy ( m_writeData );

            m_writeData.clear ();

            boost::asio::async_write
            (
                sock (),
                boost::asio::buffer
                (
                    datacopy.constData (),
                    datacopy.size ()
                ),
                ::boost::object::detail::asio_handler::wrap
                (
                    getData (),
                    & socket_p::handle_write,
                    this,
                    datacopy
                )
            );
        }

        void handle_write ( const boost::system::error_code & ec,
                            size_t bytes_transfered,
                            const utils::bytearray & data )
        {
            m_isWriting = false;

            if ( ! ec )
            {
                BOOST_OBJECT_OO;
                oo->written.emit ( data );

                do_write ();
            }
            else
            {
                disconnect ( ec );
            }

            BOOST_OBJECT_UNUSED(bytes_transfered);
        }

        void disconnect ()
        {
            disconnect ( boost::system::error_code () );
        }

        void disconnect ( boost::system::error_code const & dcec )
        {
            if ( m_isConnected )
            {
                m_isConnected = false;

                boost::system::error_code ec;

                sock ().shutdown ( asio::ip::tcp::socket::shutdown_both, ec );
                sock ().close ( ec );

                BOOST_OBJECT_OO;

                if ( dcec == asio::error::operation_aborted ||
                     dcec == asio::error::eof )
                {
                    oo->disconnected.emit ( system::error_code () );
                }
                else
                {
                    oo->disconnected.emit ( dcec );
                }
            }
        }

        bool noDelay ()
        {
            asio::ip::tcp::no_delay opt;
            system::error_code ec;

            sock ().get_option ( opt, ec );

            BOOST_OBJECT_LOG_DEBUG_IF(ec)
                    << "Failed get no_delay option: " << ec;

            return opt.value ();
        }

        void setNoDelay ( bool v )
        {
            system::error_code ec;

            sock ().set_option ( asio::ip::tcp::no_delay ( v ), ec );

            BOOST_OBJECT_LOG_DEBUG_IF(ec)
                    << "Failed set no_delay option: " << ec;
        }

        virtual asio::ip::tcp::socket & sock () = 0;

        predestruct_t predestruct () override
        {
            disconnect ();

            return object_p::predestruct ();
        }

    protected:
        bool m_isReading;
        bool m_isWriting;
        bool m_isConnected;

    private:
        utils::bytearray m_writeData;
        std::array < unsigned char, 512 > m_readBuffer;
    };

} // namespace detail
} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_NETWORK_IMPL_TCPSOCKET_P_HPP

