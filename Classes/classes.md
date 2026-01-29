# Classes

To simply put, classes are the tools to abstract the entities in problem or solution domain. It helps to express complex entities as simpler representations in software. 
Classes are primarily associated with object-oriented programming, but in multiparadigm languages like C++, they can also be used to support generic and other programming styles.
OOP is a programming approach; a programmer can decide to use this approach or another, and a programming language can have tools to support that approach or not. 
For example C does not provide built-in support for inheritance or runtime polymorphism, but similar behavior can be emulated using structs and function pointers.

Members of a class can be:

1) Data members(fundamental types, class types, enum types etc. )
2) Member functions
3) Nested types(type members  or member types) denote types declared or defined inside a class such as nested classes, type aliases, enums, etc.

```cpp
class A {
public:
    class B {};          // nested class
    struct C {};         // nested struct
    enum D { X, Y };     // nested enum
    using E = int;       // type alias
    typedef double F;    // typedef
};
```
 
Notes on features of classes:

- Member function syntax exists in language level. Conceptually, non-static member functions can be viewed as free functions with an implicit object parameter, although at the ABI and code generation level there may be differences.  
They can be conceptually treated as free functions with an implicit object parameter(typically a pointer to the object) which corresponds to the type of the object on the left-hand side of the dot operator.

```cpp
class Enemyfighter {};

class Myfighter { 
public:
	void attack1(Enemyfighter*); 		// actually has a hidden parameter of type Myfighter*
};
	
void attack2(Myfighter*, Enemyfighter*);

int main()
{
	Myfighter fighter1;
	Enemyfighter fighter2;
	fighter1.attack1(&fighter2); // If we assume that attack1 and attack2 have the same effect, it is much more clear on attack1 that which object is taking the action
	attack2(&fighter1, &fighter2);
}
 ```
- Access specifiers don't form a scope. Recall that compiler checks code in this order: Name lookup, context control (overload resolution), access control.
 
- Function redeclaration with the same signature is not allowed inside class scope, but overloading is permitted.
 
- Public interface of a class ==> public members of that class + associated free functions that operate on the class type (typically both are located in the same header file)
```cpp
//Myclass.h

 class Myclass {  //  public interface of Myclass ==> members + foo
 public:
 // members
 }
 
 void foo(Myclass); 
 ```
 - Member functions are implicitly inline when they are defined inside class, thus they don't violate ODR even if the class is placed in header file and this header file included by multiple source files. Ex:  
 ```cpp
//Myclass.h
 
#pragma once
 
class Myclass { 
public:
	void set(int x){ // this is implicitly inline 
		mx = x;
	}
	
	int get();
private: 
	int mx;
};

int Myclass::get() { // this will cause a linker error if it is included in multiple source files. It should qualified as inline in declaration or definition or both.
	return mx;
}
```
- Member functions have access to private members of any object of type of that class, not only of the object the member function is called for. Ex:
 ```cpp
class Myclass {
public:
  void set(int);

private:
  int mx;
};

Myclass g;

void Myclass::set(int x) { // valid
  g.mx = mx;	// left hand side mx is private data member of g object
  mx = x;		  // left hand side mx is private data member of the object that set function is called for
}
 ```
- Regarding name lookup, an unqualified name inside a member function first looked for in block scope then searched in class scope.
 ```cpp
class Myclass {
public:
	void foo();
	
private: 
	int x;
}

int x;

Myclass g;

void Myclass::foo() {
	int x;
	x = 45;					// local
	Myclass::x = 56;		// data member
	::x = 56;				// global
	
	x = Myclass::x + ::x; 	// valid
};

void foo();

int main() {
	Myclass m;
	m.Myclass::foo();		// member function call
	foo();					// global function call
}
 ```

- When defining a member function outside class scope, names that belong to the class don't have to be qualified inside class scope.

 ```cpp
class Myclass {
public:
  static int x;
  class Nested {};
  void bar(void)
  {
    mn;           //  1  All of them refers the same object. Note that mn is non-static data member
    this->mn;     //  2
    Myclass::mn;  //  3
  }
private:
  Nested foo(Nested);
  Nested mn;

};

Myclass::Nested Myclass::foo(Nested) // return type needs to be qualified by class name unlike parameter and the names inside function block 
{
  mn;            // 1  All of them refers the same object
  this->mn;      // 2
  Myclass::mn;   // 3
  x = x + 5;
}

Myclass::Nested foo(Myclass::Nested) {}   // this is a free function
 ```

- For non-static data members, initialization is performed via the constructor initializer list(CIL) or default member initializers. In CIL, initialization order follows the same order of declaration. 
Thus, at CIL, making the order of initalization different than declaration order of variables may cause confusion and result in indeterminate values. Ex:
 ```cpp
class Myclass {
public:
	Myclass() : mb(10), ma(mb / 3) // see order of declaration of data members. ma is initalized with an indeterminate value since mb had a indeterminate value during initialization of ma
	{
		mc = 6; // this is not an initialization but an assignment, mc has already been initialized at CIL
	}

	void print() const
	{
		std::cout << "ma = " << ma << "mb = " << mb << '\n';
	}

private:
	int ma, mb, mc;
	int md{7};	// in-class member initializer(default member initializer), it simply instructs the compiler to initialize this variable with the specified value at CIL
				// if a data member will not be initiazlied with a parameter(via parameter-ed ctors), this this way of initialization can be preferred.
	int me(8);	// error, direct initialization syntax is not allowed inside class.
};
 ```
- "Special member functions" are unique in the way that compiler, instead of the programmer, can generate the code for them under some conditions. When compiler writes the code for special member functions, we use the term "default" in order to express it. For example: Compiler defaulted the default constructor of Myclass.

Special member functions are:

1. Default ctor  
2. Destructor  
3. Copy ctor  
4. Move ctor (C++11)  
5. Copy assignment  
6. Move assignment (C++11)

- When a special member function of a class is implicitly declared - defaulted by the compiler; if the generated definition is ill-formed then the compiler will delete that special function that it should generate.
 ```cpp
class Member {
public:
  Member(int){}
};

class Demo {
  Member mx;
};

int main()
{
  Demo d; // error, the default ctor of Demo is deleted
}
 ```
## const member functions

- const member functions have a hidden _const_ _class_type*_ paramater so that they cannot modify class data members. Note that const member functions cannot call non-const member functions. Ex:
 ```cpp
class Myclass {
public:
	void foo()
	{
		bar(); // ok, conversion from T* to const T*
	}

	void bar() const
	{
		x = 54; 	// error
		foo();		// error, bar() passes hidden const Myclass* argument to foo(), but there is no conversion from const T* to T*.
		Myclass m;
		m.foo();	// valid, this is called for m object via hidden Myclass*
		baz();		// valid
	}

	Myclass& baz() const
	{
		return *this;	// error, no conversion from const T* to T*, return type should be const Myclass&
	}

	Myclass baz() 		// function overloading
	{
		return *this;
	}

private:
	int x;
};

int main() {

	Myclass m;
	const Myclass cm;

	m.bar();	// valid
	cm.bar();	// valid

	cm.foo(); // error, no conversion from const T* to T*
}
```
- mutable keyword is used for data members that do not participate in the logical state of the object and mutable data members can be mdofied by const member functions as well non-const member functions.  
Note that this context of mutable keyword has no relation with the usage of it in lambda expressions.  
 ```cpp
class Fighter {
public:
	void foo() const
	{
		++call_count; // valid
	}
private:
	int m_attack;
	int m_defense;
	mutable int call_count; // in problem domain this is unrelated with the state of the object
}
 ```
