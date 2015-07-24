////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/object/application.hpp>
#include <boost/object/network/tcpclient.hpp>

BOOST_OBJECT_INSTALL

namespace unsafe
{
    class DaytimeClientApplication : public BApplication
    {
        B_OBJECT(DaytimeClientApplication)

    protected:
        DaytimeClientApplication ()
        {
            BTcpClient * c = new BTcpClient(this);

            c->connected.connect(c->start);
            c->disconnected.connect(exit);
            c->received.connect(newData);
            c->received.connect(c->disconnect);

            c->connect ( "time.nist.gov", 13 );
        }

    private:
        void _slot_newData ( const BByteArray & data )
        {
            std::cout << data.toStdString() << std::endl;
        }

    private:
        auto_slot(newData)
    };
}

BOOST_SAFE_OBJECT(DaytimeClientApplication)

int main ()
{
    return DaytimeClientApplication ().exec ();
}
