////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ basic_use_cpp_file
//[ basic_use_cpp_includes
#include <boost/object/object.hpp>
#include <boost/object/application.hpp>
#include <iostream>
//]

//[ basic_use_cpp_install
BOOST_OBJECT_INSTALL
//]

namespace unsafe
{
    //[ basic_use_cpp_class_foo
    //[ basic_use_cpp_class_foo_open
    class foo : public BObject
    //]
    {
        //[ basic_use_cpp_class_foo_b_object
        B_OBJECT(foo)
        //]

        //[ basic_use_cpp_class_foo_slot_bar_implementation
    private:
        //[ basic_use_cpp_class_foo_slot_bar_implementation_term
        void _slot_bar ( int exitCode )
        //]
        {
            std::cout << "About to tell the application to quit ..."
                      << std::endl;

            //[ basic_use_cpp_class_foo_slot_bar_implementation_body
            bApp->quit ( exitCode );
            //]

            std::cout << "Told the application to quit ..." << std::endl;
        }
        //]

        //[ basic_use_cpp_class_foo_slot_bar
    public:
        //[ basic_use_cpp_class_foo_slot_bar_term
        async_slot(bar)
        //]
        //]
    };
    //]
}

//[ basic_use_cpp_class_foo_safe
BOOST_SAFE_OBJECT(foo)
//]

//[ basic_use_cpp_main
int main()
{
    BApplication app;
    std::cout << "Created BApplication ..." << std::endl;

    foo f;
    std::cout << "Created foo ..." << std::endl;

    f.bar ( 1 );
    std::cout << "Called foo::bar ..." << std::endl;

    std::cout << "About to enter event loop ..." << std::endl;
    int exitCode = app.exec ();
    std::cout << "Exited event loop with code "
              << exitCode << " ..." << std::endl;

    return exitCode;
}
//]
//]
