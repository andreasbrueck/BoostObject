////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/object/application.hpp>

BOOST_OBJECT_INSTALL

namespace unsafe
{
    class myObject : public BObject
    {
        B_OBJECT(myObject)

    protected:
        ~ myObject ()
        {
            //[signal_emission
            allOperationsDone ( 234 );
            //]
        }

    public:
        //[signal_macro_declare
        bsignal(allOperationsDone,void(int))
        //]
#       if 0
        //[signal_cpp_declare
        bobj::signal_t<void(int)> allOperationsDone = this;
        //]
#       endif
    };
}

BOOST_SAFE_OBJECT(myObject)

int main()
{
    BApplication app;

    myObject * obj = new myObject;

    //[signal_connect
    obj->allOperationsDone.connect ( bApp->quit );
    //]
    obj->deleteLater ();

    return app.exec ();
}

