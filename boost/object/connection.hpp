////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_CONNECTION_HPP
#define BOOST_OBJECT_CONNECTION_HPP

#include "detail/config.hpp"
#include "detail/connection_interface.hpp"

#include <boost/spirit/home/classic/core/safe_bool.hpp>

#include <memory>

namespace boost {
namespace object {

    namespace detail
    {
        template < typename, typename... >
        class signal_impl_base;
    }

    /**
     * The connection class is returned from the @c connect() function of a
     * signal.
     *
     * You can use it to test if the connection was successful, or to disconnect
     * at a later point of time.
     */
    class connection
#   ifndef BOOST_OBJECT_DOXYGEN
    : public boost::spirit::classic::safe_bool < connection >
#   endif
    {
    public:
        /**
         * @brief Disconnect the connection.
         * @return True if you disconnected the connection, false if it was
         *         already disconnected.
         *
         * The return value is no success indication for the disconnect. It only
         * shows if you call was the cause of the disconnection, or if it
         * already was disconnected before.
         *
         * You can safely assume that a connection is successfully disconnected
         * when calling this function if it wasn't already disconnected,
         * regardless of the returned value.
         */
        BOOST_OBJECT_DECL bool disconnect ();

        /**
         * @return True if connected, false if disconnected.
         */
        BOOST_OBJECT_DECL bool connected () const;

#   ifndef BOOST_OBJECT_DOXYGEN

        BOOST_OBJECT_DECL bool operator_bool () const;

    private:
        template < typename, typename... >
        friend class detail::signal_impl_base;

        typedef detail::connection_interface connection_interface_t;
        typedef std::weak_ptr < connection_interface_t >
          connection_interface_weakptr_t;

        BOOST_OBJECT_DECL connection () = default;
        BOOST_OBJECT_DECL connection ( connection_interface_weakptr_t && i );

    private:
        connection_interface_weakptr_t m_i;

#   else // #ifndef BOOST_OBJECT_DOXYGEN

    private:
        /**
         * Default constructor
         */
        connection ();

    public:
        /**
         * Wrapper for @c connected()
         */
        operator bool () const;

#   endif // #ifndef BOOST_OBJECT_DOXYGEN
    };

    // implementation in impl/object.ipp

} // namespace object
} // namespace boost

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_CONNECTION_CPP)
#include "impl/connection.ipp"
#endif

#endif // BOOST_OBJECT_CONNECTION_HPP

