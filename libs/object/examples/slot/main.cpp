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

    private:
        //[slot_slotimpl_test1
        void _slot_test1 ( int i )
        {
            std::cout << "_slot_test1(" << i << ")" << std::endl;
        }
        //]
        //[slot_slotimpl_test2
        void my_own_implementation_name ( int i )
        {
            std::cout << "my_own_implementation_name(" << i << ")" << std::endl;
        }
        //]
        //[slot_slotimpl_overload1
        void _slot_overload1 ( int ) {}
        void _slot_overload1 ( double ) {}
        //]


    public:
        //[slot_slot_test1
        auto_slot(test1)
        //]
        //[slot_slot_test2
        auto_slot(test2,my_own_implementation_name)
        //]
        //[slot_slot_overload1
        bobj::auto_slot_t<void(double)> overload1 boundto(_slot_overload1);
        //]
    };
}

BOOST_SAFE_OBJECT(myObject)

int main()
{
    BApplication app;
    myObject obj;

    //[slot_slot_test1test2_call
    //[slot_slot_test1_call
    obj.test1 ( 25 );
    //]
    //[slot_slot_test2_call
    obj.test2 ( 0 );
    //]
    //]

    obj.overload1 ( 0.1 );

    return app.exec ();
}
