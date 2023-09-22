#include "car.h" 
#include <cstdlib> 

void car_game(Car* p)
{
    Car* pnewcar = p->clone(); // thereis no virtual
    p->test_car();
    (void)getchar();
    system("cls");
}

//void car_game(Car& p)     // ok, reference semantics allow virtual dispatch
//{
//p->test_car();
//(void)getchar();
//system("cls");
//}

//void car_game(Car p)      // object slicing.. virtual dispatch cannot operate
//{
//p->test_car();
//(void)getchar();
//system("cls");
//}

int main()
{
    Car* ptr{};
    for (size_t i{}; i < 10; ++i)
    {
        ptr = create_random_car();
        cout << *ptr << "\n";
        car_game(ptr);
    }

}

/*

Notes:

1) Virtual functions can be placed in public, protected and private sections of base class and can be orverriden also in each section of derived class.
As long as you they are called via pointer/reference to base class type, virtual dispatch mechanism will be on action. In this example:
void car_game(Car* p){ p->test_car();}

2) Name lookup, context control and access control are related to static checks. These are done in compile-time.
Default arguments of function parameters are also evaluated in compile time, therefore the parameter will take base class virtual funciton's argument.

3) There are cases when virtual dispatch mechanism doesn't get activated:

    a) In object slicing(initializing a base class object with a derived class object)
    b) Virtual function calls that made inside base class ctors, becuase if otherwise, base class would try to use the members/resources of an
object(of derived class) that has not constructed yet. Note that construction starts from base class and continues to derived class.
    c) Virtual function calls that made inside base class dtor, becuase if otherwise, base class would try to use the members/resources of a destroyed object.
Note that destruction is done in the reverse order of construction, first members/resources of derived class and then base class are destroyed.
    d) When a member is used with resolution operator in base class. For ex, Base::a

4) Virtual dispatch mechanism has costs:

    a) For each polymorphic class a virtual function table is formed by compiler. It's the same table for all objects of this class. This
structure is created during runtime and employs dynamic storage, therefore allocating and freeing this memory will come with a considerable cost.

    b) Each polymorphic class object has a virtual pointer that points to its own virtual function table. You can check the size 
difference between a polymorphic class and a class that has no virtual funcitons. Adding more than one virtual function won't change the class size.
Consequently, when virtual dispatch mechanism is in action, accessing to a overridden function shall require double dereferencing.
Ex: virtPtr->table[idx]

5) "Final" contextual keyword can be used by compilers to do devirtualization(an optimization about inferring which function will be called during 
compile-time, when virtual functions/classes are involved)

5) Herb Sutter's advice: Dtor of the polymorphic base classe should be either:

    a) Public virtual, or

    b) Protected non-virtual.

6) C++ is a hybrid language, it's not specifically object-oriented. In fact, its strength lies in generic programming paradigm.
There will be alternatives of different methodologies based on trade-offs.

*/