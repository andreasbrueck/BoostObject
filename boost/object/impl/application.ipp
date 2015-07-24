////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_APPLICATION_IPP
#define BOOST_OBJECT_IMPL_APPLICATION_IPP

#include "application_p.hpp"

namespace boost {
namespace object {
namespace unsafe {

    application::application ( QuitBehavior qb )
        :
          object ( *new detail::application_p ( qb, this ) )
    {
        BOOST_ASSERT_MSG
            ( detail::boost_object_installation::globalInstance == nullptr,
              "Only create one application" );

        detail::boost_object_installation::globalInstance = this;
        detail::boost_object_installation::default_base = this;

        BOOST_OBJECT_PP;
        pp->initQuitBehavior ();
    }

    application::application ( detail::application_p & p )
        :
          object ( p )
    {
        BOOST_OBJECT_PP;
        pp->initQuitBehavior ();
    }

    application::~application ()
    {  
#       ifndef BOOST_OBJECT_DISABLE_HIERARCHY
        deleteChildren ();
#       else // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY
        BOOST_OBJECT_PP;
        if ( pp->m_qb_sig ) delete pp->m_qb_sig;
#       endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY

        detail::boost_object_installation::globalInstance = nullptr;
        detail::boost_object_installation::default_base = nullptr;
    }

    void application::_slot_quit ( int exitCode )
    {
        BOOST_OBJECT_PP;
        pp->quit ( exitCode );
    }

    void application::_slot_exit ()
    {
        _slot_quit ( 0 );
    }

    int application::exec ()
    {
        BOOST_OBJECT_PP;
        return pp->exec ();
    }

    void application::setLogHandler ( log_handler_t log_handler )
    {
        *detail::boost_object_installation::log_handler = log_handler;
    }

    application * application::globalInstance ()
    {
        return detail::boost_object_installation::globalInstance;
    }

} // namespace unsafe
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_APPLICATION_IPP
