////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_OBJECT_DATA_HPP
#define BOOST_OBJECT_DETAIL_OBJECT_DATA_HPP

#include "config.hpp"
#include "connection_interface.hpp"
#include "object_primitive.hpp"

#include "defs/recursive_mutex.hpp"

#include <memory>
#include <set>

namespace boost {
namespace object {

    class base;
    class engine;
    class object;

namespace detail {

    class engine_access;
    class object_locker;
    class engine_p;
    class base_p;

    namespace detail
    {
        template < typename RET, typename... ARGS >
        class asio_wrap;
    }

    template < typename RET, typename... ARGS >
    class signal_impl_base;

    template < typename RET, typename CLASS, typename... ARGS >
    class signal_impl;

    class object_data
    {
        friend class ::boost::object::base;
        friend class ::boost::object::detail::engine_p;
        friend class ::boost::object::detail::base_p;
        friend class ::boost::object::detail::engine_access;
        friend class ::boost::object::detail::object_locker;
        template < typename RET, typename... ARGS >
        friend class ::boost::object::detail::detail::asio_wrap;
        template < typename, typename... >
        friend class ::boost::object::detail::signal_impl_base;
        template < typename, typename, typename... >
        friend class ::boost::object::detail::signal_impl;

#       ifdef BOOST_OBJECT_ENABLE_PARENT_CHANGE
        typedef std::shared_ptr < object_data > ptr_t;

        static inline ptr_t make_ptr_t ( object_data * data )
        {
            return ptr_t ( data );
        }

        static inline ptr_t make_ptr_for_find ( object_data * data )
        {
            return data->shared_from_this ();
        }

#       else
        class maybe_deleter
        {
        public:
            maybe_deleter ( bool doDelete = true )
                :
                  m_doDelete ( doDelete )
            {}

            void operator () ( object_data * data ) const
            {
                if ( m_doDelete )
                    delete data;
            }

        private:
            bool m_doDelete;
        };

        typedef std::unique_ptr < object_data, maybe_deleter > ptr_t;

        static inline ptr_t make_ptr_t ( object_data * data )
        {
            return ptr_t ( data );
        }

        static inline ptr_t make_ptr_for_find ( object_data * data )
        {
            return ptr_t ( data, maybe_deleter ( false ) );
        }

#       endif

        object_data ( object_primitive * owner, engine & eng )
            :
              m_owner ( owner ),
              m_engine ( eng ),
              m_enableConnections ( true )
        {}

        void deleteOwnerIfAlive ()
        {
            recursive_mutex_locker_t l ( m_mutex );

            if ( m_owner != nullptr )
            {
                delete m_owner;
            }

            BOOST_OBJECT_UNUSED(l);
        }

        void registerConnection ( connection_interface * c )
        {
            // protected by connectionMutex
            m_connections.insert ( c );
        }

        void unregisterConnection ( connection_interface * c )
        {
            // protected by connectionMutex

            if ( ! m_enableConnections )
                return;

            auto it = m_connections.find ( c );

            if ( it != m_connections.end () )
            {
                m_connections.erase ( it );
            }
        }

        object * getOwnerObject ()
        {
            return reinterpret_cast < object * > ( m_owner );
        }

        object_primitive * m_owner;
        engine & m_engine;

        recursive_mutex_t m_mutex;

        ///////////////////////////////////

        bool m_enableConnections;
        std::set < connection_interface * > m_connections;
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_OBJECT_DATA_HPP

