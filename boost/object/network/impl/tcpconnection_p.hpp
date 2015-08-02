////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_NETWORK_IMPL_TCPCONNECTION_P_HPP
#define BOOST_OBJECT_NETWORK_IMPL_TCPCONNECTION_P_HPP

#include "tcpsocket_p.hpp"

namespace boost {
namespace object {
namespace network {
namespace tcp {
namespace unsafe { class connection; }
namespace detail {

    class connection_p : public socket_p
    {
        friend class unsafe::connection;

    protected:
        connection_p ( unsafe::socket * s, object * parent )
            :
              socket_p ( s, parent )
        {}
    };

} // namespace detail
} // namespace tcp
} // namespace network
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_NETWORK_IMPL_TCPCONNECTION_P_HPP

