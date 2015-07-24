////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_OBJECT_MACROS_HPP
#define BOOST_OBJECT_DETAIL_OBJECT_MACROS_HPP

#include "config.hpp"

#ifndef BOOST_OBJECT_DOXYGEN

#include <boost/preprocessor/variadic/elem.hpp>

#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT__VERIFY_B_OBJECT(...) \
    boost::object::detail::verify_boost_object_classname \
    < __VA_ARGS__ > __boost_object_debug_confirm_classname = this;
#else // BOOST_OBJECT_DEBUG
#define BOOST_OBJECT__VERIFY_B_OBJECT(...)
#endif // BOOST_OBJECT_DEBUG

#define B_OBJECT(...) \
    typedef boost::object::detail::object_interface<__VA_ARGS__> bobj; \
    BOOST_OBJECT__VERIFY_B_OBJECT(__VA_ARGS__)

#define B_PRIVATE_OBJECT(...) B_OBJECT(__VA_ARGS__)

#define BOOST_OBJECT__SAFE_TEMPLATE_LIST(NAMESPACE,FIRST,...) \
    NAMESPACE::FIRST __VA_ARGS__
#define BOOST_SAFE_OBJECT_NS(NAMESPACE,...) \
    using BOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__) = \
    decltype(boost::object::detail::safe_object_helper \
    <BOOST_OBJECT__SAFE_TEMPLATE_LIST(NAMESPACE,__VA_ARGS__)>());
#define BOOST_SAFE_OBJECT(...) BOOST_SAFE_OBJECT_NS(unsafe,__VA_ARGS__)

#endif

#endif // BOOST_OBJECT_DETAIL_OBJECT_MACROS_HPP

