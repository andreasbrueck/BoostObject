////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_THREAD_P_HPP
#define BOOST_OBJECT_IMPL_THREAD_P_HPP

#include "object_p.hpp"
#include "../engine.hpp"
#include "../thread.hpp"

#include <functional>

namespace boost {
namespace object {
namespace detail {

    class thread_p : public object_p
    {
        BOOST_OBJECT_DECLARE_PUBLIC(thread)

        BOOST_OBJECT_STL::thread m_trd;
        engine m_engine;

        thread_p ( unsafe::thread * trd, object * parent )
            :
              object_p ( trd, parent ),
              m_engine ( *this )
        {
            std::promise < bool > p;

            try
            {
                m_trd = BOOST_OBJECT_STL::thread
                        ( std::bind ( & thread_p::exec, this, &p ) );
            }
            catch ( const std::exception & e )
            {
                throw thread_error ( e.what () );
            }

            // thread needs to be running and engine needs to be set up
            // before returning from this constructor
            p.get_future ().get ();
        }

        void exec ( std::promise < bool > * p )
        {
            BOOST_OBJECT_OO;

            m_engine.exec ( [=]() { p->set_value ( true ); } );
            oo->stopped.emit ();
        }

    protected:
        engine & getEngine () override
        {
            return m_engine;
        }
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_THREAD_P_HPP

