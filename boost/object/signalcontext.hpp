////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_SIGNALCONTEXT_HPP
#define BOOST_OBJECT_SIGNALCONTEXT_HPP

#include "detail/config.hpp"

namespace boost {
namespace object {

    class object;

    namespace detail
    {
        struct signalcontext_base
        {
            virtual bool onAllowDestinationObject ( object * dest ) const
            {
                BOOST_OBJECT_UNUSED(dest);
                return true;
            }
        };
    }

    class signalcontext
    {
    public:
        /**
         * When emitting a signal, pass one or more instances of dontSendTo
         * to prohibit the signal to emit to a specifit bobject.
         */
        struct dontSendTo : detail::signalcontext_base
        {
            /**
             * @brief Constructor of dontSendTo
             * @param obj The bobject you do not want the signal to be emitted
             *        towards.
             */
            dontSendTo ( object * obj ) : m_obj ( obj ) {}

#           ifndef BOOST_OBJECT_DOXYGEN
            const object * const m_obj;

            bool onAllowDestinationObject ( object * dest ) const
            {
                return (dest!=m_obj);
            }
#           endif // BOOST_OBJECT_DOXYGEN
        };

#   ifndef BOOST_OBJECT_DOXYGEN
    private:
        signalcontext () = default;
#   endif // BOOST_OBJECT_DOXYGEN
    };

} // namespace object
} // namespace boost

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
/**
 * @brief Global name for boost::object::signalcontext
 */
typedef boost::object::signalcontext BSignalContext;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#endif // BOOST_OBJECT_SIGNALCONTEXT_HPP

