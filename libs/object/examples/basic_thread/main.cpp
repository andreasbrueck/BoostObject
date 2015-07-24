////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/object/application.hpp>
#include <boost/object/thread.hpp>
#include <iostream>
#include <thread>

BOOST_OBJECT_INSTALL

namespace unsafe
{
    class myObject : public BObject
    {
        B_OBJECT(myObject)

    protected:
        myObject ( const char * objectName, BObject * parent = nullptr )
            :
              BObject ( parent ),
              m_myObjectName ( objectName )
        {}

    private:
        void _slot_outputThreadId ( bool terminate ) const
        {
            std::cout << m_myObjectName << "'s slot got executed in thread: "
                      << std::this_thread::get_id () << std::endl;

            if ( terminate )
            {
                bApp->quit ( EXIT_SUCCESS );
            }
        }

    private:
        const char * m_myObjectName;

    public:
        auto_slot(outputThreadId)
    };
}

BOOST_SAFE_OBJECT(myObject)

int main ()
{
    BApplication    app;
    BThread         trd;

    std::cout << "main()'s thread: " << std::this_thread::get_id ()
              << std::endl;

    myObject obj1 ( "obj1" ),
             obj2 ( "obj2", & trd );

    obj1.outputThreadId ( false );
    obj2.outputThreadId ( true );

    return app.exec ();
}

