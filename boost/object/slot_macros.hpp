////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file slot_macros.hpp */

#ifndef BOOST_OBJECT_SLOT_MACROS_HPP
#define BOOST_OBJECT_SLOT_MACROS_HPP

#include "detail/config.hpp"

#ifndef BOOST_OBJECT_DOXYGEN
#ifndef BOOST_OBJECT_NO_SLOT_MACROS

#include <boost/preprocessor/facilities/overload.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#define BOOST_OBJECT_BOUNDTO(IMPL) \
    ={this,&bobj::object_type::IMPL}

#define BOOST_OBJECT__EXPAND_SLOT_2(SLOT_TYPE,SLOT_NAME,SLOT_FUNC) \
    bobj::SLOT_TYPE< \
    typename boost::object::detail::detail::remove_member_pointer \
    <decltype(&bobj::object_type::SLOT_FUNC)>::type> \
    SLOT_NAME BOOST_OBJECT_BOUNDTO(SLOT_FUNC);

#define BOOST_OBJECT__EXPAND_SLOT_1(SLOT_TYPE,SLOT_NAME) \
    BOOST_OBJECT__EXPAND_SLOT_2(SLOT_TYPE,SLOT_NAME,_slot_##SLOT_NAME)

#define BOOST_OBJECT__EXPAND_SLOT_TMPL_2(SLOT_TYPE,SLOT_NAME,SLOT_FUNC) \
    typename bobj::template SLOT_TYPE< \
    typename boost::object::detail::detail::remove_member_pointer \
    <decltype(&bobj::object_type::SLOT_FUNC)>::type> \
    SLOT_NAME BOOST_OBJECT_BOUNDTO(SLOT_FUNC);

#define BOOST_OBJECT__EXPAND_SLOT_TMPL_1(SLOT_TYPE,SLOT_NAME) \
    BOOST_OBJECT__EXPAND_SLOT_TMPL_2(SLOT_TYPE,SLOT_NAME,_slot_##SLOT_NAME)

#ifdef BOOST_MSVC
#include <boost/preprocessor/facilities/empty.hpp>
#define BOOST_OBJECT__EXPAND_SLOT(SLOT_TYPE,EXPAND_VER,...) \
    BOOST_PP_CAT(BOOST_PP_OVERLOAD(BOOST_OBJECT__EXPAND_SLOT##EXPAND_VER \
    ,__VA_ARGS__) (SLOT_TYPE,__VA_ARGS__),BOOST_PP_EMPTY())
#else // BOOST_MSVC
#define BOOST_OBJECT__EXPAND_SLOT(SLOT_TYPE,EXPAND_VER,...) \
    BOOST_PP_OVERLOAD(BOOST_OBJECT__EXPAND_SLOT##EXPAND_VER,__VA_ARGS__) \
    (SLOT_TYPE,__VA_ARGS__)
#endif // BOOST_MSVC

#define BOOST_OBJECT_ASYNC_SLOT(...) \
    BOOST_OBJECT__EXPAND_SLOT(async_slot_t,_,__VA_ARGS__)
#define BOOST_OBJECT_DIRECT_SLOT(...) \
    BOOST_OBJECT__EXPAND_SLOT(direct_slot_t,_,__VA_ARGS__)
#define BOOST_OBJECT_AUTO_SLOT(...) \
    BOOST_OBJECT__EXPAND_SLOT(auto_slot_t,_,__VA_ARGS__)

#define BOOST_OBJECT_ASYNC_SLOT_TMPL(...) \
    BOOST_OBJECT__EXPAND_SLOT(async_slot_t,_TMPL_,__VA_ARGS__)
#define BOOST_OBJECT_DIRECT_SLOT_TMPL(...) \
    BOOST_OBJECT__EXPAND_SLOT(direct_slot_t,_TMPL_,__VA_ARGS__)
#define BOOST_OBJECT_AUTO_SLOT_TMPL(...) \
    BOOST_OBJECT__EXPAND_SLOT(auto_slot_t,_TMPL_,__VA_ARGS__)

#define BOOST_OBJECT__FOREACH_SLOT_ELEM(R,DATA,ELEM) DATA(ELEM)

#ifndef BOOST_OBJECT_NO_SHORT_SLOT_MACROS
#define async_slot(...) BOOST_OBJECT_ASYNC_SLOT(__VA_ARGS__)
#define async_slot_tmpl(...) BOOST_OBJECT_ASYNC_SLOT_TMPL(__VA_ARGS__)
#define direct_slot(...) BOOST_OBJECT_DIRECT_SLOT(__VA_ARGS__)
#define direct_slot_tmpl(...) BOOST_OBJECT_DIRECT_SLOT_TMPL(__VA_ARGS__)
#define auto_slot(...) BOOST_OBJECT_AUTO_SLOT(__VA_ARGS__)
#define auto_slot_tmpl(...) BOOST_OBJECT_AUTO_SLOT_TMPL(__VA_ARGS__)

#define boundto(IMPL) BOOST_OBJECT_BOUNDTO(IMPL)
#endif // BOOST_OBJECT_NO_SHORT_SLOT_MACROS

#endif // BOOST_OBJECT_NO_SLOT_MACROS

#else // BOOST_OBJECT_DOXYGEN

/**
 * Declare an asynchronous slot named @p NAME.
 * The slot implementation will be @p IMPL.
 *
 * If no parameter @p IMPL is provided, it will default to _slot_NAME
 *
 * Possible usage:
 * @code
 * BOOST_OBJECT_ASYNC_SLOT(foo)     // The slot implementation will be _slot_foo
 * BOOST_OBJECT_ASYNC_SLOT(foo,bar) // The slot implementation is bar
 * @endcode
 */
#define BOOST_OBJECT_ASYNC_SLOT(NAME,IMPL)

/**
 * Declare a direct slot named @p NAME.
 * The slot implementation will be @p IMPL.
 *
 * If no parameter @p IMPL is provided, it will default to _slot_NAME
 *
 * Possible usage:
 * @code
 * BOOST_OBJECT_DIRECT_SLOT(foo)     //The slot implementation will be _slot_foo
 * BOOST_OBJECT_DIRECT_SLOT(foo,bar) //The slot implementation is bar
 * @endcode
 */
#define BOOST_OBJECT_DIRECT_SLOT(NAME,IMPL)

/**
 * Declare an automatic slot named @p NAME.
 * The slot implementation will be @p IMPL.
 *
 * If no parameter @p IMPL is provided, it will default to _slot_NAME
 *
 * Possible usage:
 * @code
 * BOOST_OBJECT_AUTO_SLOT(foo)     //The slot implementation will be _slot_foo
 * BOOST_OBJECT_AUTO_SLOT(foo,bar) //The slot implementation is bar
 * @endcode
 */
#define BOOST_OBJECT_AUTO_SLOT(NAME,IMPL)

/**
 * Declare an asynchronous slot named @p NAME.
 * The slot implementation will be @p IMPL.
 *
 * If no parameter @p IMPL is provided, it will default to _slot_NAME
 *
 * Possible usage:
 * @code
 * async_slot(foo)     //The slot implementation will be _slot_foo
 * async_slot(foo,bar) //The slot implementation is bar
 * @endcode
 *
 * Define @a BOOST_OBJECT_NO_SHORT_SLOT_MACROS to disable this macro.
 */
#define async_slot(NAME,IMPL)

/**
 * Declare a direct slot named @p NAME.
 * The slot implementation will be @p IMPL.
 *
 * If no parameter @p IMPL is provided, it will default to _slot_NAME
 *
 * Possible usage:
 * @code
 * direct_slot(foo)     //The slot implementation will be _slot_foo
 * direct_slot(foo,bar) //The slot implementation is bar
 * @endcode
 *
 * Define @a BOOST_OBJECT_NO_SHORT_SLOT_MACROS to disable this macro.
 */
#define direct_slot(NAME,IMPL)

/**
 * Declare an automatic slot named @p NAME.
 * The slot implementation will be @p IMPL.
 *
 * If no parameter @p IMPL is provided, it will default to _slot_NAME
 *
 * Possible usage:
 * @code
 * auto_slot(foo)     //The slot implementation will be _slot_foo
 * auto_slot(foo,bar) //The slot implementation is bar
 * @endcode
 *
 * Define @a BOOST_OBJECT_NO_SHORT_SLOT_MACROS to disable this macro.
 */
#define auto_slot(NAME,IMPL)

#endif // BOOST_OBJECT_DOXYGEN

#endif // BOOST_OBJECT_SLOT_MACROS_HPP

