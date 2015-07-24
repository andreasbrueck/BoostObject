////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_UTILS_SYSTEMSIGNALS_HPP
#define BOOST_OBJECT_UTILS_SYSTEMSIGNALS_HPP

#include "../detail/config.hpp"
#include "../object.hpp"

#include <signal.h>

namespace boost {
namespace object {
namespace utils {

BOOST_OBJECT_FORWARD_PRIVATE(systemsignals)

BOOST_OBJECT_NS_UNSAFE_START

/**
 * Emits when native signals such as @a SIGTERM or @a SIGINT are delived
 * by your OS.
 *
 * This class is a wrapper around Boost.Asio's signal_set. Please also consult
 * its @asio{reference/signal_set.html,documentation} on how it handles native
 * signals.
 *
 * You can have multiple instances of systemsignals to wait for the same
 * signal number(s).
 */
class systemsignals : public object
{
    B_OBJECT(systemsignals)

protected:
    /**
     * @brief Constructor
     * @param sig1 A native signal to wait for
     * @param parent @bobject_parent
     */
    BOOST_OBJECT_DECL systemsignals ( int sig1, object * parent = 0 );
    /**
     * @brief Constructor
     * @param sig1 A native signal to wait for
     * @param sig2 A native signal to wait for
     * @param parent @bobject_parent
     */
    BOOST_OBJECT_DECL systemsignals ( int sig1, int sig2, object * parent = 0 );
    /**
     * @brief Constructor
     * @param sig1 A native signal to wait for
     * @param sig2 A native signal to wait for
     * @param sig3 A native signal to wait for
     * @param parent @bobject_parent
     */
    BOOST_OBJECT_DECL systemsignals ( int sig1, int sig2, int sig3,
                                      object * parent = 0 );
#   ifndef BOOST_OBJECT_DOXYGEN

private:
    BOOST_OBJECT_DECLARE_PRIVATE(systemsignals)

    BOOST_OBJECT_DECL void _slot_add ( int sig );
    BOOST_OBJECT_DECL void _slot_remove ( int sig );

public:
    BOOST_OBJECT_AUTO_SLOT(add)
    BOOST_OBJECT_AUTO_SLOT(remove)
    BOOST_OBJECT_SIGNAL(emitted,void(int))

#   else // BOOST_OBJECT_DOXYGEN

    /**
     * @param sig The native signal delived by the OS
     *
     * This bobject-signal is emitted when the OS emits one of the native
     * signals you registered to this class.
     */
    signal void emitted ( int sig );

    /**
     * @param sig The native signal number to add to the list
     *
     * Adds @p sig to the list of native signals to wait for.
     * It has no effect if the signal is already in the list.
     */
    auto_slot void add ( int sig );

    /**
     * @param sig The native signal number to remove from the list
     *
     * Removes @p sig from the list of native signals to wait for.
     * It has no effect if the signal is not in the list.
     */
    auto_slot void remove ( int sig );

#   endif // BOOST_OBJECT_DOXYGEN
};

BOOST_OBJECT_NS_UNSAFE_END

BOOST_SAFE_OBJECT(systemsignals)

} // namespace utils
} // namespace object
} // namespace boost

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_UTILS_SYSTEMSIGNALS_CPP)
#include "impl/systemsignals.ipp"
#endif

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
/**
 * @brief Global name for boost::object::utils::systemsignals
 */
typedef boost::object::utils::systemsignals BSystemSignals;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#endif // BOOST_OBJECT_UTILS_SYSTEMSIGNALS_HPP

