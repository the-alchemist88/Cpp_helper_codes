# Run Time Type Identification(RTTI)

Run-time type identification (RTTI) is a mechanism that allows the type of an object to be determined during program execution. RTTI was added to the C++ language because many vendors 
of class libraries were implementing this functionality themselves. This caused incompatibilities between libraries. Thus, it became obvious that support for run-time type information
was needed at the language level.

The main advantage of dynamic polymorphism is being able to use a virtual base class (through a pointer or reference) without knowing its dynamic type.
This eliminates the dependency of new code (top-layer code — overriders) on old code (lower-layer code — base class virtual functions).

Tools for determining the dynamic type at runtime(RTTI tools):

### 1) dynamic_cast

- dynamic_cast operator examines whether downcasting is safe or not.

- Typically used for casting a pointer/reference to a base class type to a pointer/reference to a derived class type (downcasting).
```cpp
void fun(Car* cptr) // assume that Volvo is publicly derived from Car class so this functian can operate virtual dispatch
{
	if(auto* vptr = dynamic_cast<Volvo*>(cptr)) // if, at run-time, cptr points to an Volvo object then dynamic_cast is successful and aptr points to that object
	{								    		// otherwise dynamic_cast returns nullptr
		// code
	}
}										   
```
- To be able to perform a downcast with dynamic_cast, the source type (usually the base class) must be polymorphic, i.e., it must have at least one virtual function.

- When downcasting is applied by reference semantics, if it fails at runtime compiler will throw exception(std::bad_cast) in this case.
That’s why pointer semantics are often preferred (no exception, just nullptr).
```cpp
void car_game(Car& cr) 
{
	try
	{
		auto& vr = dynamic_cast<Volvo&>(cr);
		vr.open_sunroof();
		(void)getchar();
	}
	
	catch(const exception& ex)
	{
		cout << "exception caught: " << ex.what() << '\n'; 
	}
	
}				

int main()
{
	Car* cptr = create_random_car();
	fun(cptr);
}
```
- The dynamic_cast operator scans down the hierarchy: if the dynamic type that will be operand to dynamic_cast, is a type of a derived class that is lower
in a multilevel inheritance hierarchy than the type to be casted, this is not a syntax error, it is converted to operand's dynamic type
```cpp
class Volvo : public Car
{
public:
	virtual void foo()
	{
		cout << "Volvo foo()\n";
	}
};

class VolvoXC90 : public Volvo
{
public:
	void foo() override
	{
		cout << "VolvoXC90 foo()\n";
	}
};

void fun(Car* cptr)
{
	if (auto* dptr = dynamic_cast<Volvo*>(cptr))
	{
		dptr->foo();
	}
}		

int main()
{
	VolvoXC90 vxcp;

	fun(&vxcp);	//  by this call dynamic_cast will cast cptr into VolvoXC90*, not Volvo*
}
```

<ins>Output:</ins>  
VolvoXC90 foo()

### 2) typeid
```cpp
#include <typeinfo>

// Usage:
	typeid(type);
	typeid(expr);
```
- The types of expressions used as operands of the typeid operator do not have to be related by inheritance, unlike the dynamic_cast operator.

- The operand of the typeid operator is an unevaluated context, like sizeof. However, if the operand is a glvalue of a polymorphic type, it is evaluated to obtain the dynamic type.

```cpp
class Base {};   // not a polymorphic class

Base* p = nullptr;
typeid(*p);       // doesn't throw exception, also no UB
                  // static type: Base
```

When the operand is a glvalue of polymorphic type:

```cpp
struct Base {
    virtual ~Base() = default;  // polymorphic
};

int main() {
    Base* p = nullptr;

    try {
        typeid(*p);  // throws std::bad_typeid 
    }
    catch (const std::bad_typeid& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }
}
```

- An expression formed with the typeid operator yields an lvalue of type const std::type_info.

	typeid(Myclass); // type of this expression is const type_info
	const type_info& ti = typeid(Myclass); // correct syntax
	
- The default constructor of std::type_info does not exist and its copy constructor is deleted.
	
- There are two commonly used function of type_info class:
	
	- operator== (to compare two types)
	
		class Myclass{};
		Myclass mx;
		
		auto b = typeid(Myclass) == typeid(mx);
		
	-  name() (returns const char*)

	returns the type as C-string. Implementation dependant(depends on compiler)
	
- The behaviour of typeid operator depends on whether the types are polymorphic or non-polymorphic. Ex:
 ```cpp
class Base{
	public:
	virtual ~Base() = default; 
};

class Der : public Base
{

};

int main()
{
	Der myder;
	Base* baseptr = &myder;
	std::cout << (typeid(*baseptr) == typeid(Der)) << '\n'; // if Base was non-polymorphic then this expression would print 0, it would be valid but related to static type
	std::cout << typeid(*baseptr).name(); // if Base was non-polymorphic this would print class Base
}
```
<ins>Output:</ins>  
1  
Der  
													
- Unlike dynamic_cast operator, typeid operator doesn't scan down the inheritance hierarchy, it can be used to find exact match of types. Therefore, dynamic_cast is generally
more expensive than typeid.

- Since typeid doesn't do any casting, static_cast can be used to access the derived class member functions. static_cast is valid for the types that are in the same hierarchy but 
programmer always need to check whether the dynamic_types match or not. Without checking the type, it can be undefined behaviour.
```cpp
class Base 
{
public:
    virtual ~Base() = default;
};

class Der: public Base
{
public:
    void bar()
    {
        std::cout << "Der bar()\n";
    }
};


void func(Base* bptr)
{
    if (auto b = typeid(*bptr) == typeid(Der))
    {
        auto vptr = static_cast<Der*>(bptr);  // calling directly bptr->baz() without casting, may cause undefined behaviour if base class doesn't have that function for example

        vptr->bar();
    }
}

int main()
{
    Der myder;
    func(&myder);
	
	try
    {
        func(nullptr); // if argument is nullptr at runtime typeid operator throws exception of type bad_typeid
    }
    catch(const bad_typeid& ex)
    {
        cout << "exception caught: " << ex.what();
    }
}
```
<ins>Output</ins>  
Der bar()  
exception caught: std::bad_typeid  

## RTTI Tools(dynamic_cast & typeid) Summary

- dynamic_cast operator tests whether downcasting (conversion from base class to derived class) is possible.
- The expression that is the operand of dynamic_cast must be of a polymorphic type.
- dynamic_cast also scans down the hierarchy. dynamic_cast<T*>(base_ptr) succeeds if the dynamic type of *base_ptr is: T, or publicly derived from T.

- The expression that is the operand of the typeid operator does not have to be of a polymorphic type.
- If it is a polymorphic type, the dynamic type concept is applied and the type is determined at runtime, if it is not polymorphic, the static type concept is taken as a basis.
- typeid also does not scan the hierarchy, it is based on only the type of the expression which is its operand.

- The static_cast operator can be used to determine at compile time whether two different classes are in the same inheritance hierarchy.
It will be legal to cast classes in the same hierarchy to each other with static_cast, so there will be no need to examine the header files.


## Other Related Topics

-There are several ways way to check if two classes in the same hierarchy in compile time:
```cpp
static_cast<Der*>((Base*)0); // if this is valid, they Base and Der are in the same hierarchy

static_assert(std::is_base_of_v<Base, Der>);
```
- There a few keywords that form unevaluated context(for expressions which compiler doesn't produce code):

sizeof, typeid, decltype

- Unevaluated context has two features:
 
	- Eliminates side effects( sizeof(x++) --> x doesn't get incremented)

	- Eliminates undefined behaviour( a[10]{}, sizeof(a[40]) --> not ub)
