////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_CONFIG_HPP
#define BOOST_OBJECT_DETAIL_CONFIG_HPP

#include <boost/config.hpp>
#include <boost/assert.hpp>

#ifdef BOOST_NO_CXX11_VARIADIC_MACROS
// If the following line executes, the user probably hasn't read
// that Boost.Object requires a c++11 compiler. Lets use variadic
// macros to notify him.
#error Boost.Object requires a C++11 or later compiler.
#endif

#if !defined(NDEBUG) && !defined(BOOST_OBJECT_NO_DEBUG)
#   ifndef BOOST_OBJECT_DEBUG
#       define BOOST_OBJECT_DEBUG
#   endif
#endif

#if defined(BOOST_OBJECT_DYN_LINK)
#   error Dynamic linking not supported
#endif

#if !defined(BOOST_OBJECT_STATIC_LINK) && !defined(BOOST_OBJECT_DYN_LINK)
#define BOOST_OBJECT_HEADER_ONLY
#endif

#if !defined(BOOST_OBJECT_HEADER_ONLY) && !defined(BOOST_OBJECT_LIBRARY)
#   if !defined(BOOST_OBJECT_NO_AUTOLINK)
#       define BOOST_LIB_NAME boost_object
#       ifdef BOOST_OBJECT_DYN_LINK
#           define BOOST_DYN_LINK
#       endif
#       include <boost/config/auto_link.hpp>
#   endif
#endif

#if defined(BOOST_OBJECT_SHARED_INSTALL) && defined(BOOST_OBJECT_SHARED)
#   error Either define BOOST_OBJECT_SHARED_INSTALL or BOOST_OBJECT_SHARED
#endif

#if !defined(BOOST_OBJECT_DYN_LINK)
#   if defined(BOOST_OBJECT_SHARED_INSTALL)
#      define BOOST_OBJECT_RESOURCE_API BOOST_SYMBOL_EXPORT
#   endif
#   if defined(BOOST_OBJECT_SHARED)
#      define BOOST_OBJECT_RESOURCE_API BOOST_SYMBOL_IMPORT
#   endif
#else
#   if defined(BOOST_OBJECT_SHARED_INSTALL) || defined(BOOST_OBJECT_SHARED)
#       error Do not mix shared and semi-shared build
#   endif
#endif

#if !defined(BOOST_OBJECT_HEADER_ONLY)
#   if defined(BOOST_OBJECT_DYN_LINK)
#       if defined(BOOST_OBJECT_LIBRARY)
#           define BOOST_OBJECT_API BOOST_SYMBOL_EXPORT
#       else
#           define BOOST_OBJECT_API BOOST_SYMBOL_IMPORT
#       endif

#       define BOOST_OBJECT_RESOURCE_API BOOST_OBJECT_API
#   endif
#else
#   define BOOST_OBJECT_DECL inline
#endif

#if !defined(BOOST_OBJECT_DECL)
#define BOOST_OBJECT_DECL
#endif

#if !defined(BOOST_OBJECT_API)
#define BOOST_OBJECT_API
#endif

#if !defined(BOOST_OBJECT_RESOURCE_API)
#   define BOOST_OBJECT_RESOURCE_API
#endif

#if defined(BOOST_OBJECT_DEBUG)
#   if !defined(BOOST_OBJECT_VERIFY_SAFE_OBJECT)
#       define BOOST_OBJECT_VERIFY_SAFE_OBJECT
#   endif
#endif

#if defined(BOOST_OBJECT_DOXYGEN)
#define BOOST_OBJECT_NS_UNSAFE_START
#define BOOST_OBJECT_NS_UNSAFE_END
#else
#define BOOST_OBJECT_NS_UNSAFE_START namespace unsafe {
#define BOOST_OBJECT_NS_UNSAFE_END }
#endif

#ifdef BOOST_OBJECT_ENABLE_PARENT_CHANGE
#undef BOOST_OBJECT_ENABLE_PARENT_CHANGE
#endif

#include "defs/global.hpp"

#endif // BOOST_OBJECT_DETAIL_CONFIG_HPP

