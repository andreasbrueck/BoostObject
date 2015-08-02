#define BOOST_TEST_MODULE network
#include "../tests.h"

#ifndef BOOST_OBJECT_DISABLE_HIERARCHY

#include <boost/object/application.hpp>
#include <boost/object/network/tcpserver.hpp>
#include <boost/object/network/tcpclient.hpp>
#include <boost/object/utils/timer.hpp>

BOOST_OBJECT_INSTALL

////////////////////////////////////////////////////////////////////////////////

namespace unsafe
{
    class tcp_generic_checks : public BObject
    {
        B_OBJECT(tcp_generic_checks)

    public:
        ~tcp_generic_checks ()
        {
            BOOST_CHECK(m_client_count==0);
            BOOST_CHECK(m_connection_count==0);
            BOOST_CHECK(m_max_server_connections==m_max_client_count);
            BOOST_CHECK(m_server_received_count==m_max_server_connections);
            BOOST_CHECK(m_client_received_count==m_max_client_count);
        }

    protected:
        tcp_generic_checks ()
            :
              m_client_count ( 0 ),
              m_connection_count ( 0 ),
              m_max_client_count ( 0 ),
              m_max_server_connections ( 0 ),
              m_server_received_count ( 0 ),
              m_client_received_count ( 0 )
        {
            BTimer::singleShot ( 5000, bApp->exit );
        }

    private:
        void _slot_server_newConnection ( BTcpSocket * s )
        {
            m_connection_count++;
            m_max_server_connections++;

            isConnected & s->disconnected.connect ( decConnectionCount );
            isConnected & s->disconnected.connect ( cleanDisconnectCheck );
            isConnected & s->received.connect ( server_received );

            s->start ();
        }

        void _slot_server_received ( const BByteArray & data )
        {
            m_server_received_count++;

            BOOST_OBJECT_TEST_DYNAMIC_CAST(sock,BTcpSocket,sender())

            sock->setNoDelay ( false );
            BOOST_CHECK(sock->noDelay()==false);
            sock->setNoDelay ( true );
            BOOST_CHECK(sock->noDelay()==true);

            BOOST_CHECK(data==__DATE__ __TIME__);
            sock->write ( BByteArray ( __TIME__ __DATE__ ) );
        }

        void _slot_client_received ( const BByteArray & data )
        {
            m_client_received_count++;

            BOOST_CHECK(data==__TIME__ __DATE__);
            sender ()->deleteLater ();
        }

        void _slot_client_connected ( BTcpClient * c )
        {
            c->setNoDelay ( false );
            BOOST_CHECK(c->noDelay()==false);
            c->setNoDelay ( true );
            BOOST_CHECK(c->noDelay()==true);
        }

        void _slot_addClientCount ()
        {
            m_max_client_count++;
            m_client_count++;
        }

        void _slot_decClientCount ()
        {
            m_client_count--;

            if ( m_client_count == 0 && m_connection_count == 0 )
            {
                bApp->exit ();
            }
        }

        void _slot_decConnectionCount ()
        {
            m_connection_count--;

            if ( m_client_count == 0 && m_connection_count == 0 )
            {
                bApp->exit ();
            }
        }

        void _slot_cleanDisconnectCheck ( const boost::system::error_code & ec )
        {
            BOOST_CHECK_MESSAGE(!ec,ec.message()+"|"+std::to_string(ec.value())+"|"+typeid(*sender()).name());
        }

    private:
        int m_client_count;
        int m_connection_count;
        int m_max_client_count;
        int m_max_server_connections;
        int m_server_received_count;
        int m_client_received_count;

    private:
        BOOST_OBJECT_AUTO_SLOT(decConnectionCount)

    public:
        BOOST_OBJECT_AUTO_SLOT(addClientCount)
        BOOST_OBJECT_AUTO_SLOT(decClientCount)

        BOOST_OBJECT_AUTO_SLOT(server_newConnection)
        BOOST_OBJECT_AUTO_SLOT(server_received)
        BOOST_OBJECT_AUTO_SLOT(client_received)
        BOOST_OBJECT_AUTO_SLOT(client_connected)

        BOOST_OBJECT_AUTO_SLOT(cleanDisconnectCheck)
    };
}

BOOST_SAFE_OBJECT(tcp_generic_checks)

void start_client ( const std::string & addr, uint16_t port,
                    tcp_generic_checks & checks )
{
    BTcpClient * c = new BTcpClient;

    checks.addClientCount ();

    isConnected & c->disconnected.connect ( checks.cleanDisconnectCheck );
    isConnected & c->disconnected.connect ( c->deleteLater );
    isConnected & c->connected.connect ( checks.client_connected, c );
    isConnected & c->connected.connect ( c->write, BByteArray ( __DATE__ __TIME__ ) );
    isConnected & c->received.connect ( checks.client_received );
    isConnected & c->destroyed.connect ( checks.decClientCount );

    c->connect ( addr, port );
}

BOOST_OBJECT_TEST_CASE(tcp_generic)
{
    BApplication app;
    BTcpServer server;
    tcp_generic_checks checks;

    server.setReuseAddress ( true );
    BOOST_CHECK(server.reuseAddress()==true);
    server.setReuseAddress ( false );
    BOOST_CHECK(server.reuseAddress()==false);

    isConnected & server.newConnection.connect ( checks.server_newConnection );
    server.start ();

    BOOST_CHECK(server.port()!=0);

    start_client ( "localhost", server.port (), checks );
    start_client ( "127.0.0.1", server.port (), checks );

    app.exec ();
}

#else // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY
BOOST_OBJECT_NULL_TEST
#endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY
