////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_UTILS_IMPL_SYSTEMSIGNALS_P_HPP
#define BOOST_OBJECT_UTILS_IMPL_SYSTEMSIGNALS_P_HPP

#include "../systemsignals.hpp"

#include "../../detail/asio_wrap.hpp"
#include "../../detail/log.hpp"
#include "../../impl/object_p.hpp"

#include <boost/asio/signal_set.hpp>

namespace boost {
namespace object {
namespace utils {
namespace detail {

    class systemsignals_p : public ::boost::object::detail::object_p
    {
        BOOST_OBJECT_DECLARE_PUBLIC(systemsignals)

        boost::asio::signal_set m_signalset;

        template < typename... SIGS >
        systemsignals_p ( unsafe::systemsignals * s,
                          object * parent,
                          SIGS... sigs )
            :
              object_p ( s, parent ),
              m_signalset ( io_service (), sigs... )
        {
            m_signalset.async_wait
            ( ::boost::object::detail::asio_handler::wrap
              ( getData (), & systemsignals_p::signalHandler, this )
            );
        }

        void signalHandler ( const boost::system::error_code & ec, int sig )
        {
            BOOST_OBJECT_OO;

            if ( ! ec )
            {
                oo->emitted.emit ( sig );
            }
        }

        void add ( int sig )
        {
            system::error_code ec;
            m_signalset.add ( sig, ec );
            BOOST_OBJECT_LOG_DEBUG_IF(ec) << "Failed to add signal " << sig
                                          << ": " << ec;
        }

        void remove ( int sig )
        {
            system::error_code ec;
            m_signalset.remove ( sig, ec );
            BOOST_OBJECT_LOG_DEBUG_IF(ec) << "Failed to remove signal " << sig
                                          << ": " << ec;
        }

    protected:
        predestruct_t predestruct () override
        {
            boost::system::error_code ec;
            m_signalset.cancel ( ec );

            BOOST_OBJECT_LOG_DEBUG_IF(ec)
                    << "Failed to predestruct systemsignals: " << ec;

            return object_p::predestruct ();
        }
    };

} // namespace detail
} // namespace utils
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_UTILS_IMPL_SYSTEMSIGNALS_P_HPP

