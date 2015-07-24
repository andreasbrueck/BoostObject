////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_UTILS_IMPL_TIMER_IPP
#define BOOST_OBJECT_UTILS_IMPL_TIMER_IPP

#include "timer_p.hpp"

#include "../../detail/asio_wrap.hpp"

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/placeholders.hpp>

#include <functional>

namespace boost {
namespace object {
namespace utils {
namespace unsafe {

    timer::timer ( bool singleShot, object * parent )
        :
          object ( *new detail::timer_p ( this, parent ) )
    {
        BOOST_OBJECT_PP;
        pp->m_isSingleShot = singleShot;
    }

    void timer::_slot_start ( int milliseconds )
    {
        BOOST_OBJECT_PP;

        if ( pp->m_isRunning )
            return;

        pp->m_interval = milliseconds;
        pp->m_isRunning = true;

        pp->startTimer ();
    }

    void timer::_slot_stop ()
    {
        BOOST_OBJECT_PP;

        if ( ! pp->m_isRunning )
            return;

        pp->m_deadlineTimer.cancel ();
    }

    void timer::_slot_setInterval ( int milliseconds )
    {
        BOOST_OBJECT_PP;
        pp->m_interval = milliseconds;
    }

    void timer::_slot_setSingleShot ( bool isSingleShot )
    {
        BOOST_OBJECT_PP;
        pp->m_isSingleShot = isSingleShot;
    }

} // namespace unsafe
} // namespace utils
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_UTILS_IMPL_TIMER_IPP

