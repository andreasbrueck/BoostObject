#include <boost/object/application.hpp>

BOOST_OBJECT_INSTALL

namespace unsafe
{
    class myObject : public BObject
    {
        B_OBJECT(myObject)
           
    private:
        //[advanced_connection_cpp__mySlot_implementation
        //[advanced_connection_cpp__mySlot_implementation_declaration
        void _slot_mySlot ( int i )
        //]
        {
            std::cout << "mySlot(" << i <<") called" << std::endl;
        }
        //]
            
    public:
        //[advanced_connection_cpp__mySlot
        auto_slot(mySlot)
        //]

        //[advanced_connection_cpp__signals
        bsignal(mySignalInt,void(int))
        bsignal(myOtherSignalInt,void(int))
        bsignal(mySignalVoid,void())
        //]
    };
}

BOOST_SAFE_OBJECT(myObject)

//[advanced_connection_cpp__myFunction
void myFunction ( int i )
{
    std::cout << "myFunction(" << i << ") called" << std::endl;
}
//]

//[advanced_connection_cpp__myStaticFunction
struct myStruct
{
    static void myStaticFunc ( int i )
    {
        std::cout << "myStaticFunc(" << i << ") called" << std::endl;
    }
};
//]

int main()
{
    BApplication app;
    myObject obj;
    myObject context_obj;
    
    //[advanced_connection_cpp__simple_slot
    obj.mySignalInt.connect(obj.mySlot);
    //]

    //[advanced_connection_cpp__simple_signal
    obj.mySignalInt.connect(obj.myOtherSignalInt);
    //]

    //[advanced_connection_cpp__simple_function
    obj.mySignalInt.connect(&myFunction);
    //]

    //[advanced_connection_cpp__simple_static_function
    obj.mySignalInt.connect(&myStruct::myStaticFunc);
    //]

    //[advanced_connection_cpp__simple_lambda
    obj.mySignalInt.connect([&](int i)
        {
            std::cout << "lambda(" << i << ") called" << std::endl;
        }
    );
    //]

    //[advanced_connection_cpp__defaultarg_slot
    obj.mySignalVoid.connect(obj.mySlot,123);
    //]

    //[advanced_connection_cpp__defaultarg_signal
    obj.mySignalVoid.connect(obj.mySignalInt,123);
    //]

    //[advanced_connection_cpp__defaultarg_function
    obj.mySignalVoid.connect(&myFunction,123);
    //]

    //[advanced_connection_cpp__defaultarg_static_function
    obj.mySignalVoid.connect(&myStruct::myStaticFunc,123);
    //]

    //[advanced_connection_cpp__defaultarg_lambda
    obj.mySignalVoid.connect([&](int i)
        {
            std::cout << "lambda(" << i << ") called" << std::endl;
        },
        123
    );
    //]

    //[advanced_connection_cpp__context_function
    obj.mySignalInt.connect(&context_obj,&myFunction);
    //]

    //[advanced_connection_cpp__context_static_function
    obj.mySignalInt.connect(&context_obj,&myStruct::myStaticFunc);
    //]

    //[advanced_connection_cpp__context_lambda
    obj.mySignalInt.connect(&context_obj,[&](int i)
        {
            std::cout << "lambda(" << i << ") called" << std::endl;
        }
    );
    //]

    obj.mySignalInt.emit(456);
    std::cout << "------" << std::endl;
    obj.mySignalVoid.emit();

    return 0;
}

