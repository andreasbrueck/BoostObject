////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_TCPCLIENT_HPP
#define BOOST_OBJECT_NETWORK_TCPCLIENT_HPP

#include "tcpsocket.hpp"

namespace boost {
namespace object {
namespace network {
namespace tcp {

BOOST_OBJECT_FORWARD_PRIVATE(client)

BOOST_OBJECT_NS_UNSAFE_START

/**
 * The tcp client class.
 */
class client : public tcp::socket
{
    B_OBJECT(client)

protected:
    /**
     * @param parent @bobject_parent
     */
    BOOST_OBJECT_DECL explicit client ( object * parent = 0 );

    /**
     * Constructor for internal extensions of this class.
     */
    BOOST_OBJECT_DECL explicit client ( detail::client_p & p );

#ifndef BOOST_OBJECT_DOXYGEN
private:
    BOOST_OBJECT_DECLARE_PRIVATE(client)

    BOOST_OBJECT_DECL void _slot_connect ( const std::string & ip,
                                            uint16_t port );

public:
    BOOST_OBJECT_AUTO_SLOT(connect)

    BOOST_OBJECT_SIGNAL(connected,void())

#else // #ifndef BOOST_OBJECT_DOXYGEN

    /**
     * @param host A hostname or IPv4/6 address to connect to
     * @param port The port to connect to.
     *
     * This connects the socket to @p host:@p port. On success, the signal
     * @c connected is emitted. On failure, @c disconnected is emitted.
     */
    auto_slot void connect ( const std::string & host, uint16_t port );

    /**
     * This signal is emitted when the socket successfully connects to the host
     * and port given to the @c connect function.
     */
    signal void connected ();

#endif // #ifndef BOOST_OBJECT_DOXYGEN
};

BOOST_OBJECT_NS_UNSAFE_END

BOOST_SAFE_OBJECT(client)

} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
typedef boost::object::network::tcp::client BTcpClient;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_NETWORK_TCPCLIENT_CPP)
#include "impl/tcpclient.ipp"
#endif

#endif // BOOST_OBJECT_NETWORK_TCPCLIENT_HPP

