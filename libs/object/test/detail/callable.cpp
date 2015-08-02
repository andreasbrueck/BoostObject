#include <boost/object/detail/callable.hpp>
#include <type_traits>

struct functor_1
{
    void operator () () const;
};

class functor_2
{
public:
    functor_1 operator () ( functor_2 & );
};

struct functor_3
{
    static int func ( int );
};

void function ( functor_3&& );

int main ()
{
    using namespace boost::object::detail;

    auto lambda = [=] ( double, int ) {};
    auto mutable_lambda = [&] ( int ) mutable {};

    static_assert(is_callable<decltype(lambda)>::value,"");
    static_assert(std::is_same<callable_signature<decltype(lambda)>::type,void(double,int)>::value,"");

    static_assert(is_callable<decltype(mutable_lambda)>::value,"");
    static_assert(std::is_same<callable_signature<decltype(mutable_lambda)>::type,void(int)>::value,"");

    static_assert(is_callable<functor_1>::value,"");
    static_assert(std::is_same<callable_signature<functor_1>::type,void()>::value,"");

    static_assert(is_callable<functor_2>::value,"");
    static_assert(std::is_same<callable_signature<functor_2>::type,functor_1(functor_2&)>::value,"");

    static_assert(!is_callable<functor_3>::value,"");

    static_assert(is_callable<decltype(&functor_3::func)>::value,"");
    static_assert(std::is_same<callable_signature<decltype(&functor_3::func)>::type,int(int)>::value,"");

    static_assert(is_callable<decltype(&function)>::value,"");
    static_assert(std::is_same<callable_signature<decltype(&function)>::type,void(functor_3&&)>::value,"");

    return 0;
}
