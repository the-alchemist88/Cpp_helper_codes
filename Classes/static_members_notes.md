# Static Members

## Static Data Memebers

Static members of classes don't belong to the class object. They are global entities, however, since they are located in class scope, name lookup rules
will change accordingly. Also, access control will be applied to them as other members. By in-class declaration, their logical relation with the class is emphasized.

- Their storage is not inside class object.
```cpp
class Myclass
{
public:
	int a{};
	static int x1;
	static int x2;
	static int x3;
	static int x4;
};

int main()
{
	std::cout << "sizeof Myclass" << sizeof Myclass;
}
```
<ins>Output</ins>  
sizeof Myclass = 4

- Don't confuse declaration and definition of static members.
```cpp
class Myclass
{
public:
	int a{};
	static int x; // non-defining declaration, compiler doesn't allocate storage for x
};

int main()
{
	Myclass::x = 5; // link-time error
}
```

- Typically static members are only declared(not defined) in .h files and defined in .cpp files. 
```cpp
// Myclass.h
class Myclass
{
public:
	int a{};
	static int x; // declaration only
};

// Myclass.cpp
int Myclass::x; // Note that static keyword shouldn't be placed in definition, otherwise compiler will complain(similar to explicit keyword usage)  
		            // Default init will firstly make x zero-initialized here as for all static storage variables
```

- A class cannot have a data member of incomplete type. However a static data member can be an incomplete type.

```cpp
class Demo;

class Myclass
{
	Demo dx; // error
	static Demo dy; // valid
};
```

- A class cannot have a data member of its own type, but can have it as a static data member.
  
```cpp
class Myclass
{
	Myclass mx; // error
	static Myclass my; // valid
};
```
- Static data members are constructed before main function is called, same as global variables.
  
```cpp
class Myclass
{
public:
	int a{};
	static int x;
};

int fun()
{
	cout << "fun() is called\n";
	return {};
}

int Myclass::x = fun();

int main()
{
	std::cout << "main() started\n";
}
```
<ins>Output</ins>  
fun() is called  
main() started

- Recall that static objects won't be conctructed, even compiler allocates storage for them, unless the function that they are located in is called. And they will be constructed only once even if multiple calls occur.

```cpp
class Myclass {
public:
	Myclass() {
		std::cout << "Myclass() called for this object : " << this << '\n';
	}

	~Myclass() {
		std::cout << "~Myclass() called for this object: " << this << '\n';
	}
};

void foo()
{
	static Myclass ms;	// will be constructed once
	static int call_counter;

	std::cout << "call_counter = " << ++call_counter << '\n';
}

int main()
{
	foo();
	foo();
	foo();
	foo();
}
```

<ins>Possible Output</ins>    
Myclass() called for this object : 00007FF7354E0180  
call_counter = 1  
call_counter = 2  
call_counter = 3  
call_counter = 4  
~Myclass() called for this object: 00007FF7354E0180  

- Constructor initializer list(CIL) cannot initialize static data members of the class.
  
```cpp
class Myclass
{
	static int svar;
	Myclass(int x): svar{x}; // error
};
```

- Since before modern C++, const static integral type data members could be initalized within the class.
  
```cpp
class Myclass
{
public:

	const static int ival = 10; 	// valid
	const static char cval = 'a'; 	// valid
	const static char double = 0.5; // erorr
};
```

- Since C++17 standard, inline definition became possible for global variables and static data members.
  
```cpp
class Myclass
{
	inline static const char* s{ "Hello" };
}
```

- constexpr keyword implicitly qualifies members as inline and const.
  
```cpp
class Myclass
{
	constexpr static int year{ 2024 };
}
```

In practice, static data members are not located in public access, but const or constexpr static data members can be put in public access.

## Static Member Functions

From a simple point of view, static member functions are global functions with subtle differences. Firstly, they are declared inside class their name lookup rules will change accordingly. Also they can access protected and private members of
the class since they are member functions. Comparing them with non-static member functions, they don't have hidden class pointer("this") parameter as non-static member functions do. Nevertheless they are logically connected with the class. 

General features:

- Static member functions can directly call other static member functions and use static data members with their unqualified names.
- Static member functions cannot call non-static member functions and cannot use nonstatic data members without a qualified name.
- Static member functions cannot qualified with const keyword.
- "this" pointer cannot be used inside static member functions since they have no hidden class pointer as non-static member functions.

Ex:
```cpp
int init()
{
	return 30;
}

class Myclass {
public:

	Myclass(int x): svar(x) {)  // error, svar is a static variable thus it is already generated before main. In CIL, only non-static data members can be initialized 

	static void foo()
	{
		bar(); // bar is a non-static member function thus it has a hidden Myclass* paramater, consequently it must be called for an object
		Myclass mx;
		mx.bar(); // valid, bar() is qualified with an object name

		fun();

		svar = 5;
		var = 10; // error, similar reason with calling directly bar() function with its name, because static functions don't have "this" pointer
		mx.var = 10;
	}

	void bar() {}
	static void fun() {}

	void baz() {}
	static void baz() {} // error, static keyword doesn't generate overlaod
	void baz(int) {}

	static int init()
	{
		return 20;
	}

	static int svar;
	int var{};
};

int Myclass::svar = init(); // Note that there are two init() function in the code, one is a free function, the other is a static member function. Since svar is declared as a static
                            // data member in the class, during its initatlization, function name will be first searched inside class according to name lookup rules
int main()
{
	std::cout << Myclass::svar;
}
```
<ins>Output</ins>    
20  

The typical contexts where static members are used:

- Named Constructors
- Singleton patterns (shown examples above)
- Object count (there is an example in this repo)
- Factory functions
- Dynamic-only objects

Note that since classes are typically declared in header files, in cpp files client cannot directly see a member function's access category or whether it is static or non-static. Therefore, some programmers prefer to do a so-called trick via preprocessor commands.

```cpp
// Myclass.h

class Myclass {
public:
	static void foo();

// Myclass.cpp

#include "Myclass.h"

#define PUBLIC
#define STATIC

PUBLIC STATIC void Myclass::foo()
{
}
```
