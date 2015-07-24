////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_DEFS_GLOBAL_HPP
#define BOOST_OBJECT_DETAIL_DEFS_GLOBAL_HPP

#ifndef BOOST_OBJECT_DOXYGEN
#   define BOOST_OBJECT_DOXYGEN_UNSPECIFIED(PARAM) PARAM
#   define BOOST_OBJECT_DOXYGEN_REPLACE(PARAM,REPLACE) PARAM
#   define BOOST_OBJECT_NONCOPYABLE(CLASS) \
        CLASS( const CLASS& ) = delete; \
        CLASS& operator=( const CLASS& ) = delete;
#   ifndef BOOST_OBJECT_LIBRARY
#       define BOOST_OBJECT_HEADER_ONLY
#   endif // #ifndef BOOST_OBJECT_LIBRARY
#else // #ifndef BOOST_OBJECT_DOXYGEN
#   define BOOST_OBJECT_DOXYGEN_UNSPECIFIED(PARAM) unspecified
#   define BOOST_OBJECT_DOXYGEN_REPLACE(PARAM,REPLACE) REPLACE
#   define BOOST_OBJECT_NONCOPYABLE(CLASS)
#endif // #ifndef BOOST_OBJECT_DOXYGEN

////////////////////////////////////////////////////////////////////////////

#if defined(BOOST_OBJECT_USE_BOOST) && defined(BOOST_OBJECT_USE_STL)
#   error Choose to either depend on boost or the standard library
#endif

namespace std {}
namespace boost {
namespace object {
namespace detail {
namespace cfg {
#ifdef BOOST_OBJECT_USE_STL
    namespace stl = ::std;
#else
    namespace stl = ::boost;
#endif
}}}}
#define BOOST_OBJECT_STL ::boost::object::detail::cfg::stl

////////////////////////////////////////////////////////////////////////////

#if defined(BOOST_MSVC) && _MSC_VER <= 1800
#define BOOST_OBJECT_THREAD_LOCAL __declspec(thread)
#else
#define BOOST_OBJECT_THREAD_LOCAL thread_local
#endif

////////////////////////////////////////////////////////////////////////////

#define BOOST_OBJECT_DECLARE_PRIVATE(NAME) \
    inline detail::NAME##_p * p_ptr () \
    { \
        return reinterpret_cast < detail::NAME##_p * > ( p ); \
    } \
    \
    inline const detail::NAME##_p * p_ptr () const \
    { \
        return reinterpret_cast < const detail::NAME##_p * > ( p ); \
    } \
    \
    friend class detail::NAME##_p;

#define BOOST_OBJECT_DECLARE_PUBLIC(NAME) \
    inline unsafe::NAME * o_ptr () \
    { \
        return static_cast < unsafe::NAME * > ( o ); \
    } \
    \
    inline const unsafe::NAME * o_ptr () const \
    { \
        return static_cast < const unsafe::NAME * > ( o ); \
    } \
    \
    friend class unsafe::NAME;

#define BOOST_OBJECT_PP     auto * const pp = p_ptr()
#define BOOST_OBJECT_PPC    const auto * const pp = p_ptr()
#define BOOST_OBJECT_OO     auto * const oo = o_ptr()
#define BOOST_OBJECT_OOC    const auto * const oo = o_ptr()

////////////////////////////////////////////////////////////////////////////

#define BOOST_OBJECT_NOEXCEPT BOOST_NOEXCEPT_OR_NOTHROW

////////////////////////////////////////////////////////////////////////////

#define BOOST_OBJECT_UNUSED(x) ((void)x)

////////////////////////////////////////////////////////////////////////////

#define BOOST_OBJECT_FORWARD_PRIVATE(CLASS) \
    namespace detail        \
    {                       \
        class CLASS##_p;    \
    }

#define BOOST_OBJECT_FORWARD_DETAIL(CLASS) \
    namespace detail    \
    {                   \
        class CLASS;    \
    }

////////////////////////////////////////////////////////////////////////////

#include <string>

namespace boost {
namespace object {

    /**
     * The log level of a log message. The default log handler will terminate
     * the application after a @c LogFatal message was printed out.
     */
    enum LogLevel
    {
        LogDebug,
        LogError,
        LogFatal
    };

    /**
     * Function typedef for log handler functions.
     */
    typedef void ( *log_handler_t ) ( LogLevel, const std::string & );

} // namespace object
} // namespace boost


#endif // BOOST_OBJECT_DETAIL_DEFS_GLOBAL_HPP

