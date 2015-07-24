////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_TCPSERVER_HPP
#define BOOST_OBJECT_NETWORK_TCPSERVER_HPP

#include "tcpsocket.hpp"

namespace boost {
namespace object {
namespace network {
namespace tcp {

BOOST_OBJECT_FORWARD_PRIVATE(server)

BOOST_OBJECT_NS_UNSAFE_START

/**
 * The tcp server.
 */
class server : public object
{
    B_OBJECT(server)

protected:

    /**
     * @param parent @bobject_parent
     *
     * @a Port will be automatically assigned. You can use the @c port() member
     * function to retrieve that port.
     *
     * @a IpVersion will default to IPv4.
     */
    BOOST_OBJECT_DECL explicit server ( object * parent = 0 );

    /**
     * @param port The port this server shall listen on. May be zero, in which
     *             case a port is automatically assigned. You can use the
     *             @c port() member function to retrieve that port.
     * @param parent @bobject_parent
     *
     * @a IpVersion will default to IPv4.
     */
    BOOST_OBJECT_DECL explicit server ( uint16_t port,
                                        object * parent = 0 );

    /**
     * @param ipver The IP version ( IPv4/IPv6 )
     * @param port The port this server shall listen on. May be zero, in which
     *             case a port is automatically assigned. You can use the
     *             @c port() member function to retrieve that port.
     * @param parent @bobject_parent
     */
    BOOST_OBJECT_DECL explicit server ( IpVersion ipver, uint16_t port,
                                        object * parent = 0 );

    /**
     * Constructor for internal extensions of this class.
     */
    BOOST_OBJECT_DECL explicit server ( detail::server_p & p );


    /**
     * This function is internally called when a new connection got accepted.
     * It emits the signal @c newConnection, so overriding this function will
     * stop that signal from being emitted.
     *
     * @param sock The newly accepted connection
     */
    BOOST_OBJECT_DECL virtual void acceptedConnection ( tcp::socket * sock );

#ifndef BOOST_OBJECT_DOXYGEN
private:
    BOOST_OBJECT_DECLARE_PRIVATE(server)

    BOOST_OBJECT_DECL bool _slot_start ();
    BOOST_OBJECT_DECL void _slot_stop ();
    BOOST_OBJECT_DECL uint16_t _slot_port () const;

    BOOST_OBJECT_DECL bool _slot_reuseAddress ();
    BOOST_OBJECT_DECL void _slot_setReuseAddress ( bool v );

public:
    BOOST_OBJECT_AUTO_SLOT(start)
    BOOST_OBJECT_AUTO_SLOT(stop)
    BOOST_OBJECT_AUTO_SLOT(port)

    BOOST_OBJECT_AUTO_SLOT(reuseAddress)
    BOOST_OBJECT_AUTO_SLOT(setReuseAddress)

    BOOST_OBJECT_SIGNAL(newConnection,void(tcp::socket*))

#else // #ifndef BOOST_OBJECT_DOXYGEN

    /**
     * Starts the server, accepting new connections.
     */
    auto_slot void start ();

    /**
     * Stops the server, not accepting new connections.
     */
    auto_slot void stop ();

    /**
     * Returns the port this server listens on.
     */
    auto_slot uint16_t port ();

    /**
     * Gets the current setting of the SO_REUSEADDR socket option.
     */
    auto_slot bool reuseAddress ();

    /**
     * Sets the SO_REUSEADDR socket option.
     */
    auto_slot void setReuseAddress ( bool value );

    /**
     * This signal is emitted when a new connection got accepted.
     *
     * Note: If you override @c acceptedConnection, this signal is not emitted.
     *
     * @param sock The newly accepted connection
     */
    signal void newConnection ( tcp::socket * sock );

#endif // #ifndef BOOST_OBJECT_DOXYGEN
};

BOOST_OBJECT_NS_UNSAFE_END

BOOST_SAFE_OBJECT(server)

} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
typedef boost::object::network::tcp::server BTcpServer;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_NETWORK_TCPSERVER_CPP)
#include "impl/tcpserver.ipp"
#endif

#endif // BOOST_OBJECT_NETWORK_TCPSERVER_HPP

