////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_THREADPOOL_IPP
#define BOOST_OBJECT_IMPL_THREADPOOL_IPP

#include "threadpool_p.hpp"

namespace boost {
namespace object {
namespace unsafe {

    threadpool::threadpool ( int threads, object * parent )
        :
          object ( *new detail::threadpool_p ( this, threads,
                                             PickLeastUsed, parent ) )
    {}

    threadpool::threadpool ( int threads, PickBehavior pb, object * parent )
        :
          object ( *new detail::threadpool_p ( this, threads, pb, parent ) )
    {}

    void threadpool::_slot_setPickBehavior ( PickBehavior pb )
    {
        BOOST_OBJECT_PP;
        pp->setPickBehavior ( pb );
    }

    thread * threadpool::_slot_getThread ()
    {
        BOOST_OBJECT_PP;
        return pp->getThread ();
    }

} // namespace unsafe
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_THREADPOOL_IPP

