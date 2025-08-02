## Conversion constructor

It is such a constructor that, in addition to its main raison d'être, it implicitly converts a non-class expression into a class type. Simply, they are ctors with parameters.

To understand what the compiler is doing in the background, the following code will help:
```
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
	Myclass m;								// no-args ctor will be called
	std::cout << "&m = " << &m << "\n";		// address of m
	m = 35;									// Firstly, a temporary object will be created by Myclass(int x) ctor.
											// Then copy assignment operator will be called for copying from temporary object to m. Compare the address of objects copied from and copied to, on the output	
}											// dtor of m will be called
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

Type conversions performed by the compiler by calling functions defined by the programmer for non-standard type conversions are called UDC.
These functions are:

1) Conversion constructor  
2) Type-cast operator function

C++ has a very important rule about implicit conversions:

1) standard conversion + UDC --> OK
2) UDC + standard conversion --> OK
3) UDC + UDC --> not OK !!!

Example:
```
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
	A ax = 5;		// valid : UDC (int --> A)
	B bx = ax;		// valid : UDC (A --> B)
	B bx1 = 7.5;	// valid : standard conversion(double --> int) + UDC (int --> B)
	C cx = ax;		// error ! : UDC(A --> B) + UDC(B --> C) 
	C cx1 = static_cast<B>(10); // valid : excplicit conversion + UDC
	C cx2(B());		// most vexing parse! This is a function declaration with return type of class C and parameter type of B(*)() 
					// B() expression goes through function to pointer decay
	
}
```
However this feature can lead to quite absurd and unintentional conversions made by the programmer:
```
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
conversions can employ UDC. Note that "explicit" keyword forbids only implicit conversions with copy initialization syntax. It is also possible to make other ctors explicit.

```
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
	Myclass m4 = 10; // error! one paremeter ctor is explicit
	Myclass m5 = static_cast<Myclass>(5); // valid, explicit conversion
}
```

There are some cases where compiler doesn't generate code for copying intentionally even in source file  even when the language syntax visually suggests a copy/move
(e.g. copy initialization), no copy/move is performed.

Until C++17 "copy elison" was a compiler optimization. In C++17 standard some cases of copy elison became mandotory and they are called as mandatory copy elision. Actually even if
this name denotes an elision, there is no elision of any copying here since omitting it became mandatory by the standard.

Example:
```
class Myclass
{
public:
	Myclass()
	{
		std::cout << "Myclass()\n";
	}

	explicit Myclass(int x)
	{
		std::cout << "Myclass(int x)\n";
	}

	Myclass(Myclass&& other)
	{
		std::cout << "Myclass(Myclass&& other)";
	}

	Myclass(const Myclass& other) 
	{
		std::cout << "Myclass(const Myclass& other)\n";
	}
};

void func(Myclass mx){}

int main()
{
	func(Myclass{10}); 	// Straightforward thinking, first a temporary object should be constructed with Myclass(int x) ctor.
						// Then since it is an r value, move ctor gets called to construct the function parameter mx. However this is atypical copy elision case.
}
```

Without any copy elision this code would give the following output(can be tested with "-fno-elide-constructors" option in GCC):
```
Myclass(int x)
Myclass(Myclass&& other)
```
But instead the output is(since C++17 compiled):

`Myclass(int x)`
