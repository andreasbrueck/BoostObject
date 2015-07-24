////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_APPLICATION_HPP
#define BOOST_OBJECT_APPLICATION_HPP

#include "detail/config.hpp"
#include "object.hpp"

namespace boost {
namespace object {

BOOST_OBJECT_FORWARD_PRIVATE(application)

BOOST_OBJECT_NS_UNSAFE_START

    /**
     * Every Boost.Object application needs to create this class once,
     * before any other objects inherited by boost::object::object are
     * created. Example:
     * @code
     * #include <boost/object/application.hpp>
     *
     * BOOST_OBJECT_INSTALL
     *
     * int main ( int argc, char ** argv )
     * {
     *     boost::object::application app;
     *
     *     // .
     *     // .
     *     // .
     *
     *     return app.exec ();
     * }
     * @endcode
     * The application class will serve the main eventloop and will be the
     * parent for every following class inherited by boost::object::object,
     * unless you specify a different parent via its constructor.
     *
     * @par Notes for when you compile with @a BOOST_OBJECT_DISABLE_HIERARCHY:
     *
     * If you inherit from this class, and create bobjects ( classes that are
     * directly or indirectly derived from BObject ), you have to directly
     * delete them ( using @c delete or they need to go out of scope,
     * @c BObject::deleteLater() is not an option ) before
     * @c application::~application() is invoked. Otherwise it will cause
     * undefined behavior!
     *
     * Infact, all bobjects need to be deleted before
     * @c application::~application() is invoked.
     *
     */
    class application : public object
    {
        B_OBJECT(application)

    public:
        /** Defines when exec() will return */
        enum QuitBehavior
        {
            QuitDefault,    /** @a exec() will return if:
                                @li @a quit() is called
                                @li @a SIGINT or @a SIGTERM is emitted */

            QuitManual      /** @a exec() will only return if you manually
                                call @a quit() */
        };

        BOOST_OBJECT_DECL ~ application ();

        /**
         * @brief Enters the event loop
         * @return ExitCode given to @a this->quit()
         *
         * This call blocks indefinitely, until @a this->quit() is called.
         */
        BOOST_OBJECT_DECL int exec ();

        /**
         * Sets your custom log handler.
         */
        BOOST_OBJECT_DECL void setLogHandler ( log_handler_t log_handler );

        /**
         * @brief Access application from anywhere
         * @return Pointer to global instance of
         *         boost::object::application,
         *         @a nullptr if no application was created.
         */
        BOOST_OBJECT_DECL static application * globalInstance ();

    protected:
        /**
         * @brief Constructor
         * @param qb The QuitBehavior of this application
         */
        BOOST_OBJECT_DECL explicit application ( QuitBehavior qb =
                                                 QuitDefault );

        /**
         * Constructor for internal extensions of this class.
         */
        BOOST_OBJECT_DECL explicit application ( detail::application_p & );

#   ifndef BOOST_OBJECT_DOXYGEN

    private:
        BOOST_OBJECT_DECLARE_PRIVATE(application)

        BOOST_OBJECT_DECL void _slot_quit ( int exitCode );
        BOOST_OBJECT_DECL void _slot_exit ();

    public:
        BOOST_OBJECT_ASYNC_SLOT(quit)
        BOOST_OBJECT_ASYNC_SLOT(exit)

#   else
    public:
        /**
         * @param exitCode The exitCode to be returned by @a this->exec()
         *
         * Makes @a this->exec() return with the specified exitCode
         */
        async_slot void quit ( int exitCode );

        /**
         * Calls @c quit() with an exitCode of 0.
         */
        async_slot void exit ();
#   endif
    };

BOOST_OBJECT_NS_UNSAFE_END

/**
 * @brief Making boost::object::unsafe::application threadsafe.
 */
BOOST_SAFE_OBJECT ( application )

} // namespace object
} // namespace boost

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
/**
 * @brief Global name for boost::object::unsafe::application
 */
typedef boost::object::application BApplication;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_APPLICATION_CPP)

#if !defined(BOOST_OBJECT_SHARED)
#define BOOST_OBJECT_INSTALL \
    namespace boost { \
    namespace object { \
    namespace detail { \
        base * boost_object_installation::default_base = nullptr; \
        unsafe::application * boost_object_installation::globalInstance \
            = nullptr; \
        mutex_t * boost_object_installation::connectionMutex; \
        std::atomic < log_handler_t > * boost_object_installation::log_handler;\
    }}}
#endif

#include "impl/application.ipp"
#endif

#if !defined(BOOST_OBJECT_INSTALL)
#define BOOST_OBJECT_INSTALL // no-op
#endif

/**
 * @def BOOST_OBJECT_INSTALL
 * @brief Installs static dependencies to use Boost.Object header only.
 *
 */

/**
 * Short macro to access the global instance of
 * boost::object::unsafe::application from anywhere
 */
#define bApp ::boost::object::unsafe::application::globalInstance ()

#endif // BOOST_OBJECT_APPLICATION_HPP

