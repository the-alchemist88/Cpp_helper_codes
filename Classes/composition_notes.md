# Composition(Containment)

Composition means that a class has data members of another class type. It differs from nested types. Nested type means that the definition of a class is contained within another class definition. 
Terms used for denoting classes which are in compostion relationship are - Member and Owner. Basic features to know about composition:

1) Member's interface is not directly attached to the Owner's interface, but Member's functions can be called inside Owner's functions.
```
class Member
{
public:
	void foo() { std::cout << "Member foo()\n"; };
	Member() = default;
	Member(int, int, int) {};
private:
	void bar() {};
};

class Owner
{
public:	
	void foo() { std::cout << "Owner foo()\n"; };		
	void fun()
	{
		mx.foo();
		//my.bar(); 	// syntax error, private
	}

private:
	Member mx;
	Member my{ 3, 5, 7 }; 	// Reminder: Default in-class initializers for class members require "{}" or "="  based initializer syntax. There's no "()" based syntax for such initializers.
	void baz() { std::cout << "Owner baz()\n"; };
};

int main()
{
	Owner mo;
	mo.foo();
	mo.fun();
}
```
<ins>Output</ins>  
Owner foo()
Member foo()

2) Owner cannot access private members of Member. However access can be allowed via a friend declaration.

3) The special member functions that the compiler writes for Owner are passed directly to Member. In other words, whatever is called for Owner also called for Member.
(Rule of zero)

<ins>Owner</ins>			<ins>Member</ins>  
------------				----------
Default ctor	------->		Default ctor  
Copy ctor     	------->		Copy ctor  
Move ctor     	------->		Default ctor  
Copy assign.    ------->		Copy assign.  
Move assign.    ------->		Move assign.  
Dtor		------->		Dtor  

A possible example of how compiler generated special member functions look like:
```
class A {}; // member 1
class B {}; // member 2
class C {}; // member 3

class Myclass
{
public:
	Myclass() : ax(), bx(), cx() {};
	Myclass(const Myclass& rhs) : ax(rhs.ax), bx(rhs.bx), cx(rhs.cx) {};
	Myclass(Myclass&& rhs) : ax(std::move(rhs.ax)), bx(std::move(rhs.bx)), cx(std::move(rhs.cx)) {};

	Myclass& operator=(const Myclass& rhs)
	{
		ax = rhs.ax;
		bx = rhs.bx;
		cx = rhs.cx;
		return *this;
	}

	Myclass& operator=(Myclass&& rhs)
	{
		ax = std::move(rhs.ax);
		bx = std::move(rhs.bx);
		cx = std::move(rhs.cx);
		return *this;
	}

private:
	A ax;
	B bx;
	C cx;
};
```
4) If the programmer writes the Owner's copy ctor and move ctor, s/he must also write the code that decides which ctor of the Member to call.
The same also applies to assignment operator functions.
```
class Member
{
public:
	Member() { cout << "Member()\n"; }
	Member(const Member& rhs) { cout << "Member(const Member& rhs)\n"; }
	Member(Member&&) { cout << "Member(Member&&)\n"; }
};

class Owner
{
public:
	Owner() = default;
	Owner(const Owner& rhs) : ival(rhs.ival) /*, mx(rhs.mx)*/  {} // Owner copy ctor calls Member default ctor - not usually desired, uncommenting between /*..*/ will do proper coding
	Owner(Owner&& rhs) : ival(std::move(ival)) /*, mx(std::move(mx))*/ {} // Owner move ctor calls Member default ctor - not usually desired, uncommenting between /*..*/ will do proper coding
	Owner& operator=(const Owner& rhs)
	{
		ival = rhs.ival;
		//mx = rhs.mx; // programmer should add this line and also copy ctor of Member class, if s/he intents to write copy ctor of Owner
		return *this;
	}

private:
	int ival;
	Member mx;
};

int main()
{
	Owner o1{};
	Owner o2 = o1;
	Owner o3 = std::move(o1);
}
```
**Output**  
Member()
Member()
Member()
