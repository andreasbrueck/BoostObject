////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_ASIO_WRAP_HPP
#define BOOST_OBJECT_DETAIL_ASIO_WRAP_HPP

#include "config.hpp"
#include "func_traits.hpp"
#include "bind_helper.hpp"

#include "../impl/engine_p.hpp"

namespace boost {
namespace object {
namespace detail {

    namespace detail
    {
        template < typename RET, typename... ARGS >
        class asio_wrap
        {
            static_assert ( sizeof(RET*)==0,
                            "Non-void return type not supported" );
        };

        template < typename... ARGS >
        class asio_wrap < void, ARGS... >
        {
            typedef std::function < void ( ARGS... ) > func_t;

        public:
            void operator () ( ARGS... args )
            {
                auto f = [&] ()
                {
                    m_func ( args ... );
                };

                engine_access::callProtected ( m_engine, m_data, f );
            }

            asio_wrap ( object_data * data, func_t&& func )
                :
                  m_data ( data ),
                  m_func ( func ),
                  m_engine ( data->m_engine )
            {}

        private:
            object_data * m_data;
            func_t m_func;
            engine & m_engine;
        };

        ///////////////////////////////////////////////////////////////////////

        template < typename RET, typename... ARGS >
        struct compute_signature_holder
        {
            template < typename NEXT >
            using next = compute_signature_holder < RET, ARGS..., NEXT >;

            using signature_t =
#           ifdef BOOST_MSVC
                typename std::remove_pointer < RET (*) ( ARGS... ) >::type
#           else
                RET ( ARGS... )
#           endif
            ;

            typedef asio_wrap < RET, ARGS... > asio_wrap_t;

            template < typename FUNC, typename CLASS, typename... DEFARGS >
            static inline auto bind ( CLASS * c, FUNC&& f, DEFARGS&&... args )
            -> decltype ( bind_memfnc_by_args < ARGS... > ( c, f, args... ) )
            {
                return bind_memfnc_by_args < ARGS... >
                        ( c, f, std::forward < DEFARGS > ( args ) ... );
            }

            template < typename FUNC, typename... DEFARGS >
            static inline auto bind ( FUNC&& f, DEFARGS&&... args )
            -> decltype ( bind_by_args < ARGS... > ( f, args... ) )
            {
                return bind_by_args < ARGS... >
                        ( f, std::forward < DEFARGS > ( args ) ... );
            }
        };

        template < int amount, typename CURRENT, typename FIRST,
                   typename... REST >
        struct compute_signature_helper
        : compute_signature_helper < amount - 1,
                typename CURRENT::template next < FIRST >, REST... >
        {};

        template < typename CURRENT, typename FIRST, typename... ARGS >
        struct compute_signature_helper < 0, CURRENT, FIRST, ARGS... >
        : CURRENT
        {};

        template < int amount, typename SIGNATURE >
        struct compute_signature {};

        template < int amount, typename RET, typename... ARGS >
        struct compute_signature < amount, RET ( ARGS... ) >
        : std::conditional
          <
            ( sizeof...(ARGS) > amount ),
            compute_signature_helper < amount,
                compute_signature_holder < RET >, ARGS... >,
            compute_signature_holder < RET, ARGS... >
          >
          ::type
        {};

        ///////////////////////////////////////////////////////////////////////

        template < typename FUNC, typename CLASS, typename... ARGS >
        struct asio_wrap_helper
        {
            typedef typename remove_member_pointer < FUNC >::type
                member_signature_t;

            typedef func_traits < member_signature_t >
                func_traits_t;

            typedef compute_signature
                < func_traits_t::argc - sizeof... ( ARGS ), member_signature_t >
                compute_signature_t;

            typedef typename compute_signature_t::signature_t
                wrapper_signature_t;

            typedef std::function < wrapper_signature_t >
                wrapper_ret_t;

            typedef typename compute_signature_t::asio_wrap_t
                asio_wrap_t;

        };
    }

    struct asio_handler
    {
        template < typename FUNC, typename CLASS,
                   typename... ARGS >
        static inline auto wrap ( object_data * d, FUNC&& f, CLASS * c,
                                  ARGS&&... args )
        -> typename detail::asio_wrap_helper
            < FUNC, CLASS, ARGS... >::wrapper_ret_t
        {
            typedef detail::asio_wrap_helper < FUNC, CLASS, ARGS... > helper_t;

            return helper_t::compute_signature_t::bind
            (
                typename helper_t::asio_wrap_t
                (
                    d,
                    helper_t::compute_signature_t::bind
                    (
                        c, f, std::forward < ARGS > ( args ) ...
                    )
                )
            );
        }
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_ASIO_WRAP_HPP

