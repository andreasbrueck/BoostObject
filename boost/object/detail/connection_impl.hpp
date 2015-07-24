////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_CONNECTION_IMPL_HPP
#define BOOST_OBJECT_DETAIL_CONNECTION_IMPL_HPP

#include "config.hpp"
#include "connection_interface.hpp"
#include "connectable_type.hpp"
#include "slot_context.hpp"

#include <functional>

namespace boost {
namespace object {
namespace detail {

    template < typename RET, typename... ARGS >
    class signal_impl_base;

    template < typename RET, typename CLASS, typename... ARGS >
    class signal_impl;

    class object_data;

    template < typename RET, typename... ARGS >
    class connection_impl : public connection_interface
    {
        typedef signal_impl_base < RET, ARGS... > signal_impl_base_t;
        typedef std::function < RET ( const slot_context &, ARGS... ) > func_t;
        typedef connection_impl < RET, ARGS ... > mytype_t;

        template < typename, typename, typename... >
        friend class signal_impl;
        friend signal_impl_base_t;

    private:
        BOOST_OBJECT_DECL connection_impl ( generic_connectable_type * slot,
                                            object_data * obj,
                                            signal_impl_base_t * sig,
                                            func_t&& func )
            :
              connection_interface ( & vdisconnect, & vconnected ),
              m_slot ( slot ),
              m_obj ( obj ),
              m_sig ( sig ),
              m_func ( func ),
              m_disconnected ( false )
        {}

        static bool vdisconnect ( connection_interface * i )
        {
            return static_cast < mytype_t * > ( i )->disconnect ();
        }

        static bool vconnected ( connection_interface * i )
        {
            return static_cast < mytype_t * > ( i )->connected ();
        }

        BOOST_OBJECT_DECL bool disconnect ();
        BOOST_OBJECT_DECL bool connected ();

    private:
        generic_connectable_type * m_slot;
        object_data * m_obj;
        signal_impl_base_t * m_sig;
        func_t m_func;
        bool m_disconnected;
    };

    // implementation in signal.hpp

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_CONNECTION_IMPL_HPP

