////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_UTILS_IMPL_BYTEARRAY_IPP
#define BOOST_OBJECT_UTILS_IMPL_BYTEARRAY_IPP

#include "../bytearray.hpp"

#include "../../detail/defs/mutex.hpp"

#include <atomic>
#include <vector>

namespace boost {
namespace object {
namespace utils {

    namespace detail
    {
        class bytearray_p
        {
            friend class utils::bytearray;

            bytearray_p ()
                :
                  ref ( 1 )
            {}

            bytearray_p ( const unsigned char * data_, size_t size )
                :
                  ref ( 1 ),
                  data ( data_, data_ + size )
            {}

            bytearray_p ( bytearray_p * p )
                :
                  ref ( 1 ),
                  data ( p->data )
            {}

            bytearray_p ( size_t size, unsigned char def )
                :
                  ref ( 1 ),
                  data ( size, def )
            {}

            bytearray_p ( const unsigned char * d1, size_t s1,
                          const unsigned char * d2, size_t s2 )
                :
                  ref ( 1 ),
                  data ( s1 + s2 )
            {
                memcpy ( & data[0], d1, s1 );
                memcpy ( & data[0] + s1, d2, s2 );
            }

            std::atomic < int > ref;
            std::vector < unsigned char > data;
        };
    }

    bytearray::bytearray ()
        :
          p ( new detail::bytearray_p )
    {}

    bytearray::bytearray ( const std::string & str )
        :
          p ( new detail::bytearray_p ( ( const unsigned char * ) str.c_str (),
                                        str.length () ) )
    {}

    bytearray::bytearray ( const char * str, size_t )
        :
          p ( new detail::bytearray_p ( ( const unsigned char * ) str,
                                        strlen ( str ) ) )
    {}

    bytearray::bytearray ( const unsigned char * data, size_t size )
        :
          p ( new detail::bytearray_p ( data, size ) )
    {}

    bytearray::bytearray ( size_t size, unsigned char def )
        :
          p ( new detail::bytearray_p ( size, def ) )
    {}

    bytearray::~bytearray ()
    {
        if ( p != nullptr )
        {
            if ( 0 == -- p->ref )
            {
                delete p;
            }
        }
    }

    bytearray::bytearray ( const bytearray & other ) BOOST_OBJECT_NOEXCEPT
        :
          p ( other.p )
    {
        ++ p->ref;
    }

    bytearray & bytearray::operator= ( const bytearray & other )
    {
        ++ other.p->ref;

        if ( 0 == -- p->ref )
        {
            delete p;
        }

        p = other.p;

        return *this;
    }

    bytearray::bytearray ( bytearray && other ) BOOST_OBJECT_NOEXCEPT
        :
          p ( other.p )
    {
        other.p = nullptr;
    }

    bytearray & bytearray::operator= ( bytearray && other )
    {
        std::swap ( p, other.p );
        return *this;
    }

    bool bytearray::isShared () const
    {
        return (p->ref > 1);
    }

    bool bytearray::isEmpty () const
    {
        return p->data.empty ();
    }

    void bytearray::detach ( bool copyData )
    {
        detail::bytearray_p * new_p = copyData
                ? new detail::bytearray_p ( p )
                : new detail::bytearray_p;

        if ( 0 == -- p->ref )
        {
            delete p;
        }

        p = new_p;
    }

    size_t bytearray::size () const
    {
        return p->data.size ();
    }

    void bytearray::clear ()
    {
        if ( isShared () )  detach ( false );
        else                p->data.clear ();
    }

    unsigned char * bytearray::data ()
    {
        if ( isShared () ) detach ();

        if ( size () ) return &p->data[0];
        else return nullptr;
    }

    const unsigned char * bytearray::data () const
    {
        if ( size () ) return &p->data[0];
        else return nullptr;
    }

    const unsigned char * bytearray::constData () const
    {
        return data ();
    }

    bytearray::iterator bytearray::begin ()
    {
        if ( isShared () ) detach ();
        return p->data.begin ();
    }

    bytearray::const_iterator bytearray::begin () const
    {
        return p->data.cbegin ();
    }

    bytearray::const_iterator bytearray::cbegin () const
    {
        return p->data.cbegin ();
    }

    bytearray::const_iterator bytearray::constBegin () const
    {
        return p->data.cbegin ();
    }

    bytearray::iterator bytearray::end ()
    {
        if ( isShared () ) detach ();
        return p->data.end ();
    }

    bytearray::const_iterator bytearray::end () const
    {
        return p->data.cend ();
    }

    bytearray::const_iterator bytearray::cend () const
    {
        return p->data.cend ();
    }

    bytearray::const_iterator bytearray::constEnd () const
    {
        return p->data.cend ();
    }

    std::string bytearray::toStdString () const
    {
        return std::string ( ( const char * ) constData (), size () );
    }

    unsigned char bytearray::operator[] ( size_t pos ) const
    {
        return p->data[pos];
    }

    bytearray & bytearray::operator += ( const bytearray & l )
    {
        if ( ! l.isEmpty () )
        {
            if ( isEmpty () )
            {
                *this = l;
            }
            else
            {
                if ( isShared () )
                {
                    detach ();
                }

                std::vector < unsigned char > & dst = p->data;
                const std::vector < unsigned char > & src = l.p->data;

                dst.insert ( dst.end (), src.begin (), src.end () );
            }
        }

        return *this;
    }

    bytearray & bytearray::operator += ( const char * l )
    {
        if ( ! l ) return *this;

        size_t len = strlen ( l );

        if ( len )
        {
            if ( isEmpty () )
            {
                *this = bytearray ( l );
            }
            else
            {
                if ( isShared () )
                {
                    detach ();
                }

                size_t oldsize = p->data.size ();

                p->data.resize ( oldsize + len );
                memcpy ( & p->data[0] + oldsize, l, len );
            }
        }

        return *this;
    }

    bytearray & bytearray::operator += ( const std::string & l )
    {
        size_t len = l.size ();

        if ( len )
        {
            if ( isEmpty () )
            {
                *this = bytearray ( l );
            }
            else
            {
                if ( isShared () )
                {
                    detach ();
                }

                size_t oldsize = p->data.size ();

                p->data.resize ( oldsize + len );
                memcpy ( & p->data[0] + oldsize, l.data (), len );
            }
        }

        return *this;
    }

    bool bytearray::operator== ( const bytearray & l ) const
    {
        if ( size () == l.size () )
        {
            if ( memcmp ( constData (), l.constData(), size () ) == 0 )
            {
                return true;
            }
        }

        return false;
    }

    bool bytearray::operator== ( const char * l ) const
    {
        if ( l )
        {
            if ( size () == strlen ( l ) )
            {
                if ( memcmp ( constData (), l, size () ) == 0 )
                {
                    return true;
                }
            }

            return false;
        }

        return isEmpty ();
    }

    bool bytearray::operator== ( const std::string & l ) const
    {
        if ( size () == l.size () )
        {
            if ( memcmp ( constData (), l.data (), size () ) == 0 )
            {
                return true;
            }
        }

        return false;
    }

    bool bytearray::operator!= ( const bytearray & l ) const
    {
        return !(*this==l);
    }

    bool bytearray::operator!= ( const char * l ) const
    {
        return !(*this==l);
    }

    bool bytearray::operator!= ( const std::string & l ) const
    {
        return !(*this==l);
    }

    bytearray::bytearray ( const unsigned char * d1, size_t s1,
                           const unsigned char * d2, size_t s2 )
        :
          p ( new detail::bytearray_p ( d1, s1, d2, s2 ) )
    {}

    bytearray bytearray::operator+ ( const bytearray & l )
    {
        return bytearray ( constData (), size (), l.constData (), l.size () );
    }

    bytearray bytearray::operator+ ( const char * l )
    {
        size_t len = 0;

        if ( ! l || ! ( len = strlen ( l ) ) )
        {
            return *this;
        }
        else
        {
            return bytearray ( constData (), size (),
                               ( const unsigned char * ) l, len );
        }
    }

    bytearray bytearray::operator+ ( const std::string & l )
    {
        size_t len = l.size ();

        if ( ! len )
        {
            return *this;
        }
        else
        {
            return bytearray ( constData (), size (),
                               ( const unsigned char * ) l.data (), len );
        }
    }

} // namespace utils
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_UTILS_IMPL_BYTEARRAY_IPP

