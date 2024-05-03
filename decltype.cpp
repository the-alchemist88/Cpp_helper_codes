/*
Decltype Specifier
------------------

Inspects the declared type of an entity or the type and value category of an expression.

1) If the argument is an unparenthesized id-expression or an unparenthesized class member access expression, then decltype yields the type of the entity named by this expression.

2) If the argument is any other expression of type T, and

	a) if the value category of expression is xvalue, then decltype yields T&&;
	b) if the value category of expression is lvalue, then decltype yields T&;
	c) if the value category of expression is prvalue, then decltype yields T.
*/


//Inner parentheses cause the statement to be evaluated as an expression

int arr[10]{};

decltype(auto) func(size_t i)           // returns int& to arr[i], without decltype auto would strip off reference qualifier
{
    return arr[i];
}

const int&& fx();
struct A { double dval{}; };

int main()
{
    int x{};

    decltype(x) y = 5;                   // identifier --> same type --> type is int 

    decltype((x)) rx = x;                // expr --> L value --> type is const int&

    rx = 10;

    decltype(rx) ry = y;                 // identifier --> same type --> type is int&
 
    decltype(std::move(x)) rr1 = 7;      // expr --> X value --> type is int&&

    decltype(15) rr2 = 25;               // expr --> PR value --> type is int

    func(rr1) = rr2;                     // returns int& to arr[rr1]

    const A* a = new A();

    decltype(a->dval);	                 // identifier --> same type --> type is double	
    decltype((a->dval));	             // expr --> L value --> type is const double& . And because a is declared as a const pointer, the type is a reference to const double.

    decltype(fx());	                     // expr --> X value --> const int&& 
}
	
// Resource ustilized: https://en.cppreference.com/w/cpp/language/decltype