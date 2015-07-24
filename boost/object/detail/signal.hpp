////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_SIGNAL_HPP
#define BOOST_OBJECT_DETAIL_SIGNAL_HPP

#include "config.hpp"
#include "func_traits.hpp"
#include "bind_helper.hpp"

#include "object_install.hpp"

#include "connectable_type.hpp"
#include "connection_policy.hpp"

#include "object_access.hpp"
#include "object_data.hpp"

#include "slot_context.hpp"

#include "connection_impl.hpp"
#include "../connection.hpp"

#include "../signalcontext.hpp"

#include "defs/recursive_mutex.hpp"
#include "defs/mutex.hpp"

#include <memory>
#include <type_traits>
#include <forward_list>

namespace boost {
namespace object {
namespace detail {

    template < typename RET, typename... ARGS >
    class signal_impl_base
    {
        typedef connection_impl < RET, ARGS ... > connection_impl_t;
        typedef std::shared_ptr < connection_impl_t >
            connection_impl_ptr_t;

        friend connection_impl_t;

    public:
        template < typename SLOT_TYPE, typename... DEFAULT_ARGS >
        connection connect ( SLOT_TYPE & slot, DEFAULT_ARGS&&... default_args )
        {
            return connect < DeduceConnectionPolicy >
            ( slot, std::forward < DEFAULT_ARGS > ( default_args ) ... );
        }

        template < template < typename, typename > class POLICY,
                   typename SLOT_TYPE,
                   typename... DEFAULT_ARGS >
        connection connect ( SLOT_TYPE & slot, DEFAULT_ARGS&&... default_args )
        {
            static_assert ( is_generic_connectable_type < SLOT_TYPE >::value,
                            "Unconnectable types" );

            typedef typename POLICY < typename SLOT_TYPE::connectable_func_t,
              RET ( ARGS..., DEFAULT_ARGS... ) >::template type
                < RET, SLOT_TYPE, ARGS..., DEFAULT_ARGS... > deduced_policy_t;

            mutex_locker_t l ( *boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            object_data * data = slot.m_obj->getData ();

            if ( ! data->m_enableConnections )
            {
                return connection ();
            }

            for ( auto c : m_connections )
            {
                if ( c->m_slot == & slot )
                {
                    return connection ( c );
                }
            }

            // create connection
            connection_impl_ptr_t connection_ptr
            (
                new connection_impl_t
                (
                    & slot,
                    data,
                    this,
                    bind_by_args < const slot_context &, ARGS... >
                     ( deduced_policy_t ( slot ),
                       std::forward < DEFAULT_ARGS > ( default_args ) ... )
                )
            );

            ++ m_connectionCount;

            // register connection to signal
            m_connections.push_front ( connection_ptr );

            // register connection to slot owner
            data->registerConnection ( connection_ptr.get () );

            return connection ( connection_ptr );
        }

        template < typename SLOT_TYPE >
        bool disconnect ( SLOT_TYPE & slot )
        {
            static_assert ( is_generic_connectable_type < SLOT_TYPE >::value,
                            "Unconnectable types" );

            mutex_locker_t l ( boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            return __disconnect_impl
                    ( & slot, slot.m_interface->__bobj_data () );
        }

    private:
        bool __disconnect_impl ( generic_connectable_type * slot,
                            object_data * data )
        {
            for ( auto it = m_connections.begin (),
                       jt = m_connections.end (),
                       bt = m_connections.before_begin (); it!=jt; ++it, ++bt )
            {
                auto c = *it;

                if ( c->m_slot == slot && ! c->m_disconnected )
                {
                    c->m_disconnected = true;

                    data->unregisterConnection ( c.get () );

                    if ( m_callCount == 0 )
                    {
                        m_connections.erase_after ( bt );
                    }

                    -- m_connectionCount;

                    return true;
                }
            }

            return false;
        }

    protected:
        signal_impl_base ( object * obj )
            :
              m_callCount ( 0 ),
              m_connectionCount ( 0 ),
              m_slot_context ( obj )
        {}

        ~signal_impl_base ()
        {
            mutex_locker_t l ( *boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            for ( auto c : m_connections )
            {
                if ( ! c->m_disconnected )
                {
                    c->m_disconnected = true;
                    c->m_obj->unregisterConnection ( c.get () );
                }
            }
        }

        void __endOfCallOperations ()
        {
            if ( m_callCount == 0 )
            {
                // make already disconnected signals disappear
                m_connections.remove_if ( [=]( const connection_impl_ptr_t & c )
                {
                    return c->m_disconnected;
                });
            }
        }

    protected:
        std::size_t m_callCount;
        std::size_t m_connectionCount;
        std::forward_list < connection_impl_ptr_t > m_connections;
        slot_context m_slot_context;
    };

    ///////////////////////////////////////////////////////////////////////////

    // implement connection_impl from /boost/object/detail/connection_impl.hpp

    template < typename RET, typename... ARGS >
    bool connection_impl < RET, ARGS... >::disconnect ()
    {
        return m_sig->__disconnect_impl ( m_slot, m_obj );
    }

    template < typename RET, typename... ARGS >
    bool connection_impl < RET, ARGS... >::connected ()
    {
        return !m_disconnected;
    }

    ///////////////////////////////////////////////////////////////////////////

    inline bool variadic_and ()
    {
        return true;
    }

    template < typename FIRST, typename... REST >
    inline bool variadic_and ( FIRST f, REST... r )
    {
        return f && variadic_and ( r ... );
    }

    ///////////////////////////////////////////////////////////////////////////

    template < typename RET, typename CLASS, typename... ARGS >
    class signal_impl : public signal_impl_base < RET, ARGS ... >
    {
        friend CLASS;

        typedef std::vector < RET > ret_t;

    public:
        template < typename... SIGCTX >
        inline ret_t emit ( ARGS... args, SIGCTX... ctx )
        {
            return this->operator () ( args ..., ctx ... );
        }

    private:
        signal_impl ( object * obj )
            : signal_impl_base < RET, ARGS ... >( obj ) {}

        template < typename... SIGCTX >
        ret_t operator () ( ARGS... args, SIGCTX... ctx )
        {
            if ( ! this->m_connectionCount ) return ret_t ();

            mutex_locker_t l ( boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            ret_t retdata ( this->m_connectionCount );

            ++ this->m_callCount;

            for ( auto c : this->m_connections )
            {
                if ( c->m_disconnected ) continue;

                object * ownerObject = c->m_obj->getOwnerObject ();
                BOOST_OBJECT_UNUSED(ownerObject); // msvc warns unused ...

                if ( ! variadic_and ( ctx.onAllowDestinationObject
                                      ( ownerObject ) ... ) )
                {
                    continue;
                }

                l.unlock ();

                retdata.push_back ( c->m_func ( this->m_slot_context,
                                                args ... ) );

                l.lock ();
            }

            -- this->m_callCount;

            this->__endOfCallOperations ();

            return retdata;
        }
    };

    template < typename CLASS, typename... ARGS >
    class signal_impl < void, CLASS, ARGS... >
        : public signal_impl_base < void, ARGS ... >
    {
        friend CLASS;

    public:
        template < typename... SIGCTX >
        inline void emit ( ARGS... args, SIGCTX... ctx )
        {
            this->operator () ( args ..., ctx ... );
        }

    private:
        signal_impl ( object * obj )
            : signal_impl_base < void, ARGS ... > ( obj ) {}

        template < typename... SIGCTX >
        void operator () ( ARGS... args, SIGCTX... ctx )
        {
            if ( ! this->m_connectionCount ) return;

            mutex_locker_t l ( *boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            ++ this->m_callCount;

            for ( auto c : this->m_connections )
            {
                if ( c->m_disconnected ) continue;

                object * ownerObject = c->m_obj->getOwnerObject ();
                BOOST_OBJECT_UNUSED(ownerObject); // msvc warns unused ...

                if ( ! variadic_and ( ctx.onAllowDestinationObject
                                      ( ownerObject ) ... ) )
                {
                    continue;
                }

                l.unlock ();

                c->m_func ( this->m_slot_context, args ... );

                l.lock ();
            }

            -- this->m_callCount;

            this->__endOfCallOperations ();
        }
    };

    namespace detail
    {
        template < typename RET, typename... ARGS >
        struct func_traits_signal_delegate
        {
            template < typename CLASS >
            using __intermediate_signal_type =
            signal_impl < RET, CLASS, ARGS ... >;
        };
    }

    template < typename CLASS, typename... SIGNAL_FUNC >
    struct signal_traits
    {
        typedef typename detail::func_traits_delegate
          < detail::func_traits_signal_delegate, SIGNAL_FUNC ... >::template
          __intermediate_signal_type < CLASS > type;
    };


} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_SIGNAL_HPP

