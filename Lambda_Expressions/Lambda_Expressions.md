# Lambda Expressions

- General syntax of a lambda expression:
```cpp
[capture](params) specifiers {
    body;
	return expr;
};
```

- Each lambda has a unique, unnamed type. Even identical-looking lambdas produce different types.
```cpp
auto a = [](){}; // types of a and b are different
auto b = [](){}; // decltype(a) != decltype(b)
```
The type is called closure type. The instance is closure object.

- The parameter of the function call operator that the compiler writes for this class is the parameter specified between the "()" parentheses in the 
lambda expression, the "{}" brackets in the lambda expression will be the same as in the function block.

- Function call operator of compiler generated type for lambda expression has auto return type, which means return type will be deduced from return
 expression. The corresponding _operator()_ function can be as below:
```cpp
auto operator()(int x) const{return x}; // return type is int
```

- Local variables with automatic lifetime cannot be accessed in a lambda expression without capture, but local variables with static lifetime and global variables
can be used. However, if "captured", a local variable can be used in a lambda expression. This requires for the compiler to add a data element of this type 
to the class and initialize the value with the value of the captured local variable. "Capturing" is done via placing the local variable inside lambda
introducer "[]".

- The variables inside the lambda capture can be thought as non-static data members of compiler generated type:
```cpp
int capture{};

auto comp_gen = [capture](int x){return capture * x;};
```
How compiler translates it:
```cpp
class Compiler_Generated
{
public:
	Compiler_Generated(int _capture): capture(_capture) {}

    auto operator()(int x) const
    {
        return capture * x;
    }
private:
	int capture;
};
```

- Function call operator of compiler generated type for lambda expression is a const member function, which means it cannot change the copy captured object. 
However, if the object is captured by reference then it can change because function call opereator actually changes the referenced to object, not
closure type's data member:
```cpp
int x{ 10 };

auto comp_gen = [&x](){return ++x;};

// can be thought as:

class Ref
{
public:
	Ref(int& val): r(val){} 		// now r refers to object that is sent to this ctor
	auto operator()() const { return ++r; } // reference-ness of "r" didn't get modified - it cannot refer to another object, can be thought as constant pointer(int* const r = &val) 
						// "++r" expression doesn't cause incompatability with const member function
private:
	int& r;
};

auto demo = Ref(x)();

cout << "demo = " << demo << '\n'; // demo and x is 11
cout << "x = " << x << '\n';
```

- If _mutable_ keyword is placed in lambda expression then it rules out the _const_ qualifier in function call operator:
```cpp
[x]()mutable{ ++x ;} // valid
```
It modifies the captured copy, not the original object.

- _Capture all_ syntax:

	- [=]  capture all by value
	- [&]  capture all by reference

_Capture all_ does not mean all variables in scope. It means - “Capture all variables that are actually used inside the lambda”

```cpp
int x = 10, y = 20, z = 30;

auto f = [=]() {
    return x + y;  // z is not captured
};
```

- Mix capture syntax 1:

	[=, &x] 

  	- Default: by value
  	- Exception: x is by reference

Ex:
```cpp
int x = 10, y = 20;

auto f = [=, &x]() {
	int x = 10, y = 20;

	auto f = [=, &x]() mutable {
		x++;           // modifies original x
		y++;           // modifies copied y
	};

	cout << "Before lambda is called\n";
	cout << "x = " << x << '\n';
	cout << "y = " << y << '\n';

	f();

	cout << "After lambda is called\n";
	cout << "x = " << x << '\n';
	cout << "y = " << y << '\n';
```

<ins>Output</ins>  
Before lambda is called  
x = 10  
y = 20  
After lambda is called  
x = 11  
y = 20  

- Mix capture syntax 2:

	[&, x]

	- Default: by reference
	- Exception: x is by value

Ex:
```cpp
int x = 10, y = 20;

auto f = [&, x]() {
    // x is a copy
    // y is a reference
};
```
✔️ y can be modified
❌ x cannot (it's a copy)

- Init capture syntax(since C++14):
```cpp
auto f = [x = 10]() { return x; };

auto f = [ptr = std::move(p)]() {}; // move example
```
- Generic lambda syntax(since C++14):

It is actually a templated _operator()_. Ex:
```cpp
auto f = [](auto x) { return x + x; };
```
This is the mental model for generic lambda:
```cpp
class Gen_Lambda {
public:
    template<typename T>
    auto operator()(T x) const {
        return x + x;
    }
};
```
- Lambda is constexpr only if its body satisfies constexpr _constexpr_ requirements, it is a non-constant
expression(for example static variables cannot be used to initialize constexpr variables):
```cpp
constexpr int ci = []{static int x = 10; return x * x;}(); // not allowed in constexpr
```
- In order to check whether a lamba expression returns a constant expression, use constexpr keyword inside lambda after paranthesis:

constexpr int ci = []()constexpr{int x = 10; return x * x;}();

- If any of the qualifiers below is used in lambda expression then paranthesis must be placed even if inside is empty:
```cpp
[]()mutable{code};
[]()noexcept{code};
[]()constexpr{code};
[]()->int{code};
```
- Until C++17 lambda default ctor and copy assignment operator are deleted. Since C++20 lambdas can be default-constructed and support copy assignment
if they have no state(statless - doesn't capture any object).
```cpp
auto f = [](int x){ return x;};
decltype(f) g; // valid since C++20
```
- There is an implicit conversion from closure type(the type of lambda expression) to the function that the function call operator provides, if the 
lambda is stateless:
```cpp
int (*fp)(int) = [](int a){return  a * a;};
fp(20); // output 400
```
This resembles a class with a conversion operator to a function pointer:
```cpp
using fptr = int(*)(int);

class Compiler_Generated
{
public:
	int operator ()(int a) const {return a*a};
	operator fptr() const;
}
```
- "positive lambda idiom" simply converts a closure type into a function pointer type(lambda has to be stateless again). Turning back to
previous Compiler_Generated class:
```cpp
Compiler_Generated m;

+m; // same as calling "+m.operator fptr()", now m is a function pointer type

+[](int a) {return a*a;}; // now closure type is a function pointer type 
```
- Immediately Invoked Function Expression(IIFE) is a lambda expression that is mostly used in initializing constant variables, when the initilaizing 
function will be called only one time for example, in order to prevent scope leakage for one-time used functions and be clear about purpose.
```cpp
int x = 5;
const int y = [x](int a) {return x*a;}(10);
```
