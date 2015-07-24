////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_SAFE_OBJECT_HPP
#define BOOST_OBJECT_DETAIL_SAFE_OBJECT_HPP

#include "config.hpp"
#include "object_macros.hpp"
#include "object_access.hpp"

#include <utility>

namespace boost {
namespace object {
namespace detail {

    template < typename OBJ >
    class safe_object : public OBJ
    {
    public:
        template < typename... ARGS >
        safe_object ( ARGS&&... args )
            :
              OBJ ( std::forward < ARGS > ( args ) ... )
        {
        }

        ~ safe_object ()
        {
            object_access::internals ( this )->uninstall ();
        }

#       ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT
        bool isSafeObject () override
        {
            return typeid ( safe_object < OBJ > ) == typeid ( *this );
        }
#       endif // #ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT

    private:
        void __dont_create_an_unsafe_object () override {}
    };

    template < template < typename ... > class OBJ, typename... PARAMS >
    inline safe_object < OBJ < PARAMS ... > >
    safe_object_helper ();

    template < typename OBJ >
    inline safe_object < OBJ >
    safe_object_helper ();

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_SAFE_OBJECT_HPP

