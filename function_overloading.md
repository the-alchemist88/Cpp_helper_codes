# Function Overloading

The main purpose of function overloading mechanism is naming a group of functions, which do the same job in essence when they are abstracted, with the same name. Thus, user of this functions 
will employ them more conveniently. If those functions had to have the same name as in C language, user would have to know all of the different function names that actually correspond to only one functionality. Consider this example in C language:
```cpp
int   abs(int n);
long  labs(long n);
float fabs(float n);
```
Please note that implementing function overloading at compiler side is a significant burden, on the other hand C language is designed to be rather minimalistic and general, consequently it doesn't have function overloading feature. 

Requirements for function overloading to occur:

1) Namesake functions, which are called candidate functions technically, must be declared in the same scope(In case of ADL and operator functions, scope and name lookup rules might differ).
2) Their signatures must be different.

The primary job of function overloading mechanism to pick the correct function when a candiadate function is called. This process is called function overload resolution and it is a compile-time feature.

## Terminology reminders

- function overloading is related to compile time polymorphism
- function overriding is related to inheritance and run-time polymorphism

When a function is called, deciding which function will bind to this call can be at:  

1) compile-time(static binding/early binding)  
2) runtime(dynamic binding/late binding)  

Scope categories:

| C++ scope categories        | C scope categories |
| :-------------------        | :------------------|
| namespace scope  	          | file scope |
| block scope 		            | block scope |
| class scope   		          |       -       |
| function prototype scope    | function prototype scope |
| function scope    	        | function scope  |

Function signature: The part of the function declaration without return type.  

## Function overload resolution

Recall that if candidate functions' return type and signature are the same, this is called function redeclaration. However if the signatures are the same but only return types are different then compiler will complain. Ex:
```cpp
int func1(int a, int b);
int func1(int x, int y); 		    // function redeclaration, x and y are in function prototype scope, those names are unrelated to function overloading 

void func2(int x); 		
void func2(const int x); 		    // function redeclaration, top-level constness doesn't change signature

int func3(int* ptr); 		 
int func3(const int* ptr); 		  // function overloading, low-level constness

int func4(int* ptr);			
int func4(int* const ptr);		  // function redeclaration

int func5(int, int);	
double func5(int, int);			    // error, only return types are different, signatures are the same

double func6(int*);
double func6(int[]);			      // function redeclaration, array decay

double func7(int(*)(int, int));	
double func7(int(int, int));	  // function redeclaration, function to pointer conversion

double func8(int);
double func8(int&);				      // function overloading

int func9(char);
int func9(signed char);			    // function overloading, char type is implementation-defined, it can be unsigned or signed depending on the compiler. Nevertheless these are three distinct types.
int func9(unsigned char);		    // function overloading

int func10(int&);				
int func10(int&&);				      // function overloading
int func10(const int&);			    // function overloading
int func10(const int&&);		    // function overloading

int func11(int*);				
int func11(std::nullptr_t);		  // function overloading, nullptr_t is the type of nullptr
```

A call to overloaded functions can result in three ways: no match, ambiguity or best match. Overload resolution contains several phases:

1) Candidate functions are detected.
2) Viable functions, which are simply suitable functions by signature, are determined. If there is no viable function that means no match.
3) If there are more than one viable function, the result, ambiguity or best match, will be decided by criteria explained below.

Overload resolution ranks the viable candidate functions by comparing how each argument of the call matches the corresponding parameter of the candidates. 

If we are to specify how well a given argument matches the corresponding parameter of a viable candidate, we can rank the possible matches as follows (from best to worst):

a) Standard Conversion. Can be grouped in three categories:  
	- Exact match  
	- Promotion  
	- Other conversions except first two cases  
  
  Exact match subcategories:  
      1) L value ==> R value transformation  
      2) const conversion  
      3) Array to pointer conversion(array decay)  
      4) function to pointer conversion  
  
  Promotion subcategories:  
      1) Integral promotion:  
          bool, char, short  ==> int  
      2) float-double promotion:  
          float ==> double  

b) User-defined conversion parameter function  

c) Variadic parameter function  
	
For simplicity, first have a look at the one parameter examples below:
```cpp
int func1(int);
int func1(long);
int func1(float);
int func1(double);

int func2(void *);
int func2(int *);
int func2(int, int);

int func3(int);
int func3(double);

int func4(float*);
int func4(const int*);

void func5(int*);
void func5(const int*);

void func6(int);
void func6(const int&);

int foo(int);

void func7(int(*)(int));

int func8(int, int = 10);		
int func8(int);

void func9(int);
void func9(long));
void func9(double));

void func10(int);
void func10(long);
void func10(long double);

void func11(unsigned int);
void func11(char);

void func12(char);
void func12(float);
void func12(long double);

void func13(int*);
void func13(const char*);

void  func14(int);
void  func14(int&);

void  func15(int);
void  func15(int*);

void  func16(int&);
void  func16(int&&);
void  func16(const int&);

void  func17(bool);
void  func17(void*);

int main  {

	func1(45); 			  // exact match, int
	func1(45L); 		  // exact match, long
	func1(45.1f); 		// exact match, float
	func1(45.); 		  // exact match, double
 
	func2(45); 			  // no match
	
	int x{};
	func3(x); 			  // exact match, int, L value to R value transformation
	
	func4(&x); 			  // exact match, const int*, const conversion
	
	int a[5]{};	
	const int cx{};	
	func5(a);			    // exact match, int*, array to pointer conversion
	func5(&x);			  // exact match, int*
	func5(&cx);			  // exact match, const int*

	func6(54);			  // ambiguity, an R value expression can be bound to both "const T&" and "T" paramter
	func6(x);			    // ambiguity
	func6(cx);			  // ambiguity
	
	func7(foo);			  // exact match, int(*)(int), function to pointer conversion
	func7(&foo);		  // exact match, same call without conversion
 
	func8(43); 			  // ambiguity, default parameter has no effect on ranking
	
	func9('A');			  // promotion, int
	func9(2.3f);		  // promotion, double
	
	func10(2.3f);		  // ambiguity, each overload requires conversion

	func11(x);			  // ambiguity, both overload requires conversion
	
	func12(5.6);		  // ambiguity, each of overload requires conversion
	
	func13(&cx);		  // no match, there is no conversion from "const T*" to "T*"
	
	func14(45);			  // exact match, int, argument is R value expression, L value reference cannot be bound to R value expression
	func14(x);			  // ambiguity, there is no superiority between T and T& parameter
	
	func15(0);			  // exact match, int, both functions are viable but second one requires a conversion(integral to null pointer conversion)
	func15(nullptr);	// conversion, from nullptr_t to int*
	
	func16(x);			  // int&
	func16(cx);			  // const int&
	func16(23);			  // int&&, "const T&" can also bind to an R value expression but "T&&" has priority against it  
	
	func17(&x);			  // conversion, void*, this case is exception, otherwise expectation would be ambiguity

}
```

Regarding multiple paramater candidate functions, for one candidate to be considered better than another, this single rule explains it all:  

The better candidate must have at least one parameter be a better match and cannot have other parameters be a worse match(can be the same level) than the corresponding parameter in the other candidates. Ex:  

```cpp
void foo(int, long, double); 	  // 1
void foo(bool, float, int);		  // 2
void foo(long, double, float);	// 3

int main  {  

	foo(12, 12, 45u); 		  // 1, for first argument 1st foo is better match, for the rest of argument all has same level(conversion)
	foo(12, 12f, 45u);		  // ambiguity, for first argument 1st foo is better match, for second paramter 2nd foo
	foo(50u, 3.6L, true); 	// 2, for first two arguments ther is no winner, for third argument 3rd foo has promotion
	foo(50L, 3.6L, true); 	// ambiguity, for first argument 3rd foo has advantage and third 2nd foo
}
```
