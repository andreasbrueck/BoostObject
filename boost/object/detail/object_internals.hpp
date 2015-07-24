////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_OBJECT_INTERNALS_HPP
#define BOOST_OBJECT_DETAIL_OBJECT_INTERNALS_HPP

#include "config.hpp"

#include <functional>

namespace boost {
namespace asio {
    class io_service;
} // namespace asio
namespace object {

    class object;
    class engine;

namespace detail {

    class object_data;

    class object_internals
    {
    public:
        virtual void post ( const std::function<void()> & handler ) = 0;
        virtual void promisedPost ( const std::function<void()> & handler,
                                    const std::function<void()> & cancel ) = 0;
        virtual bool isInHomeThread () const = 0;
        virtual object_data * getData () const = 0;
        virtual engine & getEngine () = 0;
        virtual void uninstall () = 0;
        virtual boost::asio::io_service & io_service () = 0;

        virtual object * getSender () const = 0;
        virtual void setSender ( object * ) = 0;
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_OBJECT_INTERNALS_HPP

