////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/object/application.hpp>
#include <boost/object/thread.hpp>
#include <iostream>

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
              m_objectName ( objectName )
        {
            std::cout << m_objectName << " constructed" << std::endl;
        }

        ~ myObject ()
        {
            std::cout << m_objectName << " destructed" << std::endl;
        }

    private:
        const char * m_objectName;
    };
}

BOOST_SAFE_OBJECT(myObject)

int main()
{
    {
        BApplication app;

        myObject obj1 ( "obj1" );

        BThread * trd = new BThread;

        myObject obj2 ( "obj2", trd );
        myObject *obj3 = new myObject ( "obj3", trd ),
                 *obj4 = new myObject ( "obj4", & obj2 );

        BOOST_OBJECT_UNUSED(obj4);
        BOOST_OBJECT_UNUSED(obj3);
        BOOST_OBJECT_UNUSED(obj2);
        BOOST_OBJECT_UNUSED(obj1);
        BOOST_OBJECT_UNUSED(app);
    }

    std::cout << "Program about to exit..." << std::endl;

    return EXIT_SUCCESS;
}

