////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_UTILS_IMPL_SYSTEMSIGNALS_IPP
#define BOOST_OBJECT_UTILS_IMPL_SYSTEMSIGNALS_IPP

#include "systemsignals_p.hpp"

namespace boost {
namespace object {
namespace utils {
namespace unsafe
{
    systemsignals::systemsignals ( int sig1, object * parent )
        :
          object ( *new detail::systemsignals_p ( this, parent, sig1 ) )
    {}

    systemsignals::systemsignals ( int sig1, int sig2, object * parent )
        :
          object ( *new detail::systemsignals_p ( this, parent, sig1, sig2 ) )
    {}

    systemsignals::systemsignals ( int sig1, int sig2, int sig3,
                                   object * parent )
        :
          object ( *new detail::systemsignals_p ( this, parent,
                                                  sig1, sig2, sig3 ) )
    {}

    void systemsignals::_slot_add ( int sig )
    {
        BOOST_OBJECT_PP;
        pp->add ( sig );
    }

    void systemsignals::_slot_remove ( int sig )
    {
        BOOST_OBJECT_PP;
        pp->remove ( sig );
    }
}
} // namespace utils
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_UTILS_IMPL_SYSTEMSIGNALS_IPP

