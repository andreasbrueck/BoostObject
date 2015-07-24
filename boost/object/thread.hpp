////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_THREAD_HPP
#define BOOST_OBJECT_THREAD_HPP

#include "detail/config.hpp"
#include "object.hpp"

namespace boost {
namespace object {

BOOST_OBJECT_FORWARD_PRIVATE(thread)

/**
 * @brief The thread_error class
 * 
 * Thrown by boost::object::thread 
 */
class thread_error : public std::exception
{
public:
    thread_error ( const std::string & error )
        :
          m_what ( error )
    {}

    ~thread_error () throw() {}

    const char * what () const throw()
    {
        return m_what.c_str ();
    }

private:
    std::string m_what;
};

BOOST_OBJECT_NS_UNSAFE_START

/**
 * Creates a new thread that can be assigned to a class that inherited
 * from boost::object::base. The thread will automatically be started
 * when this class is constructed.
 *
 * Make sure to not destruct this object in the same thread as the one
 * it creates, because the destructor calls @a this->stop() and
 * @a this->wait() if needed.
 *
 * @par Notes for when you compile with @a BOOST_OBJECT_DISABLE_HIERARCHY:
 *
 * All bobjects assigned to this thread need to be directly deleted ( using
 * @c delete or they need to go out of scope, @c BObject::deleteLater() is not
 * an option ) before @c thread::~thread() is invoked. Otherwise it will cause
 * undefined behavior!
 */
class thread : public object
{
    B_OBJECT(thread)

public:
    /**
     * @brief Constructor
     * @param parent @bobject_parent
     * @throws thread_error when the thread cannot be started
     */
    BOOST_OBJECT_DECL explicit thread ( object * parent = nullptr );

    /**
     * Constructor for internal extensions of this class.
     */
    BOOST_OBJECT_DECL explicit thread ( detail::thread_p & p );

    /**
     * Destructor
     */
    BOOST_OBJECT_DECL ~ thread ();

    /**
     * @brief Stops the thread
     *
     * The thread will exit at the next possible graceful way out.
     *
     * Slots of bobjects assigned to this thread will stop working partially.
     *
     * Threads cannot be restarted.
     */
    BOOST_OBJECT_DECL void stop ();

    /**
     * @brief Blocks until the thread exited.
     */
    BOOST_OBJECT_DECL void wait ();

    /**
     * Returns the ideal number of concurrent threads on your system,
     * or 0 if that information is not available.
     */
    static BOOST_OBJECT_DECL unsigned int idealThreadCount ();

#   ifndef BOOST_OBJECT_DOXYGEN

private:
    BOOST_OBJECT_DECLARE_PRIVATE(thread)

public:
    BOOST_OBJECT_SIGNAL(stopped, void())

#   else // #ifndef BOOST_OBJECT_DOXYGEN

    /**
     * @brief Signal
     *
     * This signal emits after the event loop stopped and the thread
     * is about to exit.
     */
    signal void stopped ();

#   endif // #ifndef BOOST_OBJECT_DOXYGEN
};

BOOST_OBJECT_NS_UNSAFE_END

/**
 * @brief Making boost::object::unsafe::thread threadsafe.
 */
BOOST_SAFE_OBJECT(thread)

} // namespace object
} // namespace boost

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_THREAD_CPP)
#include "impl/thread.ipp"
#endif

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
/**
 * @brief Global name for boost::object::thread
 */
typedef boost::object::thread BThread;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#endif // BOOST_OBJECT_THREAD_HPP

