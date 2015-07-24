////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/object/application.hpp>
#include <boost/object/network/tcpserver.hpp>

BOOST_OBJECT_INSTALL

namespace unsafe
{
    class DaytimeServerApplication : public BApplication
    {
        B_OBJECT ( DaytimeServerApplication )

    protected:
        DaytimeServerApplication ()
            :
              server ( 13 )
        {
            server.newConnection.connect ( newConnection );
            server.start ();
        }

    private:
        void _slot_newConnection ( BTcpSocket * sock )
        {
            sock->disconnected.connect ( sock->deleteLater );

            // Just write the data, and trust the client to close
            // the connection after receiving. It's just an example program.
            time_t now = std::time ( 0 );
            sock->write ( BByteArray ( std::ctime ( & now ) ) );
        }

    private:
        auto_slot ( newConnection )

    private:
        BTcpServer server;
    };
}

BOOST_SAFE_OBJECT ( DaytimeServerApplication )

int main ()
{
    return DaytimeServerApplication ().exec ();
}
