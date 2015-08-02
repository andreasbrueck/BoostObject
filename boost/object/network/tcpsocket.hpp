////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_TCPSOCKET_HPP
#define BOOST_OBJECT_NETWORK_TCPSOCKET_HPP

#include "detail/network.hpp"

#include "../object.hpp"
#include "../utils/bytearray.hpp"

namespace boost {
namespace object {
namespace network {
namespace tcp {

BOOST_OBJECT_FORWARD_PRIVATE(socket)

BOOST_OBJECT_NS_UNSAFE_START

/**
 * The base socket connection class for TCP.
 */
class socket : public object
{
    B_OBJECT(socket)

protected:
    /**
     * Constructor for internal extensions of this class.
     */
    BOOST_OBJECT_DECL explicit socket ( detail::socket_p & p );

#ifndef BOOST_OBJECT_DOXYGEN
private:
    BOOST_OBJECT_DECLARE_PRIVATE(socket)

    BOOST_OBJECT_DECL void _slot_start ();
    BOOST_OBJECT_DECL void _slot_write ( const utils::bytearray & data );
    BOOST_OBJECT_DECL void _slot_disconnect ();

    BOOST_OBJECT_DECL bool _slot_noDelay ();
    BOOST_OBJECT_DECL void _slot_setNoDelay ( bool v );

public:
    BOOST_OBJECT_AUTO_SLOT(start)
    BOOST_OBJECT_AUTO_SLOT(write)
    BOOST_OBJECT_AUTO_SLOT(disconnect)

    BOOST_OBJECT_AUTO_SLOT(noDelay)
    BOOST_OBJECT_AUTO_SLOT(setNoDelay)

    BOOST_OBJECT_SIGNAL(received,void(const utils::bytearray&))
    BOOST_OBJECT_SIGNAL(written,void(const utils::bytearray&))
    BOOST_OBJECT_SIGNAL(disconnected,void(const system::error_code&))

#else // #ifndef BOOST_OBJECT_DOXYGEN

    /**
     * Starts reading the socket. Call this function after you connected
     * all your signals.
     *
     * This function has no effect if the socket is not connected.
     */
    auto_slot void start ();

    /**
     * Writes @p data to the socket.
     *
     * This function has no effect if the socket is not connected.
     *
     * Using this function will @c start reading on this socket, if its not
     * reading yet.
     *
     * @param data The data to be written
     */
    auto_slot void write ( const utils::bytearray & data );

    /**
     * Disconnects the socket connection.
     *
     * This function has no effect if the socket is not connected.
     */
    auto_slot void disconnect ();

    /**
     * Gets the current setting of the TCP_NODELAY socket option.
     */
    auto_slot bool noDelay ();

    /**
     * Sets the TCP_NODELAY socket option.
     *
     * @param value True to enable, false to disable TCP_NODELAY
     */
    auto_slot void setNoDelay ( bool value );

    /**
     * This signal is emitted when new data has been read by the socket.
     *
     * @param data The data that was read.
     */
    signal void received ( const utils::bytearray & data );

    /**
     * This signal is emitted after @p data was written to the socket.
     *
     * This data might not be the same as you gave to the @c write function. The
     * data is internally merged with existing data that has not been sent yet.
     *
     * @param data The data that was written.
     */
    signal void written ( const utils::bytearray & data );

    /**
     * This signal is emitted once when a socket is disconnected.
     */
    signal void disconnected ( const boost::system::error_code & ec );

#endif // #ifndef BOOST_OBJECT_DOXYGEN
};

BOOST_OBJECT_NS_UNSAFE_END

BOOST_SAFE_OBJECT(socket)

} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
typedef boost::object::network::tcp::socket BTcpSocket;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_NETWORK_TCPSOCKET_CPP)
#include "impl/tcpsocket.ipp"
#endif

#endif // BOOST_OBJECT_NETWORK_TCPSOCKET_HPP

