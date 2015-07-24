////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_ENGINE_HPP
#define BOOST_OBJECT_ENGINE_HPP

#include "detail/config.hpp"

#include "detail/defs/thread.hpp"

namespace boost {
namespace object {

    namespace detail
    {
        class engine_access;
    }

    BOOST_OBJECT_FORWARD_PRIVATE(engine)
    BOOST_OBJECT_FORWARD_PRIVATE(thread)
    BOOST_OBJECT_FORWARD_PRIVATE(application)

    /**
     * The default engine class that implements the eventloop for Boost.Object.
     * You will not need to use this class. Use BApplication, BThread or
     * BThreadpool instead.
     *
     * This class will be moved into the detail namespace in near future.
     */
    class engine
    {
        BOOST_OBJECT_NONCOPYABLE(engine)

    public:
        BOOST_OBJECT_DECL explicit engine ( detail::application_p & );
        BOOST_OBJECT_DECL explicit engine ( detail::thread_p & );
        BOOST_OBJECT_DECL ~ engine ();

        /**
         * Execute the engine. This function blocks indefinitely until
         * @a this->stop() is called.
         */
        BOOST_OBJECT_DECL void exec ();

        /**
         * Execute the engine. This function blocks indefinitely until
         * @a this->stop() is called.
         *
         * @param startedUp Called after the engine is set up and right before
         *                  it enters the event loop.
         */
        BOOST_OBJECT_DECL void exec ( const std::function<void()> & startedUp );

        /**
         * @brief Stops the engine. @a exec() will return shortly after.
         */
        BOOST_OBJECT_DECL void stop ();

        /**
         * @return ThreadId in which @c exec() was called with, or in which
         *         engine was constructed in if not @a exec()'ed yet.
         */
        BOOST_OBJECT_DECL detail::thread_t::id threadId ();

        /**
         * @brief Checks if the thread you're calling this function with is
         *        the thread the associated engine was executed with.
         * @return True if its the same thread, false if its a different thread.
         */
        BOOST_OBJECT_DECL bool isInHomeThread () const;

#   ifndef BOOST_OBJECT_DOXYGEN
    private:
        friend class detail::engine_access;

    private:
        detail::engine_p * data;

#   endif // BOOST_OBJECT_DOXYGEN
    };

} // namespace object
} // namespace boost

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_ENGINE_CPP)
#include "impl/engine.ipp"
#endif

#endif // BOOST_OBJECT_ENGINE_HPP

