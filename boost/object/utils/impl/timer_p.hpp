////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_UTILS_IMPL_TIMER_P_HPP
#define BOOST_OBJECT_UTILS_IMPL_TIMER_P_HPP

#include "../timer.hpp"
#include "../../detail/asio_wrap.hpp"
#include "../../impl/object_p.hpp"

#include <boost/asio/high_resolution_timer.hpp>

namespace boost {
namespace object {
namespace utils {
namespace detail {

    class timer_p : public ::boost::object::detail::object_p
    {
        BOOST_OBJECT_DECLARE_PUBLIC(timer)

        boost::asio::high_resolution_timer  m_deadlineTimer;

        bool                                m_isRunning;

        int                                 m_interval;
        bool                                m_isSingleShot;

        timer_p ( unsafe::timer * t, object * parent )
            :
              object_p ( t, parent ),
              m_deadlineTimer ( io_service () ),
              m_isRunning ( false ),
              m_interval ( -1 )
        {}

        void timer_wait_handler ( const boost::system::error_code & ec )
        {
            if ( ! ec )
            {
                BOOST_OBJECT_OO;
                oo->timeout.emit ();

                if ( ! m_isSingleShot )
                {
                    startTimer ();
                    return;
                }
            }

            m_isRunning = false;
        }

        void startTimer ()
        {
            m_deadlineTimer.expires_from_now
                    ( std::chrono::milliseconds ( m_interval ) );

            m_deadlineTimer.async_wait
            (
                ::boost::object::detail::asio_handler::wrap
                (
                    getData (),
                    & timer_p::timer_wait_handler,
                    this
                )
            );
        }

    protected:
        predestruct_t predestruct () override
        {
            boost::system::error_code ec;
            m_deadlineTimer.cancel ( ec );

            BOOST_OBJECT_LOG_DEBUG_IF(ec)
                    << "Failed to predestruct timer: " << ec;

            return object_p::predestruct ();
        }
    };


} // namespace detail
} // namespace utils
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_UTILS_IMPL_TIMER_P_HPP

