////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_OBJECT_IPP
#define BOOST_OBJECT_OBJECT_IPP

#include "object_p.hpp"

#ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT
#include "../detail/log.hpp"
#endif // #ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT

namespace boost {
namespace object {

#   ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT
    namespace detail
    {
        BOOST_OBJECT_DECL void checkSafeObject ( object * obj )
        {
            BOOST_OBJECT_LOG_FATAL_IF(!obj->isSafeObject())
                << format ( "Object of type '%s' is not a safe object")
                   %typeid(*obj).name();
        }
    }
#   endif // #ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT

    object::object ( object * parent )
        :
          base ( *new detail::object_p ( this, parent ) )
    {
#       ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT
        detail::object_access::internals ( this )->post
        ( std::bind ( & detail::checkSafeObject, this ) );
#       endif // #ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT
    }

    object::object ( detail::object_p & p )
        :
          base ( p )
    {
#       ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT
        detail::object_access::internals ( this )->post
        ( std::bind ( & detail::checkSafeObject, this ) );
#       endif // #ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT
    }

    object * object::sender () const
    {
        BOOST_OBJECT_PPC;
        return pp->getSender ();
    }

} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_OBJECT_IPP
