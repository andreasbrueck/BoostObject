////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_ENGINE_P_HPP
#define BOOST_OBJECT_IMPL_ENGINE_P_HPP

#include "../engine.hpp"

#include "../detail/object_data.hpp"
#include "../detail/bind_helper.hpp"

#include "../detail/defs/mutex.hpp"

#include <memory>
#include <functional>

#include <boost/asio/io_service.hpp>

namespace boost {
namespace object {
namespace detail {

    class promiseHandler
    : public std::enable_shared_from_this < promiseHandler >
    {
    public:
        enum Status
        {
            Pending,
            Cancelled,
            Fullfilled
        };

        promiseHandler
        (
            const std::function < bool ( promiseHandler * ) > & handler,
            const std::function < void () > & cancel
        )
            :
              m_handler ( handler ),
              m_cancel ( cancel ),
              m_status ( Pending )
        {

        }

        void invoke ()
        {
            if ( m_status == Pending )
            {
                m_status = Fullfilled;

                try
                {
                    if ( m_handler ( this ) )
                    {
                        m_status = Fullfilled;
                    }
                    else
                    {
                        cancel ();
                    }
                }
                catch ( ... )
                {
                    // might not have been fullfilled,
                    // cancel to make sure

                    try
                    {
                        cancel ();
                    }
                    catch ( ... )
                    {
                        // well seems like it was already cancelled
                    }

                    throw;
                }
            }
        }

        void cancel ()
        {
            if ( m_status == Pending )
            {
                m_status = Cancelled;

                m_cancel ();
            }
        }

    private:
        const std::function < bool ( promiseHandler * ) >   m_handler;
        const std::function < void () >                     m_cancel;
        Status                                              m_status;
    };

    /////////////////////////////////////////////////////////////////

#   ifndef BOOST_OBJECT_NO_TLS
    struct BOOST_OBJECT_RESOURCE_API fastThreadId
    {
        static inline engine ** get ()
#       if defined(BOOST_OBJECT_SHARED_INSTALL) || !defined(BOOST_OBJECT_SHARED)
        {
            static BOOST_OBJECT_THREAD_LOCAL engine * e = 0;
            return &e;
        }
#       else
        ;
#       endif
    };
#   endif

    class engine_access;

    class engine_p
    {
        friend class boost::object::engine;
        friend class boost::object::detail::engine_access;

        typedef std::shared_ptr < promiseHandler > promiseHandler_ptr_t;

        boost::asio::io_service                         m_io;
        BOOST_OBJECT_STL::thread::id                    m_threadId;
        BOOST_OBJECT_STL::thread::id                    m_createdInThreadId;

        std::set < object_data::ptr_t >                 m_objectDatas;
        std::set < promiseHandler_ptr_t >               m_promiseHandlers;

        mutex_t                                         m_mutex;

        engine *                                        m_engine;

        /////////////////////////////////////////////////////////////////

        engine_p ( engine * e, application_p & )
            :
              m_createdInThreadId ( BOOST_OBJECT_STL::this_thread::get_id () ),
              m_engine ( e )
        {}

        engine_p ( engine * e, thread_p & )
            :
              m_engine ( e )
        {}

        thread_t::id threadId ()
        {
            // expects m_mutex to be locked
            if ( m_threadId == thread_t::id () )
            {
                return m_createdInThreadId;
            }

            return m_threadId;
        }

        bool isInHomeThread ()
        {
            // expects m_mutex to be locked
            return ( threadId () ==
                     BOOST_OBJECT_STL::this_thread::get_id () );
        }

#       ifndef BOOST_OBJECT_NO_TLS
        bool isInHomeThreadFast ()
        {
            engine ** fastId = fastThreadId::get ();

            if ( m_engine == *fastId )
            {
                // we come from a dispatched handler from io_service
                return true;
            }
            else if ( *fastId == 0 )
            {
                // we did not exec yet
                if ( m_createdInThreadId ==
                     BOOST_OBJECT_STL::this_thread::get_id () )
                {
                    return true;
                }
            }

            // in all other cases, we cannot be in the correct thread
            return false;
        }
#       endif

        bool isRunning ()
        {
            // expects m_mutex to be locked
            return ( m_threadId != thread_t::id () );
        }

        bool objectDataExists ( object_data * data, bool eraseIfTrue )
        {
            // expectes m_mutex to be locked

            auto it = m_objectDatas.find
                    ( object_data::make_ptr_for_find ( data ) );

            if ( it != m_objectDatas.end () )
            {
                if ( eraseIfTrue )
                {
                    m_objectDatas.erase ( it );
                }

                return true;
            }

            return false;
        }

        object_data * createObjectData ( object_primitive * owner )
        {
            mutex_locker_t l ( m_mutex );

            object_data * data = new object_data ( owner, *m_engine );
            m_objectDatas.insert ( object_data::make_ptr_t ( data ) );

            BOOST_OBJECT_UNUSED(l);

            return data;
        }

        size_t objectDataCount ()
        {
            mutex_locker_t l ( m_mutex );
            BOOST_OBJECT_UNUSED(l);
            return m_objectDatas.size ();
        }

        void cleanupObjectData ( object_data * data )
        {
            m_io.post ( std::bind
              ( & engine_p::cleanupObjectDataHandler, this, data ) );
        }

        void cleanupObjectDataHandler ( object_data * data )
        {
            mutex_locker_t l ( m_mutex );

            objectDataExists ( data, true );

            BOOST_OBJECT_UNUSED(l);
        }

        void deleteLater ( object_data * data )
        {
            m_io.post ( std::bind
              ( & engine_p::deleteLaterHandler, this, data ) );
        }

        void deleteLaterHandler ( object_data * data )
        {
            bool exists = false;

            {
                mutex_locker_t l ( m_mutex );
                BOOST_ASSERT(isInHomeThread());

                if ( objectDataExists ( data, false ) )
                {
                    exists = true;
                }

                BOOST_OBJECT_UNUSED(l);
            }

            if ( exists )
            {
                data->deleteOwnerIfAlive ();
            }
        }

        void protectedPost
        (
            object_data * data,
            const std::function < void () > & handler
        )
        {
            m_io.post ( std::bind
            ( & engine_p::protectedPostHandler, this, data, handler ) );
        }

        void protectedPromisedPost
        (
            object_data * data,
            const std::function < void () > & handler,
            const std::function < void () > & cancel
        )
        {
            mutex_locker_t l ( m_mutex );

            if ( ! isRunning () )
            {
                // Engine is not running, cancel
                cancel ();
            }
            else
            {
                promiseHandler_ptr_t promise
                (
                    new promiseHandler
                    (
                        std::bind
                        (
                            & engine_p::protectedPromisedPostHandler,
                            this,
                            data,
                            handler,
                            arg < 1 > {}
                        ),
                        cancel
                    )
                );

                m_promiseHandlers.insert ( promise );

                m_io.post ( std::bind (
                                & promiseHandler::invoke, promise ) );
            }

            BOOST_OBJECT_UNUSED(l);
        }

        void protectedPostHandler
        (
            object_data * data,
            const std::function < void () > & handler
        )
        {
            bool exists = false;

            {
                mutex_locker_t l ( m_mutex );
                BOOST_ASSERT(isInHomeThread());

                if ( objectDataExists ( data, false ) )
                {
                    exists = true;
                }

                BOOST_OBJECT_UNUSED(l);
            }

            if ( exists )
            {
                recursive_mutex_locker_t l2 ( data->m_mutex );

                if ( data->m_owner != nullptr )
                {
                    handler ();
                }

                BOOST_OBJECT_UNUSED(l2);
            }
        }

        bool protectedPromisedPostHandler
        (
            object_data * data,
            const std::function < void () > & handler,
            promiseHandler * promise
        )
        {
            bool exists = false;

            {
                mutex_locker_t l ( m_mutex );
                BOOST_ASSERT(isInHomeThread());

                auto it = m_promiseHandlers.find
                        ( promise->shared_from_this () );

                if ( it != m_promiseHandlers.end () )
                {
                    m_promiseHandlers.erase ( it );
                }

                if ( objectDataExists ( data, false ) )
                {
                    exists = true;
                }

                BOOST_OBJECT_UNUSED(l);
            }

            if ( exists )
            {
                recursive_mutex_locker_t l2 ( data->m_mutex );

                if ( data->m_owner != nullptr )
                {
                    handler ();

                    return true;
                }

                BOOST_OBJECT_UNUSED(l2);
            }

            return false;
        }
    };

    /////////////////////////////////////////////////////////////////

    class engine_access
    {
    public:
        static object_data * createObjectData ( engine & e,
                                                object_primitive * o )
        {
            return e.data->createObjectData ( o );
        }

        static size_t objectDataCount ( engine & e )
        {
            return e.data->objectDataCount ();
        }

        static void callProtected ( engine & e, object_data * d,
                                    const std::function < void () > & f )
        {
            e.data->protectedPostHandler ( d, f );
        }

        static boost::asio::io_service & io_service ( engine & e )
        {
            return e.data->m_io;
        }

        static void deleteLater ( engine & e, object_data * d )
        {
            e.data->deleteLater ( d );
        }

        static void protectedPost
        (
            engine & e,
            object_data * d,
            const std::function < void () > & h
        )
        {
            e.data->protectedPost ( d, h );
        }

        static void protectedPromisedPost
        (
            engine & e,
            object_data * d,
            const std::function<void ()> & h,
            const std::function<void ()> & c )
        {
            e.data->protectedPromisedPost ( d, h, c );
        }

        static void cleanupObjectData ( engine & e, object_data * d )
        {
            e.data->cleanupObjectData ( d );
        }
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_ENGINE_P_HPP

