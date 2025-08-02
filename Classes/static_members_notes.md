# Static Members

## Static Data Memebers

Static members of classes don't belong to the class object. They are global entities, however, since they are located in class scope, name lookup rules
will change accordingly. Also, access control will be applied to them as other members. By in-class declaration, their logical relation with the class is emphasized.

- Their storage is not inside class object.
```
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
**Output**
sizeof Myclass = 4

- Don't confuse declaration and definition of static members:
```
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

// Myclass.h
class Myclass
{
public:
	int a{};
	static int x; // declaration only
};

// Myclass.cpp
int Myclass::x; // Note that static keyword shouldn't be placed in definition otherwise compiler will complain(similar to explicit keyword usage)
		// Default init will firstly make x zero-initialized here as for all static storage variables


- A class cannot have a data member of incomplete type. However a static data member can be an incomplete type.
```
class Demo;

class Myclass
{
	Demo dx; // error
	static Demo dy; // valid
};
```

- A class cannot have a data member of type its own type, but it can have, as a static data member.
```
class Myclass
{
	Myclass mx; // error
	static Myclass my; // valid
};
```
- Static data members are constructed before main function is called, same as global variables.
```
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
**Output**

fun() is called
main() started

- Constructor initializer list(CIL) cannot initialize static data members of the class.
```
class Myclass
{
	static int svar;
	Myclass(int x): svar{x}; // error
};
```

- Since before modern C++, const static integral type data members could be initalized within the class.
```
class Myclass
{
public:

	const static int ival = 10; 	// valid
	const static char cval = 'a'; 	// valid
	const static char double = 0.5; // erorr
};
```

- Since C++17 standard, inline definition became possible for global variables and static data members.
```
class Myclass
{
	inline static const char* s{ "Hello" };
}

- constexpr keyword implicitly adds inline and const feature to members.

class Myclass
{
	constexpr static int year{ 2024 };
}
```

## Static Member Functions

Actually they are global functions, but since they are declared inside class their name lookup rules will change accordingly. Also they can access protected and private members of
the class. They don't have hidden class pointer("this") parameter as non-static member functions do. They are logically connected with the class. 

General features:

- Static member functions can directly call other static member functions and use static data members with their unqualified names.
- Static member functions cannot call non-static member functions and cannot use nonstatic data members without a qualified name.
- Static member functions cannot qualified with const keyword.
- "this" pointer cannot be used inside static member functions since they have no hidden class pointer as non-static member functions.

Ex:
```
int init()
{
	return 30;
}

class Myclass {
public:

	static void foo()
	{
		bar(); // bar is a non-static member function thus it has a hidden Myclass* paramater, consequently it must be called for an object
		Myclass mx;
		mx.bar(); // valid, bar() is qualified with an object name

		fun();

		svar = 5;
		nsvar = 10; // error, similar reason with calling directly bar() function with its name, because static gunctions don't have "this" pointer
		mx.nsvar = 10;
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
	int nsvar{};
};

int Myclass::svar = init(); // Note that there are two init() function in the code, one is a free function, the other is a static member function. Since svar is declared as a static
							// data member in the class, during its initatlization, function name willl be first searched inside class according to name lookup rules
int main()
{
	std::cout << Myclass::svar;
}
```
**Output**
20
