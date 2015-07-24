////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_THREADPOOL_HPP
#define BOOST_OBJECT_THREADPOOL_HPP

#include "detail/config.hpp"
#include "object.hpp"

namespace boost {
namespace object {

BOOST_OBJECT_FORWARD_PRIVATE(threadpool)

BOOST_OBJECT_NS_UNSAFE_START

class thread;

/**
 * The threadpool manages multiple threads. Either use @c getThread(),
 * or simply make another bobject the child of this pool to get a thread
 * according to your @c PickBehavior.
 *
 * @par Notes for when you compile with @a BOOST_OBJECT_DISABLE_HIERARCHY:
 *
 * All bobjects assigned to a thread of this pool need to be directly deleted
 * ( using @c delete or they need to go out of scope, @c BObject::deleteLater()
 * is not an option ) before @c threadpool::~threadpool() is invoked. Otherwise
 * it will cause undefined behavior!
 */
class threadpool : public object
{
    B_OBJECT(threadpool)

public:
    /**
     * Defines the behavior with which the next thread will be picked.
     */
    enum PickBehavior
    {
        PickIncrement, /*!< Increment an internal counter that restarts
                            at 0 if bigger than the amount of threads */
        PickLeastUsed  /*!< Pick the thread that has the least amount
                            of bobjects assigned. */
    };

protected:
    /**
     * @param threads Amount of threads this pool shall start
     * @param parent @bobject_parent
     *
     * PickBehavior defaults to PickLeastUsed
     */
    BOOST_OBJECT_DECL threadpool ( int threads, object * parent = 0 );

    /**
     * @param threads Amount of threads this pool shall start
     * @param pb The pickbehavior for this pool
     * @param parent @bobject_parent
     */
    BOOST_OBJECT_DECL threadpool ( int threads,
                                   PickBehavior pb = PickLeastUsed,
                                   object * parent = 0 );

#ifndef BOOST_OBJECT_DOXYGEN

private:
    BOOST_OBJECT_DECLARE_PRIVATE(threadpool)

    BOOST_OBJECT_DECL void _slot_setPickBehavior ( PickBehavior pb );
    BOOST_OBJECT_DECL thread * _slot_getThread ();

public:
    BOOST_OBJECT_AUTO_SLOT(setPickBehavior)
    BOOST_OBJECT_AUTO_SLOT(getThread)

#else // #ifndef BOOST_OBJECT_DOXYGEN

    /**
     * Returns the next thread according to your PickBehavior.
     */
    auto_slot thread * getThread ();

    /**
     * Sets the pick behavior to @p pb.
     */
    auto_slot void setPickBehavior ( PickBehavior pb );

#endif // #ifndef BOOST_OBJECT_DOXYGEN
};

BOOST_OBJECT_NS_UNSAFE_END

BOOST_SAFE_OBJECT(threadpool)

} // namespace object
} // namespace boost

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_THREADPOOL_CPP)
#include "impl/threadpool.ipp"
#endif

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
/**
 * @brief Global name for boost::object::threadpool
 */
typedef boost::object::threadpool BThreadPool;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#endif // BOOST_OBJECT_THREADPOOL_HPP

