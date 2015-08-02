////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/object/utils/bytearray.hpp>
#include <cstdio>

//[ basic_bytearray_cpp_main
int main()
{
    BByteArray ar1 ( "abcdef" );
    unsigned char * tmp = 0;

    // Creating another bytearray that shares the internal data with ar1
    BByteArray ar2 = ar1;

    printf("ar1 internal buffer address: %p\n", (void*)ar1.constData());
    printf("ar2 internal buffer address: %p\n\n", (void*)ar2.constData());

    // performing a non read-only operation on ar2, using the non-const version
    // of BByteArray::data()
    tmp = ar2.data();

    printf("ar1 internal buffer address: %p\n", (void*)ar1.constData());
    printf("ar2 internal buffer address: %p\n\n", (void*)ar2.constData());

    // performing another non read-only operation on ar2, but this time, ar2 is
    // not shared, thus it doesn't need to copy its internal data
    tmp = ar2.data();

    printf("ar1 internal buffer address: %p\n", (void*)ar1.constData());
    printf("ar2 internal buffer address: %p\n\n", (void*)ar2.constData());

    return 0;
}
//]
//[ basic_bytearray_cpp_main_possible_output
/*`
[pre ar1 internal buffer address: 007C9918
ar2 internal buffer address: 007C9918

ar1 internal buffer address: 007C9918
ar2 internal buffer address: 00797410 // changed

ar1 internal buffer address: 007C9918
ar2 internal buffer address: 00797410 // same]
*/
//]

