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
    template < typename INPUT, typename OUTPUT >
    class myObject : public BObject
    {
        B_OBJECT(myObject<INPUT,OUTPUT>)

    private:
        void _slot_convert ( INPUT i )
        {
            converted ( ( OUTPUT ) i );
        }

    public:
        auto_slot_tmpl(convert)
        bsignal_tmpl(converted,void(OUTPUT))
    };
}

template < typename INPUT, typename OUTPUT >
BOOST_SAFE_OBJECT(myObject,INPUT,OUTPUT)

int main ()
{
    BApplication app;
    myObject < double, int > obj;

    obj.converted.connect ( app.quit );
    obj.convert ( 1.2 );

    return app.exec ();
}
