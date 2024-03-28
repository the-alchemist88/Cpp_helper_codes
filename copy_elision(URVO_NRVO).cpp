#include <iostream>

class Myclass
{
public:
    Myclass() { std::cout << "Myclass()\n"; };
    Myclass(const Myclass&) { std::cout << "Myclass(const Myclass&)\n"; };
    //Myclass(const Myclass&) = delete; // uncommenting this line will cause compiler to throw error because of f_NRVO funciton
};


Myclass f_URVO() // URVO - unnamed return value optimization
				 // In C++17 and later, URVO is mandatory and no longer considered a form of copy elision
{
    return Myclass{};
}

Myclass f_NRVO() // NRVO - named return value optimization
{                // NRVO is not mandatory, also copy ctor must be callable even if f_NRVO doesn't call it
    Myclass m;
    return m;
}

int main()
{
    auto obj1 = f_URVO();
    auto obj2 = f_NRVO();
}


/*
 URVO
----------------------------------------------
In the initialization of an object, when the source object is a nameless temporary and is of the same class type (ignoring cv-qualification) as the object. 
When the nameless temporary is the operand of a return statement, this variant of copy elision is known as URVO, "unnamed return value optimization."
(In C++17 and later, URVO is mandatory and no longer considered a form of copy elision)


 NRVO
----------------------------------------------
In a return statement, when the operand is the name of a non-volatile object with automatic storage duration, which isn't a function parameter or a catch
clause parameter, and which is of the same class type (ignoring cv-qualification) as the function return type. This variant of copy elision is known as NRVO,
"named return value optimization."
*/

