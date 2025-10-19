# constexpr keyword

Briefly, constexpr keyword ensures that a variable can only be initalized with a constant expression, by causing a syntax error in the contrary case. It will make the variable it qualifies, _const_ and _inline_(inline variables added by C++17 standard) implicitly. Please note that there is no type such as "constexpr T", it denotes "const T".
```cpp
const int x = 5;
```
Can x be used where the code requires constant expression, for example as a case label or for indicating array size? 

In C language the answer is no. In C++: It depends whether it is initalized by a constant expression or not, if x is initialized by a constant expression, then yes.

Ex:
```cpp
int foo();

int main()
{
	const int x = 5;
	const int y = foo();
	int z = x + 4;
	
	int a[x]; // ok
	int b[y]; // error, y is not a constant expression
	int c[z]; // error, z is not a constant expression
}
```
## constexpr Variables

- A constexpr variable can only be initialized by a constant expressions.
```cpp
constexpr int x = 10; // x's type is const int
```
Ex:
```cpp
int main()
{
	int i{};
	const int ci{};
	constexpr int cxi = 5;

	std::cout << boolalpha << is_same_v<decltype(i), decltype(ci)> << '\n';
	std::cout << boolalpha << is_same_v<decltype(ci), decltype(cxi)> << '\n';
}
```
<ins>Output</ins>  
false  
true

- Aggregate types can also be constexpr:
```cpp
constexpr int arr[] = {1,3,5,7,9};
constexpr int x = arr[1] + arr[3] + 6;
```
- constexpr variables are implicitly const. 

Ex1:
```cpp
constexpr int x = 10;
x = 5; // error
```
Ex2:
```cpp
int g = 7;
int main()
{
	constexpr int* p1 = &g; // p1 is const

	*p1 = 10; // ok
	p1 = nullptr; // error

	constexpr const int* p2 = &g; // p2 and also the type it points to is const
	*p2 = 20; // error
}
```
### Note  
Compiler may not allocate storage for x due to optimization reasons. However in some ways programmer can force it:
```cpp
constexpr int x{7};
std::cout << &x;
```

## constexpr Functions

Every function cannot be a constexpr function, here are several restrictions about a function that is to be qualified as constexpr:

1) Cannot have static variables,
2) The type of return expression, paramaters and local variables should be literal type,
3) There are some rules about exception handling which will not be covered here. 

The main characteristic of constexpr functions: if the return type, types of passed arguments and local variables are literal type, then return value can be obtained at compile time.
Since compiler can generate return values of constexpr functions in compile time, they will require less operations during run time.
Keep in mind that constexpr functions are implicitly inline. Ex:
```cpp
constexpr int foo(int x) {
	return x *x;
}

int bar(int x) {
	return x *x;
}

int main()
{
	auto x = foo(5);			// even if most compilers calculate return value in compile time, standards don't ensure it
	auto y = bar(5);			// either variable nor function is constexpr, nevertheless compilers can make optimization and calculate return value in compile time
	constexpr auto z = foo(5);	// calculation will be done in compile time, this is assured by the standards
}
```

## Reminder: Optimizations in C++

1) Compiler optimizations. This can be divided into two categories(compilers may have different abilities and can be customized with optimization switches):  
	- Reorganizing the source code to generate a more effective assembly code without changing the observable behavior(such as inline expansion, modifying the loops, dead code elimination etc.)  
	- Compiler can write code for the programmer(templates)
	
3) Link-time optimizations(LTO)
4) Processor optimizations(branch prediction, changing the order of operations etc.)

 
  
  
