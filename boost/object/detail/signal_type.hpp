////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_SIGNAL_TYPE_HPP
#define BOOST_OBJECT_DETAIL_SIGNAL_TYPE_HPP

#include "config.hpp"
#include "connectable_type.hpp"

namespace boost {
namespace object {
namespace detail {

    struct generic_signal_type : generic_connectable_type {};

    template < typename RET, typename... ARGS >
    struct signal_type : generic_signal_type
    {
        using signature_t =
#       ifdef BOOST_MSVC
            typename std::remove_pointer < RET (*) ( ARGS ... ) >::type;
#       else
            RET ( ARGS ... );
#       endif
    };

    template < typename TYPE >
    struct is_signal
      :
        std::is_base_of
            < generic_signal_type, TYPE >
    {};

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_SIGNAL_TYPE_HPP

