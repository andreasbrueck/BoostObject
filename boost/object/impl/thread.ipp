////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_THREAD_IPP
#define BOOST_OBJECT_THREAD_IPP

#include "thread_p.hpp"

namespace boost {
namespace object {
namespace unsafe {

    thread::thread ( object * parent )
        :
          object ( *new detail::thread_p ( this, parent ) )
    {}

    thread::thread ( detail::thread_p & p )
        :
          object ( p )
    {}

    thread::~thread ()
    {
#       ifndef BOOST_OBJECT_DISABLE_HIERARCHY
        deleteChildren ();
#       endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY

        this->stop ();
        this->wait ();
    }

    void thread::stop ()
    {
        BOOST_OBJECT_PP;
        pp->m_engine.stop ();
    }

    void thread::wait ()
    {
        BOOST_OBJECT_PP;
        pp->m_trd.join ();
    }

    unsigned int thread::idealThreadCount ()
    {
        return detail::thread_t::hardware_concurrency ();
    }

} // namespace unsafe
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_THREAD_IPP
