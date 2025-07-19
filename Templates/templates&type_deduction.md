# Generic Programming and Templates
----------------------------------

## Templates - 1
-------------

Briefly speaking, generic programming denotes the style of programming that is independent of types. For example in C language void* type is used for eliminating dependency to types.
Besides in C++ and in some other languages inheritence tools(virtual dispatch etc.) is used to employ derived class' codes without knowing the type at runtime.

The most prominant diffrence between C and C++ compiler is that C++ compiler is able to write code. Consequently, we call the code that can make compiler write code according its
instructions as templates. This style of coding is called Template Meta Programming(TMP). The types of templates are:

- function template
- class template
- alias template(C++11)
- variable template(C++14)
- concepts(C++20)


Template paramters can be:

- type paramater   		--> template<typename T>
- non-type paramater 	--> template<int n>
- template paramater 	--> parameter itself is a template (rare usage)

Syntax: 

template< comma-separated-list-of-parameters >

In order for the compiler to write the code of template, it needs to know the types of arguments that corresponds to template parameters. There are three ways of doing this for compiler:

1) Explicit template argument --> vector<int> x{1, 2, 3}
2) Deduction: Template Argument Deduction - TAD (It was applicable only for function templates until C++17)
   Class Template Argument Deduction - CTAD (C++17)
3) Default template argument --> template<typename T = int>

# Function Templates
------------------

Function templates are functions that are parameterized so that they represent a family of functions.

The keyword typename introduces a type parameter.

The process of replacing template parameters by concrete types is called instantiation. It results in a specalization(instance) of a template.

# Translation of Templates
------------------------
Templates are "compiled" in several phases:

1. Without instantiation at definition time, the template code itself is checked for correctness ignoring the template parameters. This includes:

– Syntax errors are discovered, such as missing semicolons.
– Using unknown names (type names, function names, …) that don’t depend on template parameters are discovered.
– Static assertions that don’t depend on template parameters are checked.

2. If there are multiple template parameters, ambguity conditions are checked.

3. At instantiation time, the template code is checked (again) to ensure that all code is valid. That is, now especially, all parts that depend on template parameters are
double-checked.

Ex:
```cpp
template<typename T>
void func(T x)
{
    foo(x);  // unknown name, depends on template parameter, compiles witout an error 
    x.bar(); // unknown name, depends on template parameter, compiles witout an error 
    //baz(); // error
}

int main()
{

}
```
## Template Argument Deduction(TAD)
-------------------------------
Auto type deduction and TAD is the same with one exception (std::initializer_list)

In auto type deduction, type deduction is made for the keyword auto not for variable name.

Type deduction is made for auto 

1) ```cpp auto x = expr;``` 		// cv qualifiers and refs drop. Array and function decay occur.

```cpptemplate<typename T>```    // equivalent function template 
```cppvoid func(T x);```

2) auto& x  = expr;		// cv qualifiers don't drop. Array and function decay don't occur. Cannot be bound to R value expression

template<typename T>	// equivalent function template 
void func(T& x);

3) auto&& x = expr;		// this is a universal reference. Type of x depends on the value category of expr:
				// if L value then type of x dedeuced as T& according to reference collapsing rules
				// if R value(PR value or X value) then type of x dedeuced as T&& according to reference collapsing rules
						
template<typename T>	// equivalent function template 
void func(T x);

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
    auto var5 = ca;     // type is const const int*

    auto& var6 = 5;     // error, cannot be bound to R value expression
    auto&& var7 = 5;    // type is const const int&&
    auto& var8 = var7;  // type is const const int&. The name that is type of R value reference(var7), forms nevertheless an L value expression

    auto&& var9 = ca;   // type is const int(&)[10]. ca is L value expression, according to reference collapsing rules var sloud be lvalue ref
    auto&& var10 = 10;  // type is int&&
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
In the example above Since TypeTeller is a forward declaration(incomplete class), compiler will complain. However, in the error message it also adds the type information of template
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
    return { 1, 3, 5, 7 }; // error can't deduce type, auto in a function return type or a lambda parameter implies template type deduction, not auto type deduction
}

int main()
{
    auto initList = { 1, 3, 5, 7 }; // ok, type of initList is std::initializer_list<int>
    func1({ 1, 3, 5, 7 }); // error, can't deduce type for T
    func1(initList); // // ok, type of T is std::initializer_list<int>
    func2({ 1, 3, 5, 7 }); // ok, T deduced as int
}
```
So the only real difference between auto and template type deduction is that auto assumes that a braced initializer represents a std::initializer_list, but template type deduction
doesn’t.

Examples of template paramter type mismatch situations:
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

Resources Utilised:

1) C++Templates[Vandevoorde-Josuttis]

2) Effective_Modern_C++[Scott_Meyers]
