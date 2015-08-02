////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_APPLICATION_P_HPP
#define BOOST_OBJECT_IMPL_APPLICATION_P_HPP

#include "object_p.hpp"
#include "../application.hpp"
#include "../engine.hpp"
#include "../utils/systemsignals.hpp"

namespace boost {
namespace object {
namespace detail {

    class application_engine_holder
    {
    protected:
        application_engine_holder ( application_p & app )
            :
              m_engine ( app )
        {}

    protected:
        engine m_engine;
    };

    class application_p : public application_engine_holder,
                          public object_p
    {
        BOOST_OBJECT_DECLARE_PUBLIC ( application )

        application_p ( unsafe::application::QuitBehavior qb,
                        unsafe::application * app )
            :
              application_engine_holder ( *this ),
              object_p ( app, m_engine ),
              m_log_handler ( default_log_handler ),
              m_exitCode ( 0 ),
              m_qb ( qb ),
              m_qb_sig ( 0 )
        {
            boost_object_installation::connectionMutex = & m_connectionMutex;
            boost_object_installation::log_handler = & m_log_handler;
        }

        ~application_p ()
        {
            boost_object_installation::connectionMutex = 0;
            boost_object_installation::log_handler = 0;
        }

        int exec ()
        {
            if ( m_engine.threadId ()
                 != BOOST_OBJECT_STL::this_thread::get_id () )
            {
                BOOST_ASSERT_MSG(false,
                                 "You need to exec the application in the "
                                 "same thread as it was created in" );

                std::terminate ();
            }

            m_engine.exec ();

            return m_exitCode;
        }

        void quit ( int exitCode )
        {
            m_exitCode = exitCode;

            m_engine.stop ();
        }

        void initQuitBehavior ()
        {
            BOOST_OBJECT_OO;

            if ( m_qb == unsafe::application::QuitDefault )
            {
                m_qb_sig = new utils::systemsignals ( SIGINT, SIGTERM, oo );
                m_qb_sig->emitted.connect ( oo->quit );
            }
        }

        mutex_t m_connectionMutex;
        std::atomic < log_handler_t > m_log_handler;

        int m_exitCode;

        unsafe::application::QuitBehavior m_qb;
        utils::systemsignals * m_qb_sig;
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_APPLICATION_P_HPP

