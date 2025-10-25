## Conversion constructor

It is such a constructor that can implicitly convert an expression of non-class type into an object of class type. Simply, they are ctors with parameters(not only one-paramater ctor).

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

	Myclass& operator=(const Myclass& other)
	{
		std::cout << "Myclass copy asignment for this: " << this << "\n";
		std::cout << "&other = " << &other << "\n";
		return *this;
	}
};

int main()
{
	Myclass m; // no-args ctor will be called
	std::cout << "&m = " << &m << "\n";	// address of m
	m = 35;	// firstly, a temporary object will be created by Myclass(int x) ctor  
			    // then copy assignment operator will be called for copying from temporary object to m. Compare the address of objects copied from and copied to, on the output	
}			    // dtor of m will be called
```
<ins>Possible Output</ins>  
Myclass() for this: 000000682FB6F5D4  
&m = 000000682FB6F5D4  
Myclass(int x) x = 35 for this: 000000682FB6F6B4  
Myclass copy asignment for this: 000000682FB6F5D4  
&other = 000000682FB6F6B4  
~Myclass() for this: 000000682FB6F6B4  
~Myclass() for this: 000000682FB6F5D4  

## User Defined Conversion(UDC)

Type conversions that are performed by compiler via calling functions, which are defined by programmer for non-standard type conversions, are called UDC.
These functions are:

1) Conversion constructor  
2) Type-cast operator function

C++ has a very important rule about implicit conversions:

1) standard conversion + UDC --> OK
2) UDC + standard conversion --> OK
3) UDC + UDC --> not OK !!!

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
								// B() expression goes through function to pointer conversion
	
}
```
However this feature can lead to quite absurd and unintentional conversions made by the programmer:
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

	m = pd; // valid !
}
```
In order to prevent unwanted conversions, usually one parameter ctors of classes are declared with "explicit" keyword. This can be thought as "explicit only", meaning only explicit
conversions can employ UDC. Note that "explicit" keyword forbids only implicit conversions with copy initialization syntax. It is also possible to make other ctors explicit. Ex:

```cpp
class Myclass
{
public:
	explicit Myclass(int) {};
};

int main()
{
	Myclass m1{1};
	Myclass m2(2);
	Myclass m3(3.); 
	Myclass m4 = 10; // error, one paremeter ctor is explicit
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
