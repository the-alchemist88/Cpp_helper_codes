# Generic Programming and Templates

Briefly speaking, generic programming denotes the style of programming that is independent of types. For example in C language "void*" type is used for eliminating dependency to types.
However it is called type erasure and caunted as unsafe. Besides in C++ and in some other languages inheritence tools(virtual dispatch etc.) is used to employ derived class' codes without knowing the type at runtime.
Templates in C++ on the other hand, enable type-safe generic programming, unlike void* in C. 

The most prominant difference between C and C++ compiler is that C++ compiler can generate code at compile time via templates. Consequently, we call such code, which can trigger compiler to write code by its
instructions as templates. This style of coding is called Template Meta Programming(TMP). The types of templates are:

- function template
- class template
- alias template(C++11)
- variable template(C++14)
  
Concepts are not templates themselves, but a way to constrain templates.
- concepts(C++20)


Template parameters can be:

- type parameter   		--> template<typename T>
- non-type parameter 	--> template<int n>
- template parameter 	--> parameter itself is a template (rare usage)

Syntax: 
```cpp
template<comma-separated-list-of-parameters>
```
In order for the compiler to write the code of template, it needs to know the types of arguments that corresponds to template parameters. There are three ways of doing this for compiler:

1) Explicit template argument --> `vector<int> x{1, 2, 3}`
2) Deduction:
   - Template Argument Deduction - TAD (It was applicable only for function templates until C++17)
   - Class Template Argument Deduction - CTAD (C++17)
4) Default template argument --> `template<typename T = int>`

## Function Templates

Function templates are functions that are parameterized to represent a family of functions. The keyword **typename** introduces a type parameter. The process of replacing template parameters by concrete types is called instantiation. It results in a _specialization_(instance) of a template. 

## Translation of Templates

Templates are _compiled_ in two phases, this mechanism is called as two-phase lookup:

1)
- Without instantiation at definition time, the template code itself is checked for correctness ignoring the template parameters. This includes:
	- Syntax errors are discovered, such as missing semicolons.
	- Using unknown names (type names, function names, …) that don’t depend on template parameters are discovered.
	- Static assertions that don’t depend on template parameters are checked.

- If there are multiple template parameters, ambiguity conditions are checked.

2)
- At instantiation time, the template code is checked (again) to ensure that all code is valid. That is, now especially, all parts that depend on template parameters are
double-checked.

Ex:
```cpp
template<typename T>
void func(T x)
{
    foo(x);  // unknown name, possible to find x with ADL, compiles without an error
    x.bar(); // unknown name, depends on template parameter, compiles witout an error 
    //baz(); // error
}

int main()
{
}
```

## Code Generation: Templates vs. Standard Functions

As we explained above, C++ compiler acts as a "code generator" for templates. It only writes concrete function code (instantiation) when it encounters a unique set of template arguments.

- Type-Based Selection `(template<typename T>)`
  
The compiler generates one function for each unique type. If the type T is the same, the compiler reuses the existing specialization.

```cpp
template<typename T>
void f(T n) {}

f(1);   // T is int -> f(int) is generated.
f(5);   // T is int -> Reuse f(int). No new code.
f(1.0); // T is double -> f(double) is generated. (Total: 2 functions)
```

- Value-Based Selection `(template<int N>)`
  
For Non-type Template Parameters (NTTP), the value itself is part of the function's identity (the signature). Every unique value of N triggers the generation of a new,
distinct function in the binary.
```cpp
template<int N>
void g() {}

g<1>(); // N=1 -> g<1>() is generated.
g<5>(); // N=5 -> g<5>() is generated. (Total: 2 functions)
```

- Standard Functions `(void h(int n))`

Standard functions are compiled only once, regardless of how many different values are passed to them.

```cpp
void h(int n) {}

h(1); // Calls the single instance of h(int).
h(5); // Calls the same instance of h(int).
```

## Template Argument Deduction(TAD)

Auto type deduction and TAD work very similarly with one exception (std::initializer_list), _auto_ has special rule for braced-init-list.
```cpp
template<typename T>
void fun(T x) {}

int main()
{
	auto x = { 1,2,3 };   // OK
	fun({ 1,2,3 });       // error
}

```
In auto type deduction,
type deduction is made for the keyword **auto**, not for the variable name itself. Thus, reference and pointer declarators must be taken into account separately. 
In general, there are three cases:

1) `auto x = expr;`	-->	cv qualifiers and refs drop. Array and function decay occur. Equivalent function template is:
   ```
   template<typename T>
   void func(T x);
   ```

2) `auto& x  = expr;`	-->	cv qualifiers don't drop. Array and function decay don't occur. Cannot be bound to R value expression. Equivalent function template is:
   ```
   template<typename T>
   void func(T& x);
   ```

3) `auto&& x = expr;`	-->	This is a universal reference. Type of _x_ depends on the value category of expr:  
	- if lvalue then type for _auto_ keyword will be deduced as T&, thus type of _x_ will be T& according to reference collapsing rules.  
	- if rvalue(PR value or X value) then type for _auto_ keyword will be deduced as T&&, thus type of _x_ will be T&& according to reference collapsing rules.  
	Equivalent function template is:
   ```
   template<typename T>
   void func(T&& x);
   ```

Ex:
```cpp
int main()
{
    int x{};
    const int y{};

    int a[10]{};
    const int ca[10]{};

    auto var1 = x;      // type is int
    auto var2 = y;      // type is int, const-ness drops

    auto var3 = a;      // type is int*
    auto var4 = ca;     // type is const int*, this low level const, only top level const-ness drop in copy syntax

    auto& var5 = 13;    // error, cannot be bound to R value expression
    auto&& var6 = 5;    // type is int&&
    auto& var7 = var6;  // type is int&. The name that is type of R value reference(var6), forms nevertheless an L value expression

    auto&& var8 = a;  	// type is int(&)[10]
    auto&& var9 = ca;   // type is const int(&)[10]. ca is L value expression, according to reference collapsing rules var should be lvalue ref

}
```
There is a practical method to check the type of an expression in templates:
```cpp
template<typename T>
class TypeTeller;

template<typename T>
void fun(T)
{
    TypeTeller<T> x;
}

template<typename T>
void bar(T&)
{
    TypeTeller<T> x;
}

template<typename T>
void baz(T&&)
{
    TypeTeller<T> x;
}

int main()
{
    int arr[10]{};
    
    fun(arr);
    bar(arr);
    baz(arr);
}
```
In the example above, since _TypeTeller_ is a forward declaration(incomplete class), compiler will complain. However, in the error message it also adds the type information of template
parameter T, for each function call. Ex:
```cpp
#include <initializer_list>

template<typename T>
void func1(T)
{
}

template<typename T>
void func2(std::initializer_list<T>)
{
}

auto createInitList()
{
    return { 1, 3, 5, 7 }; // error, can't deduce type, auto in a function return type or a lambda parameter implies template type deduction, not auto type deduction
}

int main()
{
    auto initList = { 1, 3, 5, 7 }; // ok, type of initList is std::initializer_list<int>
    func1({ 1, 3, 5, 7 }); 			// error, can't deduce type for T
    func1(initList);  				// ok, type of T is std::initializer_list<int>
    func2({ 1, 3, 5, 7 }); 			// ok, T deduced as int
}
```
So the only real difference between auto and template type deduction is that auto assumes that a braced initializer represents a std::initializer_list, but template type deduction
does not.

Examples of template parameter type mismatch situations:
```cpp
template<typename T>
void func1(T, T)
{

}

template<typename T>
void func2(T, T&&)
{

}

int main()
{
    func1(1, 5u); // error, T can be int or unsigned int
	int x{};
	
	func2(x,x);   // error, T can be int or int&
}
```

## Abbreviated Template Syntax

- There is a shorter sytax for writing function templates:

```cpp
void fun(auto x, auto y)
{
}
```

This is equivalent to:

```cpp
template<typename T, typename U>
void bar(T x, U y)
{
}
```

## Trailing return type (C++11)

Trailing return type syntax is typically used in function templates. Basic example to understand syntax:

Example:
```cpp
auto baz(int) -> int
{
}
```

is equivalent to:

```cpp
int baz(int)
{
}
```
This syntax is particularly useful in scenarios where it enhances the readability of complex return types.


```cpp
int bar(int);

int (*foo()) (int) // return type is int(*)(int). Functions cannot return function type, but pointer to function type
{
	return bar;
}
```

is equivalent to:

```cpp
auto baz() -> int(*)(int)
{
	return bar;
}
```

Another example:

```cpp
int arr[15];

int (&fun())[15]
{
	return a;
}
```

is equivalent to:

```cpp
auto func() -> int[15]
{
	return arr;
}
```

In practice, this syntax is most commonly used when the return type depends on template parameters.
```cpp
template<typename T, typename U>
auto sum(T x, U y) -> decltype(x + y) // since decltype(x + y) cannot be put in place of "auto" keyword as return type, it is placed after "->" token
{
	return x + y;
}
```

## Auto return type (C++14)

In this syntax the compiler deduces the return type by inspecting the return statement(s) within the function body. This is known as Return Type Deduction.

```cpp
auto sum(int x, int y)
{
    return x + y;
}

```
- If a function has multiple return statements, they must all deduce to the same type; otherwise, it’s a compilation error.

- The function definition (the body) must be visible to the compiler at the point of call for deduction to occur.

- For recursive functions, at least one return statement must precede the recursive call to allow the compiler to deduce the type first.


Resources utilized:
1) C++Templates[Vandevoorde-Josuttis]
2) Effective_Modern_C++[Scott_Meyers]
