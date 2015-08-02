#ifndef BOOST_OBJECT_TESTS_H
#define BOOST_OBJECT_TESTS_H

#include <boost/test/unit_test.hpp>

#define BOOST_OBJECT_TEST_DYNAMIC_CAST(DYNCAST_RESULT,TO,FROM)      \
    TO * DYNCAST_RESULT = dynamic_cast < TO * > ( FROM );           \
    BOOST_REQUIRE(DYNCAST_RESULT!=0);

#define BOOST_OBJECT_TEST_CASE(name)                    \
    void boost_object_test_case__##name();              \
    BOOST_AUTO_TEST_CASE(name)                          \
    {                                                   \
        boost::object::test::heap heap_test;            \
        boost_object_test_case__##name ();              \
        (void)heap_test;                                \
    }                                                   \
    void boost_object_test_case__##name ()

#define BOOST_OBJECT_NULL_TEST                          \
    BOOST_OBJECT_TEST_CASE(null_test)                   \
    { BOOST_CHECK(0==0); }

namespace boost {
namespace object {
namespace test {

    class heap
    {
#   if !defined(NDEBUG) && defined(_MSC_VER)
    public:
        heap ()
        {
            _CrtMemCheckpoint ( & oldState );
        }

        ~heap ()
        {
            _CrtMemState curState, diffState;

            _CrtMemCheckpoint ( & curState );

            // variable name choosen for obvious error message
            int memory_leaks = _CrtMemDifference ( & diffState, & oldState,
                                                   & curState );

            BOOST_REQUIRE(memory_leaks==0);
        }

    private:
        _CrtMemState oldState;
#   endif
    };

} // namespace test
} // namespace object
} // namespace boost

////////////////////////////////////////////////////////////////////////////////
/// Gadgets used in tests

struct checkConnected
{
    void operator & ( bool isConnected )
    {
        BOOST_REQUIRE(isConnected);
    }
};

struct checkNotConnected
{
    void operator & ( bool isConnected )
    {
        BOOST_REQUIRE(!isConnected);
    }
};

// all tests only have one TU
static checkConnected isConnected;
static checkNotConnected isNotConnected;

#endif
