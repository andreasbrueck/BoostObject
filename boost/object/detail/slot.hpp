////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_SLOT_HPP
#define BOOST_OBJECT_DETAIL_SLOT_HPP

#include "config.hpp"
#include "func_traits.hpp"
#include "bind_helper.hpp"

#include "object_access.hpp"
#include "object_internals.hpp"
#include "connectable_type.hpp"

#include "sender_scope.hpp"
#include "slot_context.hpp"

#include <functional>
#include <future>

namespace boost {
namespace object {
namespace detail {

    class slot_impl_base
    {
        template < typename, typename... > friend class signal_impl_base;

    public:
        slot_impl_base ( object_internals * obj )
            :
              m_obj ( obj )
        {}

    protected:
        object_internals * m_obj;
    };

    template < typename RET, typename... ARGS >
    class auto_slot_impl
            :
            public connectable_type < RET, ARGS ... >,
            public slot_impl_base
    {
        typedef std::function < RET ( ARGS ... ) > func_t;

    protected:
        auto_slot_impl ( object_internals * obj, func_t && func )
            :
              slot_impl_base ( obj ),
              m_func ( func )
        {}

    public:
        RET operator () ( ARGS... args ) const
        {
            return this->operator () ( args..., slot_context () );
        }

        RET operator () ( ARGS... args, const slot_context & ctx ) const
        {
            static_assert ( std::is_default_constructible < RET >::value,
                            "The return type must be default constructible!");

            if ( ! m_obj->isInHomeThread () )
            {
                std::promise < RET > promise;

                auto setDefaultValToPromise = [&] ()
                {
                    promise.set_value ( RET () );
                };

                auto setSlotValueToPromise =
                [&] ()
                {
                    sender_scope s ( m_obj, ctx );
                    BOOST_OBJECT_UNUSED(s);

                    promise.set_value ( m_func ( args ... ) );
                };

                m_obj->promisedPost ( setSlotValueToPromise,
                                      setDefaultValToPromise );

                std::future < RET > future = promise.get_future ();

                if ( future.valid () )
                {
                    return future.get ();
                }

                return RET ();
            }
            else
            {
                sender_scope s ( m_obj, ctx );
                BOOST_OBJECT_UNUSED(s);

                return m_func ( args ... );
            }
        }

    private:
        func_t m_func;
    };

    template < typename... ARGS >
    class auto_slot_impl < void, ARGS... >
        :
            public connectable_type < void, ARGS ... >,
            public slot_impl_base
    {
        typedef std::function < void ( ARGS ... ) > func_t;
        typedef auto_slot_impl < void, ARGS... > mytype_t;

    protected:
        auto_slot_impl ( object_internals * obj, func_t && func )
            :
              slot_impl_base ( obj ),
              m_func ( func )
        {}

    public:
        void operator () ( ARGS... args ) const
        {
            this->operator () ( args..., slot_context () );
        }

        void operator () ( ARGS... args, const slot_context & ctx ) const
        {
            if ( ! m_obj->isInHomeThread () )
            {
                m_obj->post ( [&](){ this->operator() ( args..., ctx ); } );
            }
            else
            {
                sender_scope s ( m_obj, ctx );
                BOOST_OBJECT_UNUSED(s);

                m_func ( args ... );
            }
        }

    private:
        func_t m_func;
    };

    //////////////////////////////////////////////////////////////////

    template < typename RET, typename... ARGS >
    class async_slot_impl
            : public auto_slot_impl < RET, ARGS... >
    {
    protected:
        template < typename... PARAMS >
        async_slot_impl ( PARAMS&&... params )
            :
              auto_slot_impl < RET, ARGS... >
              ( std::forward < PARAMS > ( params ) ... )
        {}
    };

    template < typename... ARGS >
    class async_slot_impl < void, ARGS... >
        :
            public connectable_type < void, ARGS ... >,
            public slot_impl_base
    {
        typedef std::function < void ( ARGS ... ) > func_t;
        typedef async_slot_impl < void, ARGS... > mytype_t;

    protected:
        async_slot_impl ( object_internals * obj, func_t && func )
            :
              slot_impl_base ( obj ),
              m_func ( func )
        {}

    public:
        void operator () ( ARGS... args ) const
        {
            this->operator () ( args..., slot_context () );
        }

        void operator () ( ARGS... args, const slot_context & ctx ) const
        {
            m_obj->post
            (
                std::bind
                (
                    & mytype_t::async_wrap,
                    this,
                    args ...,
                    ctx
                )
            );
        }

    private:
        void async_wrap ( ARGS... args, const slot_context & ctx ) const
        {
            sender_scope s ( m_obj, ctx );
            BOOST_OBJECT_UNUSED(s);

            m_func ( args ... );
        }

    private:
        func_t m_func;
    };

    //////////////////////////////////////////////////////////////////

    template < typename RET, typename... ARGS >
    class direct_slot_impl
            : public auto_slot_impl < RET, ARGS... >
    {
    protected:
        template < typename... PARAMS >
        direct_slot_impl ( PARAMS&&... params )
            :
              auto_slot_impl < RET, ARGS... >
              ( std::forward < PARAMS > ( params ) ... )
        {}
    };

    template < typename... ARGS >
    class direct_slot_impl < void, ARGS... >
        :
            public connectable_type < void, ARGS ... >,
            public slot_impl_base
    {
        typedef std::function < void ( ARGS ... ) > func_t;

    protected:
        direct_slot_impl ( object_internals * obj, func_t && func )
            :
              slot_impl_base ( obj ),
              m_func ( func )
        {}

    public:
        void operator () ( ARGS... args ) const
        {
            this->operator () ( args..., slot_context () );
        }

        void operator () ( ARGS... args, const slot_context & ctx ) const
        {
            BOOST_OBJECT_UNUSED(ctx);
            m_func ( args ... );
        }

    private:
        func_t m_func;
    };

    //////////////////////////////////////////////////////////////////

    template < typename DELEGATED_SLOT, typename CLASS,
               typename RET, typename... ARGS >
    class slot_holder : public DELEGATED_SLOT
    {
    public:
        slot_holder ( CLASS * obj, RET ( CLASS::*func ) ( ARGS ... ) )
        : DELEGATED_SLOT ( object_access::internals ( obj ),
                           bind_memfnc_by_args < ARGS ... > ( obj, func ) )
        {}

        slot_holder ( CLASS * obj, RET ( CLASS::*func ) ( ARGS ... ) const )
        : DELEGATED_SLOT ( object_access::internals ( obj ),
                           bind_memfnc_by_args < ARGS ... > ( obj, func ) )
        {}
    };

    //////////////////////////////////////////////////////////////////

    namespace detail
    {
        template < typename RET, typename... ARGS >
        struct func_traits_slot_delegate
        {
            template
            <
                template < typename, typename... > class SLOT_TYPE,
                typename SLOT_CLASS
            >
            using __intermediate_slot_type = slot_holder
            <
                SLOT_TYPE < RET, ARGS... >,
                SLOT_CLASS,
                RET,
                ARGS ...
            >;
        };
    }

    template < template < typename, typename... > class SLOT_TYPE,
               typename SLOT_CLASS,
               typename... SLOT_FUNC >
    struct slot_traits
    {
        typedef typename detail::func_traits_delegate
        < detail::func_traits_slot_delegate, SLOT_FUNC ... >::template
            __intermediate_slot_type < SLOT_TYPE, SLOT_CLASS > type;
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_SLOT_HPP

