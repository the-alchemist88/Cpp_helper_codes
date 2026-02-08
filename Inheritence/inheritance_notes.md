# INHERITANCE 

- A Derived object contains a complete Base subobject as part of its object representation, possibly followed by additional data members and padding. Therefore, the following relation always holds:  

`sizeof(Derived) >= sizeof(Base)`

- The base class of an inheritance relationship must be a complete type at the point of derivation. The compiler must know the full layout of the base class in order to embed it as a base subobject of the derived class.

- In a derived class, writing a `using declaration` for member functions or data members of the base class can have two purposes:

1) It can be used to bring base class members (those that are accessible, i.e., not private) into the public interface of the derived class, so that client code can use them through the derived type.  

2) It can be used to prevent name hiding and to form a proper overload set. If the derived class declares a function with the same name, it hides all base class functions with that name.
A `using declaration` can be used to bring the base class overloads back into scope.

```cpp
class Base
{
protected:
	void foo(int x)
	{
		cout << "Base::foo(int x)\n";
	}

	void bar(int x)
	{
		cout << "Base::bar(int x)\n";
	}

	void baz(int x)
	{
		cout << "Base::baz(int x)\n";
	}
};

class Derived: public Base
{
public:
	void bar(double x)
	{
		cout << "Der::bar(double x)\n";
	}

	void baz(int x)
	{
		cout << "Derived::baz(int x)\n";
	}

	using Base::foo;
	using Base::bar;
};

// test code
int main()
{
	Derived dx;

	dx.foo(5);	// added base class function to derived class public interface
	dx.bar(5);	// function overloading, base class object bar function called 
	dx.bar(5.);	// function overloading, derived class object bar function called 
	dx.baz(5);	// both derived and base have a function with the same signature;
              // this is not ambiguous. The derived class function is selected.
}
```
<ins>Output</ins>  
Base::foo(int x)  
Base::bar(int x)  
Der::bar(double x)  
Derived::baz(int x)  

Special member functions in inheritance behave similarly to composition. If a special member function of the derived class is implicitly declared or defaulted, the corresponding base class special member function is automatically invoked.
(e.g., the base class default constructor is called before the derived class constructor).

However, if a special member function is user-defined in the derived class, then it becomes the programmer’s responsibility to explicitly invoke the corresponding base class constructor, assignment operator, or destructor.

```cpp
class Base
{
public:
	Base() { cout << "Base()\n"; }

	Base(int, int) { cout << "Base(int, int)\n"; }

	Base(const Base& rhs)
	{
		cout << "Base(const Base& rhs)\n";
	}

	Base(Base&& rhs) noexcept
	{
		cout << "Base(Base&& rhs)\n";
	}

	Base& operator=(const Base& rhs)
	{
		cout << "Base& operator=(const Base& rhs)\n";
		return *this;
	}

	Base& operator=(Base&& rhs) noexcept
	{
		cout << "Base& operator=(Base&& rhs)\n";
		return *this;
	}

	void foo(int x)
	{
		cout << "Base::foo(int x)\n";
	}
};

class Der : public Base
{
public:
	Der() { cout << "Der()\n"; }

	Der(int x, int y) : Base(x, y)
	{
		cout << "Der(int, int)\n";
	}

	Der(const Der& rhs) : Base (rhs)
	{

		cout << "Der(const Der& rhs)\n";
	}

	Der(Der&& rhs) noexcept : Base (std::move(rhs))
	{
		cout << "Der(Der&& rhs)\n";
	}


	Der& operator=(const Der& rhs) 
	{
		Base::operator=(rhs);
		cout << "Der& operator=(const Der& rhs)\n";
		return *this;
	}

	Der& operator=(Der&& rhs) noexcept
	{
		Base::operator=(std::move(rhs));
		cout << "Der& operator=(Der&& rhs)\n";
		return *this;
	}
};

// test code
int main()
{
	Der da;			    // default ctor
	cout << "\n";
	Der db(3, 5);		// (int, int) ctor
	cout << "\n";
	Der dc = db;		// copy ctor
	cout << "\n";
	Der dd = std::move(da);	// move ctor
	cout << "\n";
	da = dd;		    // copy assignment operator
	cout << "\n";
	db = std::move(dc);	// move assignment operator
	cout << "\n";
	db.Base::foo(5);	// calling Base class function through a derived object
	db.foo(5);		    // since it's a public inheritance, derived class object also has base class public functions
}
```

<ins>Output</ins>  
Base()  
Der()  

Base(int, int)  
Der(int, int)  

Base(const Base& rhs)  
Der(const Der& rhs)  

Base(Base&& rhs)  
Der(Der&& rhs)  

Base& operator=(const Base& rhs)  
Der& operator=(const Der& rhs)  

Base& operator=(Base&& rhs)  
Der& operator=(Der&& rhs)  

Base::foo(int x)  
Base::foo(int x)  

Since modern C++11, the language provides the _inheriting constructors_ feature via the `using Base::Base;` syntax. This allows a derived class to inherit the constructors of its base class,
saving the programmer from having to manually forward all base class constructors.

However, the access control of the base class constructors is preserved. If the base class constructors are protected, they can still be inherited, but they will not be accessible to
client code through the derived class.

```cpp
class Base
{
public:
	Base() { cout << "Base()\n"; }

	Base(int)
	{
		cout << "Base(int)\n";
	}

	Base(const char*)
	{
		cout << "Base(const char*)\n";
	}

	Base(int, char)
	{
		cout << "Base(int, char)\n";
	}

protected:
	Base(int, int)
	{
		cout << "Base(int, iny)\n";
	}
	
};

class Der : public Base
{
public:
	using Base::Base;
};

// test code
int main()
{
	Der d1;
	Der d2(5);
	Der d3("Hello");
	Der d4(2, 'c');
	// Der d5(2, 4); // cannot access protected member declared in class 'Base'
}
```
<ins>Output</ins>  
Base()  
Base(int)  
Base(const char*)  
Base(int, char)  

## Inheritance Categories(Public, Private, Protected Inheritance)

### 1) Public Inheritance

- Public inheritance provides an "is-a" relationship, implicit conversion from derived class to base class happens using reference/pointer syntax.
- The public interface of the base class is added to the public interface of the derived class, and the protected interface of the base class is added to the protected interface of the derived class.
- The private part of the base class is closed to the derived class in all inheritance forms.

The situation is different for other types of inheritance that are specific to C++.
 
### 2) Private Inheritance

- In public inheritance, the interfaces of the base class are added to the derived class in the same way:

	base public 		--> 	derived public
	base protected 		--> 	derived protected
	
- However in Private inhertiance:
	
	base public 		--> 	derived private
	base protected 		--> 	derived private
	
This means (private inheritance): Protected and public parts of the base class can still be accessed within the derived class. However, client codes can no longer use the codes of the base class through the derived class.

- There is no is-a relationship. There is no implicit conversion from the derived class to the base class. Upcasting is a syntax error with two exceptions:

- Valid (implicitly) within the derived class's member function
- Valid in functions to which the derived class gives friendship

```cpp
class Base
{
public:
	void foo();
};

class Der : private Base
{
	void bar()
	{
		Der myder;

		Base* bptr = &myder; // valid
		Base& bref = myder; // valid
	}
	friend void func();
};

void func()
{
	Der myder;

	Base* bptr = &myder; // valid
	Base& bref = myder; // valid
}

int main()
{
	Der myder;

	Base* bptr = &myder; // invalid
	Base& bref = myder; // invalid
}
```
- Private inheritance is an alternative to composition (containment, i.e. having a data member of a class type). Differences between the two:

- With composition, we cannot access the `protected` members of the contained object’s class. With private inheritance, we can access the `protected` members of the base class.
- With composition, we cannot override the virtual functions of the contained object directly. With private inheritance, we can override the virtual functions of the base class.
- With composition, there is no conversion from the containing class to the contained class type. With private inheritance, there is a limited and restricted conversion from the derived class to the base class.

On the other hand, composition allows multiple subobjects of the same type (e.g., two members of the same class type, or an array of such members), which is not possible with inheritance,  
since a class can inherit from a given base class only once. Examples that are possible in composition but not in private inheritance:

- A class can have two members of another class type.
- A class can have an array of members of another class type.

In summary, upcasting in private inheritance:

- Not valid in global functions.
- Valid in member functions of the derived class.
- It is also valid for codes where the derived function is considered as friend.

### 3) Protected Inheritance

The most significant difference between private inheritance and protected inheritance:

- base public --> derived protected

In this case, classes derived with multi-level inheritance can access these elements.

Other concepts related to this topic: Multiple Inheritance & Virtual Inheritance

## Multiple Inheritance
```cpp
class BaseX { int x;}
class BaseY { int y;}

class Der : public BaseX, BaseY {}
```
This means class Der : public BaseX is the same as private BaseY. Der object has two separate base classes.  
Thus `sizeof(Der)` will be at least `sizeof(BaseX) + sizeof(BaseY)` in this case (typically 8 bytes on many platforms).


In multiple inheritance, base class subobjects are constructed in the order in which they appear in the inheritance list (from left to right),
not in the order in which they appear in the constructor’s initializer list. The same rule applies in reverse order for destruction.
```cpp
class Der : public BaseX, public BaseY // they are constructed in this order
{
	Der(): BaseY(), BaseX() // not in this order
}
```
Name lookup is done simultaneously in all classes (derived and base classes), so there is no function overloading (scopes are different anyway).
In order to prevent ambiguity here static_cast operator or a qualified name can be used. For example:
```cpp
class BaseX
{
public:
	void foo()
	{
		std::cout << "BaseX foo()\n";
	}
};

class BaseY
{
public:
	int foo(int, int)
	{
		std::cout << "BaseY foo(int, int)\n";
		return {};
	}
};

class Der : public BaseX, public BaseY
{

};

int main()
{
	Der myder;

	myder.BaseX::foo();
	myder.BaseY::foo(1, 2);

	static_cast<BaseX&>(myder).foo();
	static_cast<BaseY&>(myder).foo(1, 2);
}
```
<ins>Output</ins>  
BaseX foo()  
BaseY foo(int, int)  
BaseX foo()  
BaseY foo(int, int)  

Example with qualified names:

```cpp
class Base {};

class BaseX: public Base
{
public:
	void foo()
	{
		std::cout << "BaseX foo()\n";
	}

	virtual void vfunc()
	{
		std::cout << "BaseX vfunc()\n";
	}
};

class BaseY: public Base
{
public:
	void foo()
	{
		std::cout << "BaseY foo()\n";
	}

	virtual void vfunc()
	{
		std::cout << "BaseY vfunc()\n";
	}
};

class Der : public BaseX, public BaseY
{
public:

	void vfunc() override
	{
		std::cout << "Der vfunc()\n";
	}
};

int main()
{
	Der myder;

	myder.BaseX::foo();
	myder.BaseY::foo();

	Der::BaseX* bpx = &myder;
	Der::BaseY* bpy = &myder;

	bpx->vfunc();
	bpy->vfunc();
}
```

<ins>Output</ins>  
BaseX foo()  
BaseY foo()  
Der vfunc()  
Der vfunc()  

If the function overloading mechanism is desired to be on action, then "using declaration" can be added for both base classes in the derived class:
```cpp
class BaseX
{
public:
	void foo()
	{
		std::cout << "BaseX foo()\n";
	}
};

class BaseY
{
public:
	int foo(int, int)
	{
		std::cout << "BaseY foo(int, int)\n";
		return {};
	}
};

class Der : public BaseX, public BaseY
{
public:
	using BaseX::foo;
	using BaseY::foo;
};

int main()
{
	Der myder;

	myder.foo();
	myder.foo(5, 6);
}
```
## Diamond Formation Problem (Dreadful Diamond of Derivation)

In a diamond inheritance hierarchy, virtual inheritance is used to ensure that only one instance of the common base class subobject exists in the most derived object. The common base class is called a _virtual base class_.
A virtual base class is initialized by the _most derived class_. Any attempts to initialize the virtual base class in intermediate classes are ignored; only the initializer provided by the most derived class is used.

There is a tool to solve the problem of the base class object coming to life twice (ambiguity) when an object of the derived class type is defined in the hierarchy in the form of diamond. Syntax:

class Person{};  
class Employee:  virtual public Person{};  
class Engineer:  virtual public Person{};  
class Developer: public Employee, public Engineer{};  

If the class that is derived by virtual inheritance (Employee and Engineer) will be a base class together with another class in multiple inheritance, the virtual inheritance syntax will ensure that only one of thie common base classes object is included in the derived class. In this syntax, both base classes (Employee and Engineer) must be obtained from virtual inheritance, otherwise the virtual inheritance mechanism will not be activated. Here, the common base class (Person) is called as virtual base class. Virtual base class must be initialized by the ctors of all of its derived classes.
```cpp
class Person // virtual base class
{
public:
	Person(const char* arg)
	{
		std::cout << arg << '\n';
	}
};

class Employee : virtual public Person
{
public:
	Employee() : Person("Employee") {}
};

class Engineer : virtual public Person
{
public:
	Engineer() : Person("Engineer") {}
};

class Developer : public Employee, public Engineer
{
public:
	Developer() : Person("Developer") {}
};

class Senior_Developer : public Developer
{
public:
	Senior_Developer() : Person("Senior_Developer") {}
};

int main()
{
	Person per{"Person"};
	Engineer eng;
	Employee emp;
	Developer dev;
	Senior_Developer Sdev;
}
```
<ins>Output</ins>  
Person  
Engineer  
Employee  
Developer  
Senior_Developer  

## Empty Base Optimization (EBO)

An empty class still occupies at least one byte of storage, in order to ensure that distinct objects have distinct addresses. If such a class is used as a data member, 
the compiler may need to add padding for alignment, which can increase the object size(e.g. it may occupy 4 bytes instead of 1).

For this reason, inheriting from an empty class is often preferred, because most modern compilers apply the _Empty Base Optimization (EBO)_ and do not allocate additional
storage for the empty base subobject. Note that EBO is not strictly guaranteed by the standard, but it is widely implemented by major compilers.

Why is an empty class formed in the first place?

- Just because a class does not have members does not mean that it will not have an interface.
- Such classes are used to create overload resolution with some techniques.
- Empty classes are used as stacks in the generic programming paradigm.

There is another interesting scenario of private inheritance employed by Herb Sutter. This scenario is also called "restricted polymorphism".
It is used to ensure that only certain functions benefit from the virtual dispatch mechanism of the virtual function in the base class.
```cpp
class Base
{
public:
	virtual void vfunc();
};

class Der: private Base
{
public:
	void vfunc() override;
	friend void g1();
};

void foo(Base& r)
{
	r.vfunc();
}

void g1()
{
	Der1 myder;
	foo1(myder); // invalid, friend func
}

void g2()
{
	Der1 myder;
	foo1(myder); // valid
}
```
