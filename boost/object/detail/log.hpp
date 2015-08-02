////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_LOG_HPP
#define BOOST_OBJECT_DETAIL_LOG_HPP

#include "config.hpp"
#include "object_install.hpp"

#include <boost/system/error_code.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace boost {
namespace object {
namespace detail {

    template < class TYPE, class ENABLE = void >
    struct log
    {
        static std::string convert ( TYPE val )
        {
            return lexical_cast < std::string > ( val );
        }
    };

    template < class T >
    struct is_format : std::false_type {};

    template < class Ch, class Tr, class Alloc >
    struct is_format < basic_format < Ch, Tr, Alloc > > : std::true_type {};

    template < class TYPE >
    struct log < TYPE,
                 typename std::enable_if < is_format < TYPE >::value >::type >
    {
        static std::string convert ( const TYPE & val )
        {
            return str ( val );
        }
    };

    template <>
    struct log < system::error_code, void >
    {
        static std::string convert ( const system::error_code & ec )
        {
            return str ( format ( "%s (%i)" )
                                  % ec.message ()
                                  % ec.value () );
        }
    };

    class log_scope
    {
    public:
        log_scope ( LogLevel ll, const char * file, int line )
            :
              m_ll ( ll ),
              m_message ( file
                          ? str ( format ( "%s@%i: " ) % file % line )
                          : std::string ()
                        )
        {}

        ~log_scope ()
        {
            boost_object_installation::log_handler->load () ( m_ll, m_message );
        }

        template < typename TYPE >
        log_scope & operator<< ( TYPE&& val )
        {
            m_message += log < typename std::decay < TYPE >::type >::convert
                            ( std::forward < TYPE > ( val ) );

            return *this;
        }

    private:
        LogLevel m_ll;
        std::string m_message;
    };

    class null_log
    {
    public:
        template < typename TYPE >
        const null_log & operator<< ( TYPE&& ) const { return *this; }
    };

} // namespace detail
} // namespace object
} // namespace boost

#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT_LOG(LEVEL) \
    ::boost::object::detail::log_scope(LEVEL,__FILE__,__LINE__)
#else //#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT_LOG(LEVEL) \
    ::boost::object::detail::log_scope(LEVEL,0,0)
#endif //#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT_LOG_IF(COND,LEVEL) \
    if((COND)) BOOST_OBJECT_LOG(LEVEL)

#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT_LOG_DEBUG \
    BOOST_OBJECT_LOG(boost::object::LogDebug)
#else //#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT_LOG_DEBUG \
    ::boost::object::detail::null_log()
#endif //#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT_LOG_ERROR \
    BOOST_OBJECT_LOG(boost::object::LogError)
#define BOOST_OBJECT_LOG_FATAL \
    BOOST_OBJECT_LOG(boost::object::LogFatal)

#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT_LOG_DEBUG_IF(COND) \
    BOOST_OBJECT_LOG_IF(COND,boost::object::LogDebug)
#else //#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT_LOG_DEBUG_IF(COND) \
    ::boost::object::detail::null_log()
#endif //#ifdef BOOST_OBJECT_DEBUG
#define BOOST_OBJECT_LOG_ERROR_IF(COND) \
    BOOST_OBJECT_LOG_IF(COND,boost::object::LogError)
#define BOOST_OBJECT_LOG_FATAL_IF(COND) \
    BOOST_OBJECT_LOG_IF(COND,boost::object::LogFatal)

#endif // BOOST_OBJECT_DETAIL_LOG_HPP

