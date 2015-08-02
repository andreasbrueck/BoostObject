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
#include "callable.hpp"
#include "bind_helper.hpp"

#include "object_install.hpp"

#include "connectable_type.hpp"
#include "connection_policy.hpp"

#include "object_access.hpp"
#include "object_data.hpp"

#include "slot_context.hpp"
#include "slot_context.hpp"

#include "connection_impl.hpp"
#include "../connection.hpp"

#include "../signalcontext.hpp"

#include "defs/recursive_mutex.hpp"
#include "defs/mutex.hpp"

#include "log.hpp"

#include <memory>
#include <type_traits>
#include <forward_list>

#include <boost/scope_exit.hpp>

namespace boost {
namespace object {
namespace detail {

    template < typename RET, typename... ARGS >
    class signal_impl_base : public signal_type < RET, ARGS... >
    {
        typedef connection_impl < RET, ARGS ... > connection_impl_t;
        typedef std::shared_ptr < connection_impl_t >
            connection_impl_ptr_t;

        template < typename, typename... >
        friend class signal_impl_base;
        friend connection_impl_t;

    public:

        template < typename FIRST, typename... REST >
        connection connect ( FIRST&& f, REST&&... rest )
        {
            return connect < DeduceConnectionPolicy >
            ( std::forward < FIRST > ( f ),
              std::forward < REST > ( rest ) ... );
        }

        template < template < typename, typename > class POLICY,
                   typename FIRST, typename... REST >
        connection connect ( FIRST&& f, REST&&... rest )
        {
            // TODO nicer errors instead of template soup

            return __connect_impl < POLICY >
            ( std::forward < FIRST > ( f ),
              std::forward < REST > ( rest ) ... );
        }

        template < typename CONN_TYPE >
        bool disconnect ( CONN_TYPE & slot )
        {
            static_assert ( is_generic_connectable_type < CONN_TYPE >::value,
                            "Undisconnectable types" );

            mutex_locker_t l ( *boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            return __disconnect_impl
                    ( & slot, __getObjectData ( slot.m_obj ) );
        }

    private:
        object_data * __getObjectData ( base * obj )
        {
            return __getObjectData ( object_access::internals ( obj ) );
        }

        object_data * __getObjectData ( object_internals * obj )
        {
            return obj->getData ();
        }

        template < template < typename, typename > class POLICY,
                   typename CONN_TYPE, typename... DEFAULT_ARGS >
        typename std::enable_if
        <
            is_generic_connectable_type < CONN_TYPE >::value,
            connection
        >::type
        __connect_impl ( CONN_TYPE & slot, DEFAULT_ARGS&&... default_args )
        {
            typedef typename POLICY < typename CONN_TYPE::signature_t,
              RET ( ARGS..., DEFAULT_ARGS... ) >::template type
                < RET, CONN_TYPE, ARGS..., DEFAULT_ARGS... > deduced_policy_t;

            if ( static_cast<generic_connectable_type*>(this) ==
                 static_cast<generic_connectable_type*>(&slot) )
            {
                BOOST_OBJECT_LOG_DEBUG
                    << "Cannot connect a signal to itself";

                return connection ();
            }

            mutex_locker_t l ( *boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            object_data * data = __getObjectData ( slot.m_obj );

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

        template < template < typename, typename > class POLICY,
                   typename FUNC, typename... DEFAULT_ARGS >
        typename std::enable_if
        <
            is_callable<typename std::remove_reference<FUNC>::type>::value,

            connection
        >::type
        __connect_impl ( FUNC&& f, DEFAULT_ARGS&&... default_args )
        {
            typedef typename POLICY< typename callable_signature
              < typename std::remove_reference < FUNC >::type >::type,
              RET ( ARGS..., DEFAULT_ARGS... ) >::template type
                < RET, FUNC, ARGS..., DEFAULT_ARGS... > deduced_policy_t;

            mutex_locker_t l ( *boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            // create connection
            connection_impl_ptr_t connection_ptr
            (
                new connection_impl_t
                (
                    nullptr,
                    nullptr,
                    this,
                    bind_by_args < const slot_context &, ARGS... >
                     ( deduced_policy_t ( f ),
                       std::forward < DEFAULT_ARGS > ( default_args ) ... )
                )
            );

            ++ m_connectionCount;

            // register connection to signal
            m_connections.push_front ( connection_ptr );

            return connection ( connection_ptr );
        }

        template < template < typename, typename > class POLICY,
                   typename FUNC, typename... DEFAULT_ARGS >
        typename std::enable_if
        <
            is_callable<typename std::remove_reference<FUNC>::type >::value,

            connection
        >::type
        __connect_impl ( boost::object::base * obj, FUNC&& f,
                         DEFAULT_ARGS&&... default_args )
        {
            typedef typename POLICY< typename callable_signature <
              typename std::remove_reference<FUNC>::type >::type,
              RET ( ARGS..., DEFAULT_ARGS... ) >::template type
                < RET, FUNC, ARGS..., DEFAULT_ARGS... > deduced_policy_t;

            mutex_locker_t l ( *boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            object_data * data = object_access::internals ( obj )->getData ();

            if ( ! data->m_enableConnections )
            {
                return connection ();
            }

            // create connection
            connection_impl_ptr_t connection_ptr
            (
                new connection_impl_t
                (
                    nullptr,
                    data,
                    this,
                    bind_by_args < const slot_context &, ARGS... >
                     ( deduced_policy_t ( f ),
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

        bool __disconnect_impl ( connection_impl_t * c, object_data * data )
        {
            for ( auto it = m_connections.begin (),
                       jt = m_connections.end (),
                       bt = m_connections.before_begin (); it!=jt; ++it, ++bt )
            {
                if ( it->get() == c && ! c->m_disconnected )
                {
                    c->m_disconnected = true;

                    if ( data )
                    {
                        data->unregisterConnection ( c );
                    }

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
              m_obj ( obj )
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

                    if ( c->m_obj )
                    {
                        c->m_obj->unregisterConnection ( c.get () );
                    }
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
        object * m_obj;
    };

    ///////////////////////////////////////////////////////////////////////////

    // implement connection_impl from /boost/object/detail/connection_impl.hpp

    template < typename RET, typename... ARGS >
    bool connection_impl < RET, ARGS... >::disconnect ()
    {
        return m_sig->__disconnect_impl ( this, m_obj );
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
        inline ret_t emit ( ARGS... args, SIGCTX const & ... ctx )
        {
            return this->operator () ( args ..., ctx ... );
        }

    private:
        signal_impl ( object * obj )
            : signal_impl_base < RET, ARGS ... >( obj ) {}

        template < typename... SIGCTX >
        ret_t operator () ( ARGS... args, SIGCTX const & ... ctx )
        {
            if ( ! this->m_connectionCount ) return ret_t ();

            mutex_locker_t l ( boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            ret_t retdata ( this->m_connectionCount );

            ++ this->m_callCount;

            BOOST_SCOPE_EXIT_ALL(&)
            {
                -- this->m_callCount;

                this->__endOfCallOperations ();
            };

            slot_context slot_ctx ( this->m_obj );

            for ( auto c : this->m_connections )
            {
                if ( c->m_disconnected ) continue;

                if ( c->m_obj )
                {
                    object * ownerObject = c->m_obj->getOwnerObject ();
                    BOOST_OBJECT_UNUSED(ownerObject); // msvc warns unused ...

                    if ( ! variadic_and ( ctx.onAllowDestinationObject
                                          ( ownerObject ) ... ) )
                    {
                        continue;
                    }
                }

                l.unlock ();

                retdata.push_back ( c->m_func ( slot_ctx, args ... ) );

                l.lock ();
            }

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
        inline void emit ( ARGS... args, SIGCTX const & ... ctx )
        {
            this->operator () ( args ..., ctx ... );
        }

    private:
        signal_impl ( object * obj )
            : signal_impl_base < void, ARGS ... > ( obj ) {}

        template < typename... SIGCTX >
        void operator () ( ARGS... args, SIGCTX const &... ctx )
        {
            if ( ! this->m_connectionCount ) return;

            mutex_locker_t l ( *boost_object_installation::connectionMutex );

            BOOST_OBJECT_UNUSED(l);

            ++ this->m_callCount;

            BOOST_SCOPE_EXIT_ALL(&)
            {
                -- this->m_callCount;

                this->__endOfCallOperations ();
            };

            slot_context slot_ctx ( this->m_obj );

            for ( auto c : this->m_connections )
            {
                if ( c->m_disconnected ) continue;

                if ( c->m_obj )
                {
                    object * ownerObject = c->m_obj->getOwnerObject ();
                    BOOST_OBJECT_UNUSED(ownerObject); // msvc warns unused ...

                    if ( ! variadic_and ( ctx.onAllowDestinationObject
                                          ( ownerObject ) ... ) )
                    {
                        continue;
                    }
                }

                l.unlock ();

                c->m_func ( slot_ctx, args ... );

                l.lock ();
            }
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

