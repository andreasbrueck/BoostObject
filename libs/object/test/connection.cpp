#define BOOST_TEST_MODULE connection
#include "tests.h"

#include <boost/object/application.hpp>
BOOST_OBJECT_INSTALL

////////////////////////////////////////////////////////////////////////////////

int connectionTestCount = 0;

namespace unsafe
{
    class connectionTest : public BObject
    {
        B_OBJECT(connectionTest)

    private:
        void _slot_slot1 ( const std::string & str )
        {
            BOOST_CHECK(str==__DATE__);
            BOOST_CHECK(sender()==this);

            ++ connectionTestCount;
        }

        void _slot_slot2 ( const std::string & str, const std::string & str2 )
        {
            BOOST_CHECK(str==__DATE__);
            BOOST_CHECK(str2==__TIME__);
            BOOST_CHECK(sender()==this);

            ++ connectionTestCount;
        }

    public:
        auto_slot(slot1)
        auto_slot(slot2)
        bsignal(sig1,void(const std::string &))
        bsignal(sig2,void(const std::string &))
    };
}

BOOST_SAFE_OBJECT(connectionTest)

struct connectionFunctions
{
    void operator () ( const std::string & str )
    {
        BOOST_CHECK(str==__DATE__);
        ++ connectionTestCount;
    }

    static void staticFunc ( const std::string & str )
    {
        BOOST_CHECK(str==__DATE__);
        ++ connectionTestCount;
    }
};

struct connectionFunctions2
{
    void operator () ( const std::string & str, const std::string & str2 )
    {
        BOOST_CHECK(str==__DATE__);
        BOOST_CHECK(str2==__TIME__);
        ++ connectionTestCount;
    }

    static void staticFunc ( const std::string & str, const std::string & str2 )
    {
        BOOST_CHECK(str==__DATE__);
        BOOST_CHECK(str2==__TIME__);
        ++ connectionTestCount;
    }
};

BOOST_OBJECT_TEST_CASE(connections)
{
    BApplication app;
    connectionTest test;
    std::string timestr ( __TIME__ );

    BConnection conn;

    connectionTestCount = 0;

    {
        auto lambda = [=] ( const std::string & str, const std::string & str2 )
        {
            BOOST_CHECK(str==__DATE__);
            BOOST_CHECK(str2==__TIME__);
            ++ connectionTestCount;
        };

        isConnected & test.sig1.connect ( test.slot1 );
        isConnected & test.sig1.connect ( test.slot2, timestr );

        /////

        isConnected & test.sig1.connect ( [&] ( const std::string & str )
            {
                BOOST_CHECK(str==__DATE__);
                ++ connectionTestCount;
            }
        );
        isConnected & test.sig1.connect ( lambda, timestr );
        isConnected & test.sig1.connect ( connectionFunctions () );
        isConnected & test.sig1.connect ( & connectionFunctions::staticFunc );
        isConnected & test.sig1.connect ( connectionFunctions2 (), timestr );
        isConnected & test.sig1.connect ( & connectionFunctions2::staticFunc,
                                          timestr );

        /////

        isConnected & test.sig1.connect ( &app, [&] ( const std::string & str )
            {
                BOOST_CHECK(str==__DATE__);
                ++ connectionTestCount;
            }
        );
        isConnected & test.sig1.connect ( &app, lambda, timestr );
        isConnected & test.sig1.connect ( &app, connectionFunctions () );
        isConnected & test.sig1.connect ( &app, & connectionFunctions::staticFunc );
        isConnected & test.sig1.connect ( &app, connectionFunctions2 (), timestr );
        conn = test.sig1.connect ( &app, & connectionFunctions2::staticFunc, timestr );
        isConnected & conn;
    }

    isConnected & test.sig2.connect(test.sig1);
    isNotConnected & test.sig2.connect(test.sig2);

    test.sig1.emit ( __DATE__ );
    test.sig2.emit ( __DATE__ );

    BOOST_REQUIRE(connectionTestCount==28);
    connectionTestCount = 0;


    bool disconnected = test.sig1.disconnect(test.slot1);
    BOOST_CHECK(disconnected);

    test.sig1.emit ( __DATE__ );
    BOOST_REQUIRE(connectionTestCount==13);
    connectionTestCount = 0;


    disconnected = test.sig2.disconnect(test.sig1);
    BOOST_CHECK(disconnected);

    test.sig2.emit ( __DATE__ );
    BOOST_REQUIRE(connectionTestCount==0);
    connectionTestCount = 0;


    disconnected = conn.disconnect ();
    BOOST_CHECK(disconnected);

    test.sig1.emit ( __DATE__ );
    BOOST_REQUIRE(connectionTestCount==12);
    connectionTestCount = 0;


    test.sig1.emit ( __DATE__, BSignalContext::dontSendTo ( & app ) );
    BOOST_REQUIRE(connectionTestCount==7);
    connectionTestCount = 0;

    test.sig1.emit ( __DATE__, BSignalContext::dontSendTo ( & app ),
                               BSignalContext::dontSendTo ( & test ) );
    BOOST_REQUIRE(connectionTestCount==6);
    connectionTestCount = 0;
}
