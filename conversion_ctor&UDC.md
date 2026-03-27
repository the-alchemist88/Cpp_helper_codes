## Conversion constructor

A conversion constructor is a constructor that enables conversion from another type to the class type. 
Typically, they are constructors that can be called with a single argument (possibly via default arguments) 
and thus participate in implicit conversions, although constructors with multiple parameters may also 
enable conversion in certain contexts (e.g., list-initialization).

In order to understand what the compiler is doing in the background, the following example may help:
```cpp
class Myclass
{
public:
	Myclass()
	{
		std::cout << "Myclass() for this: " << this << '\n';
	}

	~Myclass()
	{
		std::cout << "~Myclass() for this: " << this << '\n';
	}

	Myclass(int x) // Conversion ctor
	{
		std::cout << "Myclass(int x) x = " << x << " for this: " << this << '\n'; // address of the temporary object to be created by converting from int to Myclass
	}

	Myclass(int x, int y) // Conversion ctor
	{
		std::cout << "Myclass(int x, int y) x = " << x << " y = " << y <<  " for this: " << this << '\n'; // address of the temporary object to be created by converting from int to Myclass
	}

	Myclass& operator=(const Myclass& other)
	{
		std::cout << "Myclass copy asignment for this: " << this << "\n";
		std::cout << "&other = " << &other << "\n";
		return *this;
	}
};

int main()
{
    // no-args ctor will be called
	Myclass m;
    // address of m
	std::cout << "&m = " << &m << "\n";
    // firstly, a temporary object will be created by Myclass(int x) ctor
    // then copy assignment operator will be called for copying from temporary object to m
    // compare the address of objects copied from and copied to, on the output
    // dtor of m will be called
	m = 35;
	m = {5, 6}; // this is also valid but not used in practice
}			    
```
<ins>Possible Output</ins>  
Myclass() for this: 00000038F52FFBA4  
&m = 00000038F52FFBA4  
Myclass(int x) x = 35 for this: 00000038F52FFC84  
Myclass copy asignment for this: 00000038F52FFBA4  
&other = 00000038F52FFC84  
~Myclass() for this: 00000038F52FFC84  
Myclass(int x, int y) x = 5, y = 6 for this: 00000038F52FFCA4  
Myclass copy asignment for this: 00000038F52FFBA4  
&other = 00000038F52FFCA4  
~Myclass() for this: 00000038F52FFCA4  
~Myclass() for this: 00000038F52FFBA4  

## User-Defined Conversion(UDC)

Type conversions that are performed by compiler via calling functions, which are defined by programmer for non-standard type conversions, are called UDC.
These functions are:

- Conversion constructor  
- Type-cast operator function

C++ has a very important rule about implicit conversions: An implicit conversion sequence can contain at most one UDC.

- standard conversion + UDC --> OK
- UDC + standard conversion --> OK
- UDC + UDC --> not OK !!!

Example:
```cpp
class A
{
public:
	A(int){};
};

class B
{
public:
	B(int){};
	B(A){};
};

class C
{
public:
	C(B){};
};

int main()
{
	A ax = 5;			// valid, UDC (int --> A)
	B bx = ax;		// valid, UDC (A --> B)
	B bx1 = 7.5;	// valid, standard conversion(double --> int) + UDC (int --> B)
	C cx = ax;		// error, UDC(A --> B) + UDC(B --> C) 
	C cx1 = static_cast<B>(10); // valid, excplicit conversion + UDC
	C cx2(B());		// most vexing parse, this is a function declaration with return type of class C and parameter type of B(*)() 
}
```
Side Note: Why in the above example "B()" looks like an object but is not one?

At first glance, B() appears to be a temporary object construction. In many contexts, that intuition is correct. However, in the declaration "C cx2(B());" B() is not an object construction at all.
The reason is that this line appears in a declaration context. According to the C++ grammar, if a statement can be parsed both as a declaration and as an expression, the declaration interpretation is always chosen. 
This rule is what leads to the so-called “most vexing parse”. The compiler does not interpret B() as “construct a temporary B object”. Instead, it interprets it as a function type taking no parameters and returning B. 
As a result, the full statement is parsed as: C cx2( B (*)() );This means that cx2 is a function name, the return type is C, and the parameter is a pointer to a function that takes no parameters and returns B. 
The following syntax all construct an object:

```cpp
C cx2((B()));

C cx2{B{}};

B tmp{};
C cx2(tmp);
```

However this rule can lead to quite absurd and unintentional conversions made by the programmer:
```cpp
class Myclass
{
public:
	Myclass() {};
	Myclass(bool) {};
};

int main()
{
	Myclass m;

	double dval{ 2.7 };
	auto pd = &dval;

	// double* → bool (standard conversion)
	// bool → Myclass (UDC)
	// valid !
	m = pd; 
}
```
In order to prevent unwanted conversions, usually one parameter ctors of classes are declared with _explicit_ keyword. This can be thought as "explicit only", meaning only explicit
conversions can employ UDC. Note that explicit constructors are not considered for implicit conversions (such as copy initialization). Thus _explicit_ keyword forbids implicit conversions
with copy initialization syntax, but can still be used in direct initialization. It is also possible to make other ctors explicit. Ex:

```cpp
class Myclass
{
public:
	explicit Myclass(int) {};
};

int main()
{
	Myclass m1{1};   // direct-list-init → explicit
	Myclass m2(2);   // direct-init → explicit
	Myclass m3(3.);  // double → int, direct-init → explicit
	Myclass m4 = 10; // error, copy init and one paremeter ctor is explicit
	Myclass m5 = static_cast<Myclass>(5); // valid, explicit conversion
}
```

Note that explicit ctor won't be included in function overload set if the object constructed by copy initalization syntax. Ex:

```cpp
class Myclass
{
public:
	explicit Myclass(double) {};
	Myclass(int) {}; // this is called
};

int main()
{
	Myclass m = 45.9; // valid, double converts into int
}
```
