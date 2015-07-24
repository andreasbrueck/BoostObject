#define BOOST_TEST_MODULE bbytearray
#include "tests.h"

#include <boost/object/utils/bytearray.hpp>

BOOST_OBJECT_TEST_CASE(copy)
{
    BByteArray a ( __DATE__ );
    BByteArray b ( a );

    BOOST_CHECK(a==b);
    BOOST_CHECK(a==__DATE__);
    BOOST_CHECK(a.constData()==b.constData());
}

BOOST_OBJECT_TEST_CASE(copy_assign)
{
    BByteArray a ( __DATE__ );
    BByteArray b = a;

    BOOST_CHECK(a==b);
    BOOST_CHECK(a==__DATE__);
    BOOST_CHECK(a.constData()==b.constData());
}

BOOST_OBJECT_TEST_CASE(detach)
{
    BByteArray a ( __DATE__ );
    BByteArray b ( a );

    a += __TIME__;

    BOOST_CHECK(a!=b);
    BOOST_CHECK(a==__DATE__ __TIME__);
    BOOST_CHECK(b==__DATE__);
    BOOST_CHECK(a.constData()!=b.constData());
}

BOOST_OBJECT_TEST_CASE(operators)
{
    BByteArray a ( __DATE__ );
    BByteArray b ( __DATE__ );
    BByteArray c ( __TIME__ );
    BByteArray d = b + c;

    BOOST_CHECK(a==__DATE__);
    BOOST_CHECK(a==b);

    BOOST_CHECK(a+b==__DATE__ __DATE__);
    BOOST_CHECK(a+c==__DATE__ __TIME__);

    BOOST_CHECK(d==__DATE__ __TIME__);

    a += c;

    BOOST_CHECK(a==d);
    BOOST_CHECK(a==__DATE__ __TIME__);

    BOOST_CHECK(a!=b);
}

BOOST_OBJECT_TEST_CASE(stdstring)
{
    BByteArray a ( std::string ( __DATE__ ) );

    BOOST_CHECK(a==__DATE__);
    BOOST_CHECK(a==std::string(__DATE__));
    BOOST_CHECK(a.toStdString()==__DATE__);

    BByteArray b = a + std::string ( __TIME__ );

    BOOST_CHECK(b==__DATE__ __TIME__);
    BOOST_CHECK(b==std::string(__DATE__ __TIME__));
    BOOST_CHECK(b.toStdString()==__DATE__ __TIME__);

    BByteArray c = a + std::string ();

    BOOST_CHECK(c==__DATE__);
    BOOST_CHECK(c==std::string(__DATE__));
    BOOST_CHECK(c.toStdString()==__DATE__);

    a += std::string ( __TIME__ );

    BOOST_CHECK(a==__DATE__ __TIME__);
    BOOST_CHECK(a==std::string(__DATE__ __TIME__));
    BOOST_CHECK(a.toStdString()==__DATE__ __TIME__);
}

void range_based_for_impl ( BByteArray&& b )
{
    size_t i = 0;

    for ( const unsigned char c : b )
    {
        BOOST_CHECK(b[i]==c);
        ++ i;
    }

    BOOST_CHECK(i==b.size());
}

BOOST_OBJECT_TEST_CASE(range_based_for)
{
    range_based_for_impl ( BByteArray ( __DATE__ ) );
    range_based_for_impl ( BByteArray () );
}

BOOST_OBJECT_TEST_CASE(clear)
{
    BByteArray a ( __DATE__ );
    BByteArray b ( a );

    BOOST_CHECK(a==b);
    b.clear();
    BOOST_CHECK(a==__DATE__);
    BOOST_CHECK(b.isEmpty());
}
