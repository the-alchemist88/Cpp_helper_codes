#include "car.h" 
#include <cstdlib> 

void car_game(Car* p)
{
    p->test_car();
    (void)getchar();
    system("cls");
}

//void car_game(Car& p)     // ok, reference semantics allow virtual dispatch
//{
//    p.start();
//    p.run();
//    p.stop();
//    (void)getchar();
//}

//void car_game(Car p)      // object slicing.. virtual dispatch cannot operate
//{
//    p.start();
//    p.run();
//    p.stop();
//    (void)getchar();
//}

int main()
{
    Car* ptr{};
    for (size_t i{}; i < 10; ++i)
    {
        ptr = create_random_car();
        car_game(ptr);
    }

}

/*

Notes:

1) Virtual functions can be overriden in public, protected and private sections of derived class.
It won't cause problem as long as you call them with a function that take base class pointer/reference ("void car_game(Car* p)" in this example)

2) Name lookup, context control and access control are related to static checks. Compiler will do these in compile-time.

*/