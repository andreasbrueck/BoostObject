#define BOOST_TEST_MODULE generic
#include "tests.h"

#include <boost/object/application.hpp>
#include <boost/object/thread.hpp>
BOOST_OBJECT_INSTALL

#include <boost/object/detail/defs/thread.hpp>

////////////////////////////////////////////////////////////////////////////////

BOOST_OBJECT_TEST_CASE(quit_application)
{
    BApplication app;
    app.quit(123);
    BOOST_CHECK(app.exec()==123);
}

////////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_OBJECT_DISABLE_HIERARCHY

namespace unsafe
{
    class flag_destruct : public BObject
    {
        bool * m_b;

    public:
        flag_destruct ( bool & b ) : m_b ( & b ) {}
        ~ flag_destruct () { *m_b = !*m_b; }
    };
}

BOOST_SAFE_OBJECT(flag_destruct)

BOOST_OBJECT_TEST_CASE(default_parent_delete)
{
    bool destructed = false;

    {
        BApplication app;
        new flag_destruct ( destructed );
    }

    BOOST_CHECK(destructed==true);
}
#endif // #ifndef BOOST_OBJECT_DISABLE_HIERARCHY

////////////////////////////////////////////////////////////////////////////////

namespace unsafe
{
    class sender_emit : public BObject
    {
        B_OBJECT(sender_emit)

    protected:
        sender_emit ( BObject * parent = 0 )
            :
              BObject ( parent )
        {}

    private:
        void _slot_send_from_inside ()
        {
            send ();
        }

    public:
        BOOST_OBJECT_AUTO_SLOT(send_from_inside)
        BOOST_OBJECT_SIGNAL(send,void())
    };

    class sender_check : public BObject
    {
        B_OBJECT(sender_check)

    protected:
        explicit sender_check ( sender_emit * emitter, BObject * parent = 0 )
            :
              BObject ( parent ),
              emitter ( emitter ),
              test_count ( 0 )
        {
            emitter->send.connect ( auto_void );
            emitter->send.connect ( async_void );
            emitter->send.connect ( auto_nonvoid );
        }

    private:
        void _slot_auto_void (){ BOOST_CHECK(emitter==sender()); inc_count();}
        void _slot_async_void (){ BOOST_CHECK(emitter==sender()); inc_count();}
        bool _slot_auto_nonvoid (){ BOOST_CHECK(emitter==sender());
                                    inc_count();
                                    return true; }

        void inc_count ()
        {
            if ( ++ test_count == 3 )
            {
                bApp->quit ( 0 );
            }
        }

    private:
        BObject * emitter;

        int test_count;

    public:
        BOOST_OBJECT_AUTO_SLOT(auto_void)
        BOOST_OBJECT_ASYNC_SLOT(async_void)
        BOOST_OBJECT_AUTO_SLOT(auto_nonvoid)
    };
}

BOOST_SAFE_OBJECT(sender_check)
BOOST_SAFE_OBJECT(sender_emit)

BOOST_OBJECT_TEST_CASE(sender)
{
    BApplication    app;

    sender_emit     sender;
    sender_check    receiver ( & sender );

    sender.send_from_inside ();

    app.exec ();
}

BOOST_OBJECT_TEST_CASE(sender_threaded)
{
    BApplication    app;
    BThread         trd;

    sender_emit     sender ( & trd );
    sender_check    receiver ( & sender );

    sender.send_from_inside ();

    app.exec ();
}

////////////////////////////////////////////////////////////////////////////////

namespace unsafe
{
    class testThreadId : public BObject
    {
        B_OBJECT(testThreadId)

    protected:
        testThreadId ( bool quit = false, BObject * parent = 0 )
            :
              BObject ( parent ),
              m_quit ( quit )
        {}

        void _slot_test ()
        {
            m_id = BOOST_OBJECT_STL::this_thread::get_id ();

            if ( m_quit ) bApp->quit(0);
        }

    public:
        boost::object::detail::thread_t::id m_id;

    private:
        bool m_quit;

    public:
        BOOST_OBJECT_AUTO_SLOT(test)
    };
}

BOOST_SAFE_OBJECT(testThreadId)

BOOST_OBJECT_TEST_CASE(automatic_slot)
{
    {
        BApplication app;
        {
            BThread trd;
            {
                auto cur_id = BOOST_OBJECT_STL::this_thread::get_id ();

                testThreadId t1, t2 ( true, & trd );

                t1.test ();
                t2.test ();

                app.exec ();

                BOOST_CHECK(t1.m_id==cur_id);
                BOOST_CHECK(t2.m_id!=cur_id);
            }
            BOOST_CHECK_MESSAGE(1==1,"bla");
        }
        BOOST_CHECK_MESSAGE(1==1,"blu");
    }
    BOOST_CHECK_MESSAGE(1==1,"blo");
}

////////////////////////////////////////////////////////////////////////////////

namespace unsafe
{
    class fail_if_called_or_not : public BObject
    {
        B_OBJECT(fail_if_called_or_not)

    protected:
        explicit fail_if_called_or_not ( bool fail )
            :
              m_was_called ( false ),
              m_fail ( fail )
        {}

        ~ fail_if_called_or_not ()
        {
            if ( m_fail )
            {
                BOOST_CHECK(m_was_called==false);
            }
            else
            {
                BOOST_CHECK(m_was_called==true);
            }
        }

    private:
        void _slot_slot ()
        {
            m_was_called = true;
        }

    private:
        bool m_was_called;
        bool m_fail;

    public:
        BOOST_OBJECT_DIRECT_SLOT(slot)
    };

    class fail_if_called_or_not_emitter : public BObject
    {
        B_OBJECT(fail_if_called_or_not_emitter)

    public:
        BOOST_OBJECT_SIGNAL(sig,void())
    };
}

BOOST_SAFE_OBJECT(fail_if_called_or_not)
BOOST_SAFE_OBJECT(fail_if_called_or_not_emitter)

BOOST_OBJECT_TEST_CASE(signalContext_dontSendTo)
{
    BApplication app;

    fail_if_called_or_not           f1 ( false ),
                                    f2 ( false ),
                                    f3 ( true ),
                                    f4 ( true );
    fail_if_called_or_not_emitter   fe;

    fe.sig.connect ( f1.slot );
    fe.sig.connect ( f2.slot );
    fe.sig.connect ( f3.slot );

    fe.sig.emit ( boost::object::signalcontext::dontSendTo ( & f3 ) );

    fe.sig.connect ( f4.slot );

    fe.sig.emit ( boost::object::signalcontext::dontSendTo ( & f3 ),
                  boost::object::signalcontext::dontSendTo ( & f4 ) );

}
