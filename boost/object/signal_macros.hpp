////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file signal_macros.hpp */

#ifndef BOOST_OBJECT_SIGNAL_MACROS_HPP
#define BOOST_OBJECT_SIGNAL_MACROS_HPP

#include "detail/config.hpp"

#ifndef BOOST_OBJECT_DOXYGEN
#ifndef BOOST_OBJECT_NO_SIGNAL_MACROS

#define BOOST_OBJECT_SIGNAL(NAME,...) \
    bobj::signal_t < __VA_ARGS__ > NAME = this;

#define BOOST_OBJECT_SIGNAL_TMPL(NAME,...) \
    typename bobj::template signal_t < __VA_ARGS__ > NAME = this;

#ifndef BOOST_OBJECT_NO_SHORT_SIGNAL_MACROS
#define bsignal(NAME,...) BOOST_OBJECT_SIGNAL(NAME,__VA_ARGS__)
#define bsignal_tmpl(NAME,...) BOOST_OBJECT_SIGNAL_TMPL(NAME,__VA_ARGS__)
#endif // BOOST_OBJECT_NO_SHORT_SIGNAL_MACROS

#endif // BOOST_OBJECT_NO_SIGNAL_MACROS

#else // BOOST_OBJECT_DOXYGEN

/**
 * Declares a signal named @p NAME which will evaluate as a function of type
 * @p PROTOTYPE.
 *
 * @param NAME The name of your signal
 * @param PROTOTYPE The function prototype this signal shall have
 *
 * Possible usage:
 * @code
 * BOOST_OBJECT_SIGNAL(foo,void(int))
 * @endcode
 */
#define BOOST_OBJECT_SIGNAL(NAME,PROTOTYPE)

/**
 * Declares a signal named @p NAME which will evaluate as a function of type
 * @p PROTOTYPE.
 *
 * @param NAME The name of your signal
 * @param PROTOTYPE The function prototype this signal shall have
 *
 * Possible usage:
 * @code
 * bsignal(foo,void(int))
 * @endcode
 *
 * Define @a BOOST_OBJECT_NO_SHORT_SIGNAL_MACROS to disable this macro.
 */
#define bsignal(NAME,PROTOTYPE)

#endif // BOOST_OBJECT_DOXYGEN

#endif // BOOST_OBJECT_SIGNAL_MACROS_HPP

