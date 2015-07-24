////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_BASE_IPP
#define BOOST_OBJECT_IMPL_BASE_IPP

#include "base_p.hpp"
#include "../detail/object_access.hpp"

namespace boost {
namespace object {

    base::base ( detail::base_p & p_ )
        :
          p ( & p_ )
    {}

    base::~base ()
    {
        p->uninstall ();

#       ifndef BOOST_OBJECT_DISABLE_HIERARCHY

        BOOST_ASSERT(p->m_parent==nullptr);

        deleteChildren ();

#       endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY

        delete p;
    }

#   ifndef BOOST_OBJECT_DISABLE_HIERARCHY

    void base::deleteChildren ()
    {
        p->deleteChildren ();
    }

#   endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY

    void base::_slot_deleteLater ()
    {
        p->deleteLater ();
    }

    namespace detail
    {
        object_internals * object_access::internals ( base * b )
        {
            return b->p;
        }

        object_internals * object_access::internals ( base_p * b )
        {
            return b;
        }
    }

} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_BASE_IPP

