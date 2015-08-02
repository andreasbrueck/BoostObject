////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_BASE_P_HPP
#define BOOST_OBJECT_IMPL_BASE_P_HPP

#include "../base.hpp"
#include "../detail/object_install.hpp"
#include "../detail/object_data.hpp"
#include "../detail/object_internals.hpp"

#ifdef BOOST_OBJECT_HEADER_ONLY
#include "../engine.hpp"
#endif

#include "engine_p.hpp"

namespace boost {
namespace object {

    class object;

namespace detail {

    class object_p;

    class base_p : public object_internals
    {
        friend class boost::object::base;
        friend class boost::object::object;
        friend class boost::object::detail::object_p;

    protected:
        base_p ( base * obj, base * parent )
            :
              o ( obj ),
              m_isUninstalled ( false )
        {
            BOOST_ASSERT_MSG ( boost_object_installation::default_base
                                       != nullptr,
                                       "Create an application first" );

            if ( parent == nullptr || parent == o )
            {
                parent = boost_object_installation::default_base;
            }

            m_obj_data = engine_access::createObjectData
                            ( parent->p->getEngine (), o );

#           ifndef BOOST_OBJECT_DISABLE_HIERARCHY

            recursive_mutex_locker_t l ( parent->p->m_hierarchyMutex );

            m_parent = parent;
            parent->p->addChild ( o );

            BOOST_OBJECT_UNUSED(l);

#           endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY
        }

        base_p ( base * obj, engine & e )
            :
              o ( obj )
        {
            m_obj_data = engine_access::createObjectData ( e, o );
            m_isUninstalled = false;
#           ifndef BOOST_OBJECT_DISABLE_HIERARCHY
            m_parent = nullptr;
#           endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY
        }

        virtual ~ base_p ()
        {}

        virtual engine & getEngine () override
        {
            return m_obj_data->m_engine;
        }

        boost::asio::io_service & io_service () final override
        {
            return engine_access::io_service ( m_obj_data->m_engine );
        }

        object_data * getData () const final override
        {
            return m_obj_data;
        }

        class predestruct_t
        {
            friend class base_p;
            friend class object_p;

            bool val;

            predestruct_t ( bool b ) : val ( b ) {}

        public:
            predestruct_t () : val ( false ) {}
        };

        virtual predestruct_t predestruct ()
        {
            return predestruct_t ( true );
        }

    protected:
        base * o;

    private:
        object_data * m_obj_data;
        boost::object::object * m_sender;

        bool m_isUninstalled;

#       ifndef BOOST_OBJECT_DISABLE_HIERARCHY

        base * m_parent;
        std::set < base * > m_children;
        recursive_mutex_t m_hierarchyMutex;
        bool m_disableRemoveChild = false;

    private:
        void addChild ( base * child )
        {
            m_children.insert ( child );
        }

        void removeChild ( base * child )
        {
            if ( m_disableRemoveChild )
            {
                return;
            }

            auto it = m_children.find ( child );

            BOOST_ASSERT(it!=m_children.end());

            if ( it != m_children.end () )
            {
                m_children.erase ( it );
            }
        }

        void deleteChildren ()
        {
            recursive_mutex_locker_t l ( m_hierarchyMutex );

            if ( m_children.size () )
            {
                m_disableRemoveChild = true;

                for ( base * child : m_children )
                {
                    delete child;
                }

                m_children.clear ();

                m_disableRemoveChild = false;
            }

            BOOST_OBJECT_UNUSED(l);
        }

#       endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY

        void deleteLater ()
        {
            engine_access::deleteLater ( m_obj_data->m_engine, m_obj_data );
        }

        void post ( const std::function < void () > & handler ) final override
        {
            engine_access::protectedPost ( m_obj_data->m_engine, m_obj_data,
                                           handler );
        }

        void promisedPost ( const std::function<void()> & handler,
                            const std::function<void()> & cancel )final override
        {
            engine_access::protectedPromisedPost ( m_obj_data->m_engine,
                                                   m_obj_data,
                                                   handler, cancel );
        }

        bool isInHomeThread () const final override
        {
            return m_obj_data->m_engine.isInHomeThread ();
        }

        void uninstall () override
        {
            // Disconnect all signals that call a slot owned by this object
            {
                mutex_locker_t l ( *boost_object_installation::connectionMutex );

                m_obj_data->m_enableConnections = false;

                for ( auto c : m_obj_data->m_connections )
                {
                    c->disconnect ();
                }

                BOOST_OBJECT_UNUSED(l);
            }

#           ifndef BOOST_OBJECT_DISABLE_HIERARCHY

            // remove this from parents children list
            if ( m_parent )
            {
                recursive_mutex_locker_t l ( m_parent->p->m_hierarchyMutex );

                m_parent->p->removeChild ( o );
                m_parent = nullptr;

                BOOST_OBJECT_UNUSED(l);
            }

#           endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY

            // prevent destruction while this object is in use, for example a
            // slot is being executed
            recursive_mutex_locker_t l ( m_obj_data->m_mutex );

            // set this object abandoned and queue for cleanup
            m_obj_data->m_owner = nullptr;
            engine_access::cleanupObjectData ( m_obj_data->m_engine,
                                               m_obj_data );

            BOOST_OBJECT_UNUSED(l);

            m_isUninstalled = true;
        }

        object * getSender () const final override
        {
            BOOST_ASSERT(isInHomeThread());
            return m_sender;
        }

        void setSender ( object * sender )
        {
            BOOST_ASSERT(isInHomeThread());
            m_sender = sender;
        }
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_BASE_P_HPP

