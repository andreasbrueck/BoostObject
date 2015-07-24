////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_UTILS_BYTEARRAY_HPP
#define BOOST_OBJECT_UTILS_BYTEARRAY_HPP

#include "../detail/config.hpp"

#include <memory>
#include <string>
#include <vector>

namespace boost {
namespace object {
namespace utils {

BOOST_OBJECT_FORWARD_PRIVATE(bytearray)

/**
 * The bytearray class is the default data container for Boost.Object.
 *
 * Its internal data is not copied when the bytearray class is copied. Instead,
 * it is only copied whenever there are 2 or more copies of a bytearray
 * instance, and one of them intends to alter the data. All member functions
 * that copy the internal data will say if and under which circumstances
 * they do so.
 *
 */
class bytearray
{
public:
    /**
     * Constructs an empty bytearray
     */
    BOOST_OBJECT_DECL explicit bytearray ();

    /**
     * @param size If -1, uses strlen() on @p str, if bigger than 0, @p size
     *             bytes of @p str are copied.
     * @param str A null terminated string to copy data from. Must point to a
     *            valid null terminated string.
     *
     * Constructs a bytearray from a null terminated string, not including the
     * null termination character.
     */
    BOOST_OBJECT_DECL explicit bytearray ( const char * str, size_t size = -1 );

    /**
     * Constructs a bytearray with the first @p size bytes pointed to by
     * @p data.
     */
    BOOST_OBJECT_DECL explicit bytearray ( const unsigned char * data,
                                           size_t size );

    /**
     * Construct a bytearray with the size @p size initialized to @p def
     */
    BOOST_OBJECT_DECL explicit bytearray ( size_t size, unsigned char def = 0 );

    /**
     * Constructs a bytearray from std::string, not including null terminate
     */
    BOOST_OBJECT_DECL explicit bytearray ( const std::string & str );

    BOOST_OBJECT_DECL ~bytearray ();

    /**
     * @return The size of this bytearray
     *
     * Returns the size of this bytearray.
     */
    BOOST_OBJECT_DECL size_t size () const;

    /**
     * @return The data of this bytearray
     *
     * If more than 1 copy of this bytearray exists ( isShared() == true ),
     * this instance will copy all of its internal data before this function
     * returns. After that, isShared() will return false.
     */
    BOOST_OBJECT_DECL unsigned char * data ();

    /**
     * @return The data of this bytearray
     *
     * This function is the same as the non-const overload, except that it
     * will not copy its internal data if more than 1 copy of this bytearray
     * exists.
     */
    BOOST_OBJECT_DECL const unsigned char * data () const;

    /**
     * @return The data of this bytearray
     *
     * This function is the same as @c bytearray::data()const.
     */
    BOOST_OBJECT_DECL const unsigned char * constData () const;

    typedef std::vector<unsigned char>::iterator iterator;
    typedef std::vector<unsigned char>::const_iterator const_iterator;

    BOOST_OBJECT_DECL iterator begin ();
    BOOST_OBJECT_DECL const_iterator begin () const;
    BOOST_OBJECT_DECL const_iterator cbegin () const;
    BOOST_OBJECT_DECL const_iterator constBegin () const;

    BOOST_OBJECT_DECL iterator end ();
    BOOST_OBJECT_DECL const_iterator end () const;
    BOOST_OBJECT_DECL const_iterator cend () const;
    BOOST_OBJECT_DECL const_iterator constEnd () const;

    /**
     * Converts the internal data to an @c std::string.
     */
    BOOST_OBJECT_DECL std::string toStdString () const;

    /**
     * Access specified element. The behaviour is undefined if @p pos is out of
     * range.
     */
    BOOST_OBJECT_DECL unsigned char operator[] ( size_t pos ) const;

    /**
     * @return true if more than 1 copy of this bytearray exists, otherwise
     *         false.
     */
    BOOST_OBJECT_DECL bool isShared () const;

    /**
     * @return True if this bytearray does not contain any data ( @c size()==0 )
     */
    BOOST_OBJECT_DECL bool isEmpty () const;

    /**
     * Clears the internal buffer. After this call, @c size() is 0 and
     * @c isEmpty() is true.
     *
     * If the data is shared ( @c isShared()==true ), this function will not
     * affect any other copy of this bytearray instance.
     */
    BOOST_OBJECT_DECL void clear ();

    /**
     * ...
     */
    BOOST_OBJECT_DECL bytearray & operator+= ( const bytearray & l );
    BOOST_OBJECT_DECL bytearray & operator+= ( const char * l );
    BOOST_OBJECT_DECL bytearray & operator+= ( const std::string & l );
    BOOST_OBJECT_DECL bool operator== ( const bytearray & l ) const;
    BOOST_OBJECT_DECL bool operator== ( const char * l ) const;
    BOOST_OBJECT_DECL bool operator== ( const std::string & l ) const;
    BOOST_OBJECT_DECL bool operator!= ( const bytearray & l ) const;
    BOOST_OBJECT_DECL bool operator!= ( const char * l ) const;
    BOOST_OBJECT_DECL bool operator!= ( const std::string & l ) const;
    BOOST_OBJECT_DECL bytearray operator+ ( const bytearray & l );
    BOOST_OBJECT_DECL bytearray operator+ ( const char * l );
    BOOST_OBJECT_DECL bytearray operator+ ( const std::string & l );

    BOOST_OBJECT_DECL
    bytearray ( const bytearray & other ) BOOST_OBJECT_NOEXCEPT;
    BOOST_OBJECT_DECL
    bytearray ( bytearray && other ) BOOST_OBJECT_NOEXCEPT;

    BOOST_OBJECT_DECL bytearray & operator= ( const bytearray & other );
    BOOST_OBJECT_DECL bytearray & operator= ( bytearray && other );

#ifndef BOOST_OBJECT_DOXYGEN
private:
    BOOST_OBJECT_DECL bytearray ( const unsigned char * d1, size_t s1,
                                  const unsigned char * d2, size_t s2 );
    BOOST_OBJECT_DECL void detach ( bool copyData = true );

private:
    detail::bytearray_p * p;
#endif // #ifndef BOOST_OBJECT_DOXYGEN
};

} // namespace utils
} // namespace object
} // namespace boost

#if defined(BOOST_OBJECT_HEADER_ONLY) \
    || defined(BOOST_OBJECT__IN_UTILS_BYTEARRAY_CPP)
#include "impl/bytearray.ipp"
#endif

#ifndef BOOST_OBJECT_NO_GLOBAL_TYPEDEFS
/**
 * @brief Global name for boost::object::utils::bytearray
 */
typedef boost::object::utils::bytearray BByteArray;
#endif // BOOST_OBJECT_NO_GLOBAL_TYPEDEFS

#endif // BOOST_OBJECT_UTILS_BYTEARRAY_HPP

