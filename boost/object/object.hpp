////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_OBJECT_HPP
#define BOOST_OBJECT_OBJECT_HPP

#include "base.hpp"

#include "detail/safe_object.hpp"
#include "detail/object_interface.hpp"

#include "slot_macros.hpp"
#include "signal_macros.hpp"

namespace boost {
namespace object {

    BOOST_OBJECT_FORWARD_PRIVATE(object)

    /**
     * The base class to inherit from for every class that wants to enable
     * Boost.Object signal/slot mechanisms. Example:
     * @code
     * #include <boost/object/object.hpp>
     *
     * class myClass : public boost::object::object
     * {
     *     B_OBJECT(myClass)
     *
     * protected:
     *     myClass ( boost::object::base * parent = nullptr )
     *         :
     *           base ( parent )
     *     {}
     *
     * private:
     *     void _slot_finish ( int exitCode )
     *     {
     *         bApp->quit ( exitCode );
     *     }
     *
     * public:
     *     auto_slot(finish)
     * };
     * @endcode
     */
    class object : public base
    {
        B_OBJECT(object)

    public:
        /**
         * When the execution of a slot implementation is caused by
         * signal emission, the returned value is the bobject that owns
         * the signal. Returns 0 if a signal emission was not the cause,
         * or when not called from inside a slot implementation.
         *
         * Warning: A direct slot returning void will not update the value
         * returned by this function!
         */
        BOOST_OBJECT_DECL object * sender () const;

    protected:
        /**
         * @brief Constructor
         * @param parent Makes this instance the child of @p parent
         *
         * If @p parent equals @a nullptr, @p parent will be internally set to
         * your BApplication instance.
         */
        BOOST_OBJECT_DECL object ( object * parent = nullptr );

        /**
         * Constructor for internal extensions of this class.
         */
        BOOST_OBJECT_DECL object ( detail::object_p & p );

#   ifndef BOOST_OBJECT_DOXYGEN
    private:
        BOOST_OBJECT_DECLARE_PRIVATE(object)

    public:
        BOOST_OBJECT_DIRECT_SLOT(deleteLater)
        BOOST_OBJECT_SIGNAL(destroyed,void(object*))

#   else // #ifndef BOOST_OBJECT_DOXYGEN
    public:
        /**
         * Queues this object for deletion. It is save to call for the object
         * that you're currently in ( this->deleteLater() ).
         *
         * Note that it will not cancel any asynchronous operation for this
         * object that is currently already in queue waiting to be dispatched.
         */
         async_slot void deleteLater (); // mark as async_slot for documentation
                                         // to avoid confusion with the
                                         // description of this function

        /**
         * This signal emits when an objects is about to be deleted.
         *
         * CAUTION: Depending on your slot type, @p obj might not live anymore
         * by the time your slot implementation is called. If you don't know
         * the internals of Boost.Object, it is generally discouraged to access
         * any memory pointed to by @p obj
         */
        signal void destroyed ( base * obj );

#   endif // #ifndef BOOST_OBJECT_DOXYGEN
    };

} // namespace object
} // namespace boost

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
/**
 * @brief Global name for boost::object::object
 */
typedef boost::object::object BObject;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_OBJECT_CPP)
#include "impl/object.ipp"
#endif

#ifdef BOOST_OBJECT_DOXYGEN

/**
 * This macro defines dependencies for your class that other utilities of
 * Boost.Object ( signal/slots/etc ) need.
 *
 * @param CLASSNAME The type of your class
 *
 * Possible use:
 * @code
 * class myClass1 : public BObject
 * {
 *     B_OBJECT(myClass1)
 *
 *     // ..
 * };
 *
 * template < typename TARG1, typename TARG2, typename... TARGREST >
 * class myClass2 : public BObject
 * {
 *     B_OBJECT(myClass2<TARG1,TARG2,TARGREST...>)
 *
 *     // ..
 * };
 * @endcode
 */
#define B_OBJECT(CLASSNAME)

/**
 * This macro helps you define a safe version of your bobject.
 *
 * @param CLASSNAME The name of your class nested in namespace unsafe
 * @param ... All template parameters required for your class, if any.
 *
 * Possible use:
 * @code
 * namespace unsafe
 * {
 *     class myObject : public BObject {};
 *
 *     template < typename TARG1, typename... TARGREST >
 *     class myOtherObject : public BObject {};
 * }
 *
 * BOOST_SAFE_OBJECT(myObject)
 *
 * template < typename TARG1, typename... TARGREST >
 * BOOST_SAFE_OBJECT(myOtherObject,TARG1,TARGREST...)
 * @endcode
 *
 * If you want to use a different nested namespace, use BOOST_SAFE_OBJECT_NS
 */
#define BOOST_SAFE_OBJECT(CLASSNAME,...)

/**
 * This macro helps you define a safe version of your bobject.
 *
 * @param NAMESPACE The namespace your class is nested in
 * @param CLASSNAME The name of your class nested in namespace @p NAMESPACE
 * @param ... All template parameters required for your class, if any.
 *
 * Possible use:
 * @code
 * namespace myownunsafenamespace
 * {
 *     class myObject : public BObject {};
 *
 *     template < typename TARG1, typename... TARGREST >
 *     class myOtherObject : public BObject {};
 * }
 *
 * BOOST_SAFE_OBJECT_NS(myownunsafenamespace,myObject)
 *
 * template < typename TARG1, typename... TARGREST >
 * BOOST_SAFE_OBJECT_NS(myownunsafenamespace,myOtherObject,TARG1,TARGREST...)
 * @endcode
 */
#define BOOST_SAFE_OBJECT_NS(NAMESPACE,CLASSNAME,...)

/**
 * @a bobj is a typedef inside of your @a B_OBJECT macro.
 *
 * This struct bobj shall attempt to document the real templated code as good
 * as possible.
 *
 * The members are all private, only accessible from the class which declared
 * @a B_OBJECT
 */
struct bobj
{
    /**
     * @tparam FUNCTION_SIGNATURE The signature of the slot.
     *         Can be a function signature or a list of parameters, the first
     *         being the return type:
     *         @li @a auto_slot_t<void(int)>
     *         @li @a auto_slot_t<void> Evaluates to @a auto_slot_t<void()>
     *         @li @a auto_slot_t<void,int> Evaluates to @a auto_slot_t<void(int)>
     *         @li @a auto_slot_t<> Evaluates to @a auto_slot_t<void()>
     */
    template < typename... FUNCTION_SIGNATURE >
    class auto_slot_t
    {
    public:
        /**
         * @brief Calls the slot implementation in a manner depending from which
         *        thread you call this function from.
         * @tparam RET Deduced from @a FUNCTION_SIGNATURE
         * @tparam ARGS Deduced from @a FUNCTION_SIGNATURE
         */
        RET operator () ( ARGS... args );
    };

    /**
     * @tparam FUNCTION_SIGNATURE The signature of the slot.
     *         Can be a function signature or a list of parameters, the first
     *         being the return type:
     *         @li @a async_slot_t<void(int)>
     *         @li @a async_slot_t<void> Evaluates to @a async_slot_t<void()>
     *         @li @a async_slot_t<void,int> Evaluates to @a async_slot_t<void(int)>
     *         @li @a async_slot_t<> Evaluates to @a async_slot_t<void()>
     */
    template < typename... FUNCTION_SIGNATURE >
    class async_slot_t
    {
    public:
        /**
         * @brief Calls the slot implementation in an asynchronous manner.
         * @tparam RET Deduced from @a FUNCTION_SIGNATURE
         * @tparam ARGS Deduced from @a FUNCTION_SIGNATURE
         */
        RET operator () ( ARGS... args );
    };

    /**
     * @tparam FUNCTION_SIGNATURE The signature of the slot.
     *         Can be a function signature or a list of parameters, the first
     *         being the return type:
     *         @li @a direct_slot_t<void(int)>
     *         @li @a direct_slot_t<void> Evaluates to @a direct_slot_t<void()>
     *         @li @a direct_slot_t<void,int> Evaluates to @a direct_slot_t<void(int)>
     *         @li @a direct_slot_t<> Evaluates to @a direct_slot_t<void()>
     */
    template < typename... FUNCTION_SIGNATURE >
    class direct_slot_t
    {
    public:
        /**
         * @brief Calls the slot implementation directly.
         * @tparam RET Deduced from @a FUNCTION_SIGNATURE
         * @tparam ARGS Deduced from @a FUNCTION_SIGNATURE
         */
        RET operator () ( ARGS... args );
    };

    /**
     * @tparam FUNCTION_SIGNATURE The signature of the signal.
     *         Can be a function signature or a list of parameters, the first
     *         being the return type:
     *         @li @a signal_t<void(int)>
     *         @li @a signal_t<void> Evaluates to @a signal_t<void()>
     *         @li @a signal_t<void,int> Evaluates to @a signal_t<void(int)>
     *         @li @a signal_t<> Evaluates to @a signal_t<void()>
     */
    template < typename... FUNCTION_SIGNATURE >
    class signal_t
    {
    private:
        /**
         * @brief Emits the signal.
         * @tparam RET Deduced from @a FUNCTION_SIGNATURE
         * @tparam ARGS Deduced from @a FUNCTION_SIGNATURE
         * @tparam CONTEXT Zero or more instances of subclasses from
         *                 boost::object::signalcontext
         *
         * This member is private, having your bobject as friend.
         */
        template < typename... CONTEXT >
        RET operator () ( ARGS... args, CONTEXT & ctx );

    public:
        /**
         * @brief Connects a slot to this signal.
         * @tparam DEFAULT_ARGS
         * @param slot The slot you want to connect to this signal
         */
        template < class SLOT_TYPE, class... DEFAULT_ARGS >
        boost::object::connection connect ( SLOT_TYPE & slot, DEFAULT_ARGS... );

        /**
         * This is a public wrapper around the private operator().
         */
        template < typename... CONTEXT >
        RET emit ( ARGS... args, CONTEXT & ctx );
    };

    /**
     * Can be used to instantiate a safe bobject.
     *
     * @a createSafe is standalone, meaning it works without any declaration of
     * @a BOOST_SAFE_OBJECT for this bobject.
     *
     * Example:
     * @code
     * namespace unsafe
     * {
     *     class example : public BObject
     *     {
     *         B_OBJECT(example)
     *
     *     public:
     *         static example * create ()
     *         {
     *             // Creates a safe object of "type":
     *             // BOOST_SAFE_OBJECT(example)
     *             return bobj::createSafe ();
     *         }
     *     };
     * }
     * @endcode
     */
    template < typename... PARAMETERS >
    unspecified createSafe ( PARAMETERS... );
};

#endif // BOOST_OBJECT_DOXYGEN

#endif // BOOST_OBJECT_OBJECT_HPP

