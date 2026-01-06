# Temporary Objects

Temporary object: Even in the source code there is no named object of this type, but in the code that the compiler generates(in assembly code) there is an object constructed.  

There are two ways to construct a temporary object:  

1) Via the code that stimulate the compiler to form one:
```cpp
const int& r1 = 45;
// Although 45 is a prvalue, a reference must bind to an object.
// Therefore, a temporary int object initialized with 45 is materialized,
// and its lifetime is extended to match the lifetime of r1. Look item a below

int&& r2 = 67;
// Even though y is an rvalue reference and and integer literal "67" is a prvalue expression,
// reference requires an object to bind to.
// Therefore, a temporary int object initialized with 67 is materialized,
// and its lifetime is extended to match the lifetime of r1. Look item a below
```

2) Directly using temporary object syntax:
```cpp
int();
int{};
int(45);
int(14);

Myclass(); // look below item b 
Myclass{}; // look below item b 
```

a) By C++17 standard, prvalue expressions no longer directly form temporary objects. However, when used where a glvalue expression is expected, a temporary object is generated.
In this case, the prvalue expression is converted to an xvalue expression, and an object is formed; this is called _temporary_ _materialization_. 

A prvalue expression of class type is not an object itself. By a common analogy, it's more like "the recipe used to form a class object." However, a class object is formed when there is a "temporary materialization conversion." In other words, temporary materialization results in a conversion from the prvalue category to the xvalue category.

b) If you don't make a statement from an prvalue expression of class type and don't use it, in other words dicard it, here temporary materialization kicks in and the constructor is called to form the object.

Example for first method:
```cpp
class Myclass
{
public:
	Myclass()
	{
		std::cout << "Myclass() called for this: " << this << "\n";
	}

	Myclass(int x)
	{
		std::cout << "Myclass(int x) called for this: " << this << "(x = " << x << ")\n";
	}

	~Myclass()
	{
		std::cout << "~Myclass() called for this: " << this << "\n";
	}
};

int main()
{

	Myclass mx;
	mx = 10;
}
```

Output:
 ```text
Myclass() called for this: 0000006F371DFCA4 
Myclass(int x) called for this: 0000006F371DFD84(x = 10)  
~Myclass() called for this: 0000006F371DFD84  
~Myclass() called for this: 0000006F371DFCA4  
```
In the output, two objects of Myclass type are constructed even if in the source code there is only one. Because in the assignment statement a temporary object of type Myclass is
constructed with Myclass(int x ) constructor and just after the end of this statement it's destroyed.

Value category of expressions that generate a temporary object is R value(PR value-->Rvalue).

When a reference (const L value reference or R values reference) is bound to a temporary object (in a PR value expression category) the life of the temporary object is fixed to scope
of the referring name. That is, the temporary object is not destroyed until the end of the scope of the referring name. This rule is called life extension. The first example was a
typical case of life extension:
```cpp
const int& x = 45;
```
Another example for life extension:

```cpp
class Myclass
{
public:
	Myclass()
	{
		std::cout << "Myclass() called for this: " << this << "\n";
	}

	~Myclass()
	{
		std::cout << "~Myclass() called for this: " << this << "\n";
	}
};

int main()
{
	Myclass&& r = Myclass{};

	std::cout << "main function continues\n";
}
```
Output:
```text
Myclass() called for this: 0000009AEF8FFAA4  
main function continues  
~Myclass() called for this: 0000009AEF8FFAA4  
```
The life of a temporary object ends just after running the code of statement where it was constructed(if there is no life extension).

## Move-ing Operations

```cpp
void func(std::string&& r) // r value reference parameter function, this functions main purpose is moving from r. Usually this is declared with an lvalue overload.
{
  std::string s1 = r; // doesn't move, value category of expression 'r' is an lvalue --> copy ctor of string class is called
                        // data type of r is std::string&&

  std::cout << "r.length(): " << r.length() << '\n';
  std::cout << "s1.length(): " << s1.length() << '\n';

  std::cout << "\nNow moved\n\n";
  std::string s2 = std::move(s); // does move, expression 'std::move(s)' is xvalue --> move ctor of string class is called

  std::cout << "r.length(): " << r.length() << '\n';
  std::cout << "s2.length(): " << s2.length() << '\n';
}

int main()
{
	std::string s(10'000, 'A');

	std::string&& r = std::move(s); // doesn't move, just binds r vlaue reference to x value of s

	std::cout << "r.length(): " << r.length() << '\n';
	std::cout << "s.length(): " << s.length() << '\n';

	std::cout << "\nfunc gets called\n\n";
	func(std::move(s));
}
```
Output:
```text
r.length(): 10000  
s.length(): 10000  

func gets called  

r.length(): 10000  
s1.length(): 10000  

Now moved  

r.length(): 0  
s2.length(): 10000  
```
Moved-from state: Value is unknown. However, object must be in a valid state: Invariants are not corrupted after move operation. This means:

1) Reassignable
2) Calling the destructor of shouldn't cause any issues. Its resources are already stolen so its dtor shouldn't try to release its resource.

Even if the standard library doesn't reassure about the value of a moved-from object, it practically makes the object as if it is default initialized.
