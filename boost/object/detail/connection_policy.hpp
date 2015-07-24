////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_CONNECTION_POLICY_HPP
#define BOOST_OBJECT_DETAIL_CONNECTION_POLICY_HPP

#include "config.hpp"
#include "slot_context.hpp"

#include <type_traits>
#include <tuple>

namespace boost {
namespace object {
namespace detail {

    template < typename SLOT_TYPE >
    struct ConnectionPolicy
    {
        SLOT_TYPE & m_slot;

        ConnectionPolicy ( SLOT_TYPE & slot )
        : m_slot ( slot ) {}
    };

    template < typename SIG_RET, typename SLOT_TYPE, typename... SIG_ARGS >
    struct DropArgumentsConnectionPolicy : ConnectionPolicy < SLOT_TYPE >
    {
        DropArgumentsConnectionPolicy ( SLOT_TYPE & slot )
        : ConnectionPolicy < SLOT_TYPE > ( slot ) {}

        SIG_RET operator () ( const slot_context & ctx, SIG_ARGS... )
        { return this->m_slot ( ctx ); }
    };

    template < typename SLOT_TYPE, typename... SIG_ARGS >
    struct DropArgumentsConnectionPolicy
    < void, SLOT_TYPE, SIG_ARGS... > : ConnectionPolicy < SLOT_TYPE >
    {
        DropArgumentsConnectionPolicy ( SLOT_TYPE & slot )
        : ConnectionPolicy < SLOT_TYPE > ( slot ) {}

        void operator () ( const slot_context & ctx, SIG_ARGS... )
        { this->m_slot ( ctx ); }
    };

    template < typename SIG_RET, typename SLOT_TYPE, typename... SIG_ARGS >
    struct DirectConnectionPolicy : ConnectionPolicy < SLOT_TYPE >
    {
        DirectConnectionPolicy ( SLOT_TYPE & slot )
        : ConnectionPolicy < SLOT_TYPE > ( slot ) {}

        SIG_RET operator () ( const slot_context & ctx, SIG_ARGS... args )
        { return this->m_slot ( args ..., ctx ); }
    };

    template < typename SLOT_TYPE, typename... SIG_ARGS >
    struct DirectConnectionPolicy
    < void, SLOT_TYPE, SIG_ARGS... > : ConnectionPolicy < SLOT_TYPE >
    {
        DirectConnectionPolicy ( SLOT_TYPE & slot )
        : ConnectionPolicy < SLOT_TYPE > ( slot ) {}

        void operator () ( const slot_context & ctx, SIG_ARGS... args )
        { this->m_slot ( args ..., ctx ); }
    };

    // DummyConnectionPolicy is used to have the compiler only output
    // the static_assert message, nothing more.
    template < typename SIG_RET, typename SLOT_TYPE, typename... SIG_ARGS >
    struct DummyConnectionPolicy : ConnectionPolicy < SLOT_TYPE >
    {
        DummyConnectionPolicy ( SLOT_TYPE & slot )
        : ConnectionPolicy < SLOT_TYPE >( slot ) {}

        SIG_RET operator () ( const slot_context &, SIG_ARGS... )
        { return SIG_RET (); }
    };

    template < typename SLOT_TYPE, typename... SIG_ARGS >
    struct DummyConnectionPolicy
    < void, SLOT_TYPE, SIG_ARGS... > : ConnectionPolicy < SLOT_TYPE >
    {
        DummyConnectionPolicy ( SLOT_TYPE & slot )
        : ConnectionPolicy < SLOT_TYPE > ( slot ) {}

        void operator () ( const slot_context &, SIG_ARGS... )
        {}
    };

    ///////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template < typename SIGNATURE >
        struct signature_traits
        {};

        template < typename RET, typename... ARGS >
        struct signature_traits < RET ( ARGS ... ) >
        {
            typedef RET return_type;
            static const int arg_count = sizeof...(ARGS);
        };

        //////////////////////////////////////////////////////////////////

        template < template < typename, typename, typename... > class TYPE >
        struct DeduceConnectionPolicy_Helper_TypeHolder
        {
            template < typename T1, typename T2, typename... T3 >
            using type = TYPE < T1, T2, T3... >;
        };

        //////////////////////////////////////////////////////////////////

        template < typename SLOT_ARG, typename SIG_ARG >
        struct DeduceConnectionPolicy_CheckSingleArgumentType
        {
            static_assert ( std::is_convertible < SIG_ARG, SLOT_ARG >::value,
                            "Signal/Slot argument incompatible" );

            template < typename SIG_RET, typename SLOT_TYPE,
                       typename... SIG_ARGS >
            using DirectConnectionPolicy_t = DirectConnectionPolicy
            < SIG_RET, SLOT_TYPE, SIG_ARGS... >;
        };

        template < int Index, typename SLOT_TUPLE, typename SIG_TUPLE >
        struct DeduceConnectionPolicy_LoopArgumentTypes
        : DeduceConnectionPolicy_LoopArgumentTypes
          < Index - 1, SLOT_TUPLE, SIG_TUPLE >
        {
            typedef DeduceConnectionPolicy_CheckSingleArgumentType
                    <
                        typename std::tuple_element < Index, SLOT_TUPLE >::type,
                        typename std::tuple_element < Index, SIG_TUPLE >::type
                    >
                    __dummy;
        };

        template < typename SLOT_TUPLE, typename SIG_TUPLE >
        struct DeduceConnectionPolicy_LoopArgumentTypes
        < 0, SLOT_TUPLE, SIG_TUPLE >
        {
            typedef DeduceConnectionPolicy_CheckSingleArgumentType
                    <
                        typename std::tuple_element < 0, SLOT_TUPLE >::type,
                        typename std::tuple_element < 0, SIG_TUPLE >::type
                    >
                    __signalArgCheck;

            template < typename SIG_RET, typename SLOT_TYPE,
                       typename... SIG_ARGS >
            using type = typename __signalArgCheck::template
            DirectConnectionPolicy_t < SIG_RET, SLOT_TYPE, SIG_ARGS... >;
        };

        template < typename SLOT_SIGNATURE, typename SIG_SIGNATURE >
        struct DeduceConnectionPolicy_CheckArgumentTypes
        {};

        template < typename SLOT_RET,
                   typename SIG_RET,
                   typename... SLOT_ARGS,
                   typename... SIG_ARGS
                 >
        struct DeduceConnectionPolicy_CheckArgumentTypes
        < SLOT_RET ( SLOT_ARGS... ), SIG_RET ( SIG_ARGS... ) >
        : DeduceConnectionPolicy_LoopArgumentTypes
          <
            sizeof...(SLOT_ARGS)-1,
            std::tuple < SLOT_ARGS... >,
            std::tuple < SIG_ARGS... >
          >
        {
            static_assert ( sizeof...(SLOT_ARGS) == sizeof...(SIG_ARGS),
                            "unhandled error" );

            static_assert ( sizeof...(SLOT_ARGS) != 0, "unhandled error" );
        };

        //////////////////////////////////////////////////////////////////

        template < int SLOT_ARGC, int SIG_ARGC, typename enable = void >
        struct DeduceConnectionPolicy_CheckArgs
        {
            static_assert ( sizeof(SLOT_ARGC)==0,
                            "Signal/Slot argument count incompatible" );

            template < typename, typename >
            using type = DeduceConnectionPolicy_Helper_TypeHolder
            < DummyConnectionPolicy >;
        };

        template < int ARGC >
        struct DeduceConnectionPolicy_CheckArgs
        < ARGC, ARGC, typename std::enable_if < ARGC != 0 >::type >
        {
            template < typename SLOT_SIGNATURE, typename SIG_SIGNATURE >
            using type = DeduceConnectionPolicy_CheckArgumentTypes
            < SLOT_SIGNATURE, SIG_SIGNATURE >;
        };

        template <>
        struct DeduceConnectionPolicy_CheckArgs < 0, 0, void >
        {
            template < typename, typename >
            using type = DeduceConnectionPolicy_Helper_TypeHolder
            < DirectConnectionPolicy >;
        };

        template < int SIG_ARGC >
        struct DeduceConnectionPolicy_CheckArgs
        < 0, SIG_ARGC, typename std::enable_if < SIG_ARGC != 0 >::type >
        {
            template < typename, typename >
            using type = DeduceConnectionPolicy_Helper_TypeHolder
            < DropArgumentsConnectionPolicy >;
        };

        //////////////////////////////////////////////////////////////////

        template < typename SLOT_SIGNATURE, typename SIG_SIGNATURE >
        struct DeduceConnectionPolicy_CompatibleReturnTypes
        : DeduceConnectionPolicy_CheckArgs
        <
            signature_traits < SLOT_SIGNATURE >::arg_count,
            signature_traits < SIG_SIGNATURE >::arg_count
        >
        ::template type < SLOT_SIGNATURE, SIG_SIGNATURE >
        {};

        //////////////////////////////////////////////////////////////////

        template < typename SIG_RET, typename SLOT_RET, typename enable = void >
        struct DeduceConnectionPolicy_CheckReturnType
        {
            static_assert ( std::is_convertible < SLOT_RET, SIG_RET >::value,
                            "Signal/Slot return type incompatible" );

            template < typename SLOT_SIGNATURE, typename SIG_SIGNATURE >
            using type = DeduceConnectionPolicy_CompatibleReturnTypes
            < SLOT_SIGNATURE, SIG_SIGNATURE >;
        };

        template < typename RET >
        struct DeduceConnectionPolicy_CheckReturnType < RET, RET, void >
        {
            template < typename SLOT_SIGNATURE, typename SIG_SIGNATURE >
            using type = DeduceConnectionPolicy_CompatibleReturnTypes
            < SLOT_SIGNATURE, SIG_SIGNATURE >;
        };

        template < typename SLOT_RET >
        struct DeduceConnectionPolicy_CheckReturnType
        < void, SLOT_RET,
          typename std::enable_if < ! std::is_void < SLOT_RET >::value >::type
        >
        {
            template < typename SLOT_SIGNATURE, typename SIG_SIGNATURE >
            using type = DeduceConnectionPolicy_CompatibleReturnTypes
            < SLOT_SIGNATURE, SIG_SIGNATURE >;
        };

    } // namespace detail

    ///////////////////////////////////////////////////////////////////////////

    template < typename SLOT_SIGNATURE, typename SIG_SIGNATURE >
    struct DeduceConnectionPolicy
    : detail::DeduceConnectionPolicy_CheckReturnType
    <
        typename detail::signature_traits < SIG_SIGNATURE >::return_type,
        typename detail::signature_traits < SLOT_SIGNATURE >::return_type
    >::template type < SLOT_SIGNATURE, SIG_SIGNATURE >
    {

    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_CONNECTION_POLICY_HPP

