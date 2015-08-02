////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_ENGINE_IPP
#define BOOST_OBJECT_IMPL_ENGINE_IPP

#include "engine_p.hpp"
#include "../detail/log.hpp"

#include <boost/exception/diagnostic_information.hpp>

namespace boost {
namespace object {

    engine::engine ( detail::application_p & app )
        :
          data ( new detail::engine_p ( this, app ) )
    {}

    engine::engine ( detail::thread_p & trd )
        :
          data ( new detail::engine_p ( this, trd ) )
    {}

    engine::~engine ()
    {
        delete data;
    }

    void engine::exec ()
    {
        exec ( [](){} );
    }

    void engine::exec ( const std::function<void()> & startedUp )
    {
        {
            detail::mutex_locker_t l ( data->m_mutex );

            BOOST_OBJECT_UNUSED(l);

            if ( data->m_threadId != BOOST_OBJECT_STL::thread::id () )
            {
                // exec () was alread executed, return
                return;
            }

            data->m_threadId = BOOST_OBJECT_STL::this_thread::get_id ();
            *detail::fastThreadId::get () = this;
        }

        boost::asio::io_service::work work ( data->m_io );

        BOOST_OBJECT_UNUSED(work);

        startedUp ();

        for ( ;; )
        {
            try
            {
                data->m_io.run ();
                break;
            }
            catch ( ... )
            {
                BOOST_OBJECT_LOG_FATAL
                        << boost::current_exception_diagnostic_information ();

                throw;
            }
        }

        detail::mutex_locker_t l ( data->m_mutex );

        {
            for ( auto promise : data->m_promiseHandlers )
            {
                promise->cancel ();
            }

            data->m_promiseHandlers.clear ();
        }

        data->m_io.reset ();

        BOOST_OBJECT_UNUSED(l);

        // reset thread id
        *detail::fastThreadId::get () = 0;
        data->m_threadId = BOOST_OBJECT_STL::thread::id ();
    }

    void engine::stop ()
    {
        data->m_io.stop ();
    }

    detail::thread_t::id engine::threadId ()
    {
        detail::mutex_locker_t l ( data->m_mutex );

        BOOST_OBJECT_UNUSED(l);

        return data->threadId ();
    }

    bool engine::isInHomeThread () const
    {
#       ifdef BOOST_OBJECT_NO_TLS
        detail::mutex_locker_t l ( data->m_mutex );
        BOOST_OBJECT_UNUSED(l);
        return data->isInHomeThread ();
#       else
        return data->isInHomeThreadFast ();
#       endif
    }

} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_ENGINE_IPP

