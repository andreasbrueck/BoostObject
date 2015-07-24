////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_BASE_HPP
#define BOOST_OBJECT_BASE_HPP

#include "detail/config.hpp"
#include "detail/object_access.hpp"
#include "detail/object_primitive.hpp"

namespace boost {
namespace object {

    BOOST_OBJECT_FORWARD_PRIVATE(base)

    class object;

    /**
     * @brief The base class for all BObjects.
     *
     * Do not use this class directly, always use boost::object::object instead.
     */
    class base
#   ifndef BOOST_OBJECT_DOXYGEN
    : public detail::object_primitive
#   endif // #ifndef BOOST_OBJECT_DOXYGEN
    {
    public:
        /**
         * Destructor
         */
        BOOST_OBJECT_DECL virtual ~base ();

#       ifndef BOOST_OBJECT_DISABLE_HIERARCHY

        /**
         * Deletes all children of this object.
         */
        BOOST_OBJECT_DECL void deleteChildren ();

#       endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY

#       ifndef BOOST_OBJECT_DOXYGEN

#       ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT
        /**
         * @brief Test if the instance of this object is safe.
         * @return True if safe, false if not.
         */
        virtual bool isSafeObject () = 0;
#       endif // #ifdef BOOST_OBJECT_VERIFY_SAFE_OBJECT

    protected:
        virtual void __dont_create_an_unsafe_object () = 0;

    protected:
        detail::base_p * p;

    private:
        BOOST_OBJECT_NONCOPYABLE(base)

        friend class object;
        friend class detail::base_p;
        friend class detail::object_access;

        BOOST_OBJECT_DECL base ( detail::base_p & p );

        BOOST_OBJECT_DECL void _slot_deleteLater ();

#       endif // #ifndef BOOST_OBJECT_DOXYGEN
    };

} // namespace object
} // namespace boost

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_BASE_CPP)
#include "impl/base.ipp"
#endif

#endif // BOOST_OBJECT_BASE_HPP

