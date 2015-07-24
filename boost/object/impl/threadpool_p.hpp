////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_THREADPOOL_P_HPP
#define BOOST_OBJECT_IMPL_THREADPOOL_P_HPP

#include "../threadpool.hpp"
#include "object_p.hpp"
#include "../thread.hpp"

#include <vector>

namespace boost {
namespace object {
namespace detail {

    class threadpool_p : public object_p
    {
        B_PRIVATE_OBJECT ( threadpool_p )
        BOOST_OBJECT_DECLARE_PUBLIC ( threadpool )

        unsafe::threadpool::PickBehavior    m_pb;
        std::vector < thread * >            m_threads;

        size_t                              m_trdInc;

        void setPickBehavior ( unsafe::threadpool::PickBehavior pb )
        {
            m_pb = pb;
        }

        size_t getUsage ( unsafe::thread * trd )
        {
            engine & e = object_access::internals ( trd )->getEngine ();
            return engine_access::objectDataCount ( e );
        }

        thread * getThread ()
        {
            if ( ! m_threads.size () )
            {
                return nullptr;
            }

            thread * trd = nullptr;

            switch ( m_pb )
            {
                case unsafe::threadpool::PickIncrement:
                {
                    if ( ++ m_trdInc > m_threads.size () - 1 )
                    {
                        m_trdInc = 0;
                    }

                    trd = m_threads [ m_trdInc ];
                }
                break;
                case unsafe::threadpool::PickLeastUsed:
                {
                    size_t lastUsage = 0;

                    for ( thread * t : m_threads )
                    {
                        size_t curUsage = getUsage ( t );

                        if ( trd == nullptr || lastUsage > curUsage )
                        {
                            lastUsage = curUsage;
                            trd = t;
                        }
                    }
                }
                break;
            }

            return trd;
        }

        engine * _slot_pickEngine ()
        {
            thread * trd = getThread ();

            if ( ! trd )
            {
                // retrieve the engine assigned to this object
                return & object_p::getEngine ();
            }

            // retrieve the engine assigned to trd object
            return & object_access::internals ( trd )->getEngine ();
        }

    protected:
        threadpool_p ( unsafe::threadpool * tp,
                       int threads,
                       unsafe::threadpool::PickBehavior pb,
                       object * parent )
            :
              object_p ( tp, parent ),
              m_pb ( pb )
        {
            m_threads.reserve ( threads );

            for ( int i = 0; i < threads; ++ i )
            {
                m_threads.push_back ( new thread ( o_ptr () ) );
            }
        }

        engine & getEngine () override
        {
            engine * e = pickEngine ();

            if ( ! e )
            {
                return object_p::getEngine ();
            }

            return *e;
        }

    private:
        BOOST_OBJECT_AUTO_SLOT(pickEngine)
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_THREADPOOL_P_HPP

