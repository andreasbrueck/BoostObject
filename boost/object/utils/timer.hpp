////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_UTILS_TIMER_HPP
#define BOOST_OBJECT_UTILS_TIMER_HPP

#include "../detail/config.hpp"
#include "../object.hpp"

namespace boost {
namespace object {
namespace utils {

BOOST_OBJECT_FORWARD_PRIVATE(timer)

BOOST_OBJECT_NS_UNSAFE_START

/**
 * The timer class is a high level interface to repetitive or single shot
 * timers.
 *
 * Whenever the timer fires, the @a timeout() signal is emitted.
 *
 * Example:
 * @code
 * namespace unsafe
 * {
 *     class receiver : public BObject
 *     {
 *         B_OBJECT(receiver)
 *
 *     private:
 *         void _slot_print()
 *         {
 *             std::cout << "timer fired" << std::endl;
 *         }
 *     public:
 *         auto_slot(print)
 *     };
 * }
 *
 * BOOST_SAFE_OBJECT(receiver)
 *
 * int main ()
 * {
 *     BApplication app;
 *
 *     receiver r;
 *
 *     BTimer repeatingTimer,
 *            singleShotTimer ( true );
 *
 *     repeatingTimer.timeout.connect ( r.print );
 *     singleShotTimer.timeout.connect ( app.exit );
 *
 *     repeatingTimer.start ( 1000 );
 *     singleShotTimer.start ( 9000 );
 *
 *     return app.exec ();
 * }
 * @endcode
 */
class timer : public object
{
    B_OBJECT(timer)

protected:
    /**
     * @param singleShot Determines if the timer is repititive (false) or
     *                   single shot (true)
     * @param parent @bobject_parent
     */
    BOOST_OBJECT_DECL explicit timer ( bool singleShot = false,
                                       object * parent = 0 );

public:
    /**
     * @param slot The slot that will be called on timeout
     * @param def Optional default args passed to the connection
     * @param milliseconds Milliseconds till the timer timeouts
     *
     * Creates a single shot timer and automatically deletes it on timeout.
     *
     * Example:
     * @code
     * int main ()
     * {
     *     BApplication app;
     *     BTimer::singleShot ( 5000, app.quit, EXIT_SUCCESS );
     *     return app.exec ();
     * }
     * @endcode
     */
    template < typename SLOT_TYPE, typename... DEFAULT_ARGS >
    static void singleShot ( int milliseconds, SLOT_TYPE & slot,
                             DEFAULT_ARGS&&... def )
    {
        unsafe::timer * t = bobj::createSafe ( true );

        t->timeout.connect ( slot, std::forward < DEFAULT_ARGS > ( def ) ... );
        t->timeout.connect ( t->deleteLater );

        t->start ( milliseconds );
    }

#ifndef BOOST_OBJECT_DOXYGEN

private:
    BOOST_OBJECT_DECL void _slot_start ( int milliseconds );
    BOOST_OBJECT_DECL void _slot_stop ();

    BOOST_OBJECT_DECL void _slot_setSingleShot ( bool isSingleShot );
    BOOST_OBJECT_DECL void _slot_setInterval ( int milliseconds );

private:
    BOOST_OBJECT_DECLARE_PRIVATE(timer)

public:
    BOOST_OBJECT_AUTO_SLOT(start)
    BOOST_OBJECT_AUTO_SLOT(stop)
    BOOST_OBJECT_AUTO_SLOT(setSingleShot)
    BOOST_OBJECT_AUTO_SLOT(setInterval)

    BOOST_OBJECT_SIGNAL(timeout, void())

#else // #ifndef BOOST_OBJECT_DOXYGEN

    /**
     * @param milliseconds Milliseconds till the timer timeouts
     *
     * Starts the timer and will timeout after @p milliseconds.
     *
     * This function has no effect if the timer is already running.
     */
    auto_slot void start ( int milliseconds );

    /**
     * Stops a running timer. If the timer is not already in the timeout state,
     * no @a timeout() signal will be emitted.
     *
     * This function has no effect if the timer is not running.
     */
    auto_slot void stop ();

    /**
     * @param val True for single shot, false for repetitive
     *
     * Sets the timer to single shot or repetitive mode.
     *
     * Can be called while the time is running.
     */
    auto_slot void setSingleShot ( bool val );

    /**
     * @param milliseconds Milliseconds till the timer timeouts
     *
     * Sets the timeout interval for the timer.
     *
     * Can be called while the timer is running. However, it will not alter
     * the running timer, but instead will accept the new value the next time
     * the internal timer is being run again ( which happens automatically
     * if the timer is in repetitive mode ).
     */
    auto_slot void setInterval ( int milliseconds );

    /**
     * This signal emits when the timer times out.
     */
    signal void timeout ();

#endif // #ifndef BOOST_OBJECT_DOXYGEN
};

BOOST_OBJECT_NS_UNSAFE_END

BOOST_SAFE_OBJECT(timer)

} // namespace utils
} // namespace object
} // namespace boost

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_UTILS_TIMER_CPP)
#include "impl/timer.ipp"
#endif

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
/**
 * @brief Global name for boost::object::utils::timer
 */
typedef boost::object::utils::timer BTimer;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#endif // BOOST_OBJECT_UTILS_TIMER_HPP

