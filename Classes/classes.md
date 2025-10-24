To simply put, classes are the tools to abstract the entites in problem/solution domain. It helps to express complex entites simplier software-wise. 
Please bear in mind that classes don't have to only be related to object oriented programming(OOP) paradigm as they are also heavily used on generic and functional programming paradigms. OOP a is programming approach; a programmer can decide to use this approach or another, and a programming language can have tools to support that approach or not. 
For example a programmer can implement inheritence or runtime polymorphism in C langauge but the problem is C doesn't have the support for these features, programmer has to implement by himself.
 
Notes on features of classes:

1) Member function syntax exists in langage level, at the assembly level, there is no difference betweeen non-static member functions and free functions. Consider them as free fucntions but having a hidden pointer to class paramater, type of the object that is left operand of dot operator. 
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
	fighter1.attack1(&me); // If we assume that attack1 and attack2 have the same effect, it is much more clear on attack1 that which object is taking the action
	attack2(&fighter1, &fighter2);
}
 ```
2) Access specifiers don't form a scope. Recall that compiler checks code in this order: Name lookup, context control, access control.
 
3) Function redeclaration is not allowed inside class scope.
 
4) Public interface of a class ==> public members of that class + free functions that has paramater of type of that class (typically both are located in the same header file)
  ```cpp
//Myclass.h

 class Myclass {  //  public interface of Myclass ==> members + foo
 public:
 // members
 }
 
 void foo(Myclass); 
 
 5) Member functions are implicitly inline when they are defined inside class, thus they don't violate ODR even if the class is placed in header file and this header file included by multiple source files. Ex:
 
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

int Myclass::get { // this will cause error if it is included in multiple source files. It should qualified as inline in declaration or definition or both.
	return mx;
}
 ```
5) Member functions have access to private members of any object of type of that class, not only of the object the member function is called for. Ex:
 ```cpp
class Myclass {
public:
	void set(int);
	
private: 
	int mx;
}

Myclass g;

void Myclass::set(int x) { // valid
	g.mx = mx;	// mx is private data member of g object
	mx = x;		// mx is private data member of the object that set function is called for
}
 ```
6) Regarding name lookup, an unqualified name inside a member function first looked for in block scope then searched in class scope.
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
7) In constructor initializer list(CIL), initializatin order follows the same order of declaration. 
Thus, at CIL, making the order of initalization different than declaration order of variables may cause confusion and UB. Note that Initalization only happens at CIL and using it considered as best practice. Ex:
 ```cpp
class Myclass {
public:
	Myclass() : mb(10), ma(mb / 3) // UB, see order of declaration of data members. ma is initalized with an indeterminate value since mb had a indeterminate value during initialization of ma
	{
		mc = 6; // this is not an initialization but an assignment, mc has already been initialized at CIL
	}

	void print() const
	{
		std::cout << "ma = " << ma << "mb = " << mb << '\n';
	}

private:
	int ma, mb, mc;
	int md{7};	// this is not an initialization, it simply instructs the compiler to initialize this variable with the specified value at CIL
	int me(8);	// error, direct initialization syntax is not allowed inside class
};
 ```
8) "Special member functions" are unique in that way that compiler, instead of the programmer, can generate the code for them under some conditions. When compiler writes the code for them, we use the verb "default" in order to express it. For example: Compiler defaulted the default constructor of Myclass.

Special member functions are:

Default ctor
Destructor
Copy ctor
Move ctor (C++11)
Copy assignment
Move assignment (C++11)

## const member functions

1) const member functions have a hidden const class_type* paramater so that they cannot modify class data members. Note that non-const member functions cannot call const member functions. Ex:
 ```cpp
class Myclass {
public:
	void foo() 
	{
		bar(); // ok, conversion from T* to const T*
	}
	
	void bar() const
	{
		x = 54: 	// error
		foo();		// error, bar() passes hidden const Myclass* argument to foo(), but there is no conversion from const T* to T*.
		Myclass m;
		m.foo();	// valid, this is called for m object via hidden Myclass*
		baz();		// valid
	}
	
	Myclass* baz() const
	{
		return *this;	// error, no conversion from const T* to T*, return type should be const Myclass*
	}
	
	Myclass* baz() 		// function overloading
	{
		return *this;
	}
	
private:
	int x;
}

int main() {

	Myclass m;
	const Myclass cm;

	m.bar();	// valid
	cm.bar();	// valid

	m.foo();	// error, no conversion from const T* to T*
}
 ```
2) mutable keyword is used for data members that don't change state of a class and mutable data members can be mdofied by const member functions as well non-const member functions. Note that this context of mutable keyword has no relation with the usage of it in lambda expressions.
 ```cpp
class Fighter {
public:
	void foo() const
	{
		++call_count; // valid
	}
private;
	int m_attack;
	int m_defense;
	mutable int call_count; // in problem domain this is unrelated with the state of the object
}
 ```
