# Copy Elision

There are some cases where compiler doesn't generate code for intentional copying. To explain briefly, even if the syntax visually suggests a copy/move operation (e.g. copy initialization), no copy/move is performed. This deliberate miss of copy/move operations by compiler is called _copy elision_. 

Until C++17, "copy elison" was a compiler optimization technique where unnecessary copy/move ctor calls are skipped. Since C++17 standard, in certain cases, copy elison became mandatory(guaranteed) and called as _mandatory copy elision_. Even though this name implies an elision, there is no elision of any copying here since omitting it became mandatory by the standard. Thus, the term "mandatory copy elision" is referred as misnomer by C++ programmers.  Ex:

```cpp
class Myclass
{
public:
	Myclass()
	{
		std::cout << "Myclass()\n";
	}

	explicit Myclass(int x)
	{
		std::cout << "Myclass(int x)\n";
	}

	Myclass(Myclass&& other)
	{
		std::cout << "Myclass(Myclass&& other)";
	}

	Myclass(const Myclass& other) 
	{
		std::cout << "Myclass(const Myclass& other)\n";
	}
};

void func(Myclass mx){}

int main()
{
	func(Myclass{10}); 	// Straightforward thinking, first a temporary object should be constructed with Myclass(int x) ctor.
						// Then since it is an r value, move ctor gets called to construct the function parameter mx. However this is atypical copy elision case.
}
```
Without any copy elision this code would give the following output(can be tested with "-fno-elide-constructors" option in GCC):
```cpp
Myclass(int x)
Myclass(Myclass&& other)
```
But instead the output is(since C++17 compiled):
```cpp
Myclass(int x)
```
## Copy Elision Cases:

### Case 1: Passing PR value expression to a function 

If a function has a paramater of any class type and this function is called via a PR value expression such as a temporary object expression, since C++17, compiler applies mandatory
copy elision. It takes "Myclass{10}" argument as an initalizer expression for "Myclass mx" object when calling "func(Myclass{10})". In other words, temporary object doesn't get
materialized.

### Case 2: Return Value Optimization(RVO)

- Unnamed Return Value Optimization(URVO)

In the initialization of an object, when the source object is a nameless temporary and is of the same class type (ignoring cv-qualification) as the target object.
When the nameless temporary is the operand of a return statement, this variant of copy elision is known as URVO(In C++17 and later, URVO is mandatory and no longer considered a form of copy elision)

- Named Return Value Optimization(NRVO)

In a return statement, when the operand is the name of a non-volatile object with automatic storage duration, which isn't a function parameter or a catch
clause parameter, and which is of the same class type (ignoring cv-qualification) as the function return type. This variant of copy elision is known as NRVO. Ex:
```cpp
class Myclass
{
public:
    Myclass() { std::cout << "Myclass()\n"; };
    Myclass(const Myclass&) { std::cout << "Myclass(const Myclass&)\n"; };
    //Myclass(const Myclass&) = delete; // uncommenting this line will cause compiler to throw error because of f_NRVO funciton
};

Myclass f_URVO() // URVO - unnamed return value optimization
				 // In C++17 and later, URVO is mandatory and no longer considered a form of copy elision
{
    return Myclass{};
}

Myclass f_NRVO() // NRVO - named return value optimization
{				 // NRVO is not mandatory, also a callable copy/move ctor is required even if f_NRVO doesn't call it
    Myclass m;
    return m;
}

int main()
{
    auto obj1 = f_URVO();  
    auto obj2 = f_NRVO();
}
```
<ins>Output:</ins>  
Myclass()  
Myclass()  

Without any copy elision in this code, 2 copy/move ctors should be called for each object initialization. First is for constructing the temporary objects(via return expression)
that will initialize obj1 and obj2 where the functions are called, second is for constructing of obj1 and obj2. Since function call expressions are PR value expressions, move ctor 
is first choice for obj1 and obj2.

<ins>Output with -fno-elide-constructors option in GCC:</ins>  
Myclass()  
Myclass(Myclass&& other)  
Myclass(Myclass&& other)  
Myclass()  
Myclass(Myclass&& other)  
Myclass(Myclass&& other)  

Note that although Myclass has user defined copy ctor, compiler calls move ctor for f_NRVO() even it has copy syntax in return statement, in other words it returns an L value
expression and return type is Myclass thus it should use copy initialization, but instead move ctor kicks in. It is not new rule, since modern C++ this case is applied as shown.
However, some programmers that are unaware of this rule,  may want to reassure calling move ctor by using std::move(name) in return statement. This usage is called pessimistic move
and must be avoided since it can block NRVO. Pessimistic move ex:

```cpp
Myclass pess_foo()        // blocks NRVO, don't use pessimistic move 
{                               
    Myclass x;
    return std::move(x);  // programmer ties to trigger move semantics by changing the value category of a named object
						  // however "return x" already triggers move ctor not copy ctor
}
```

## Copy ctor vs Move ctor vs Copy elision

If we compare copy ctor, move ctor and copy elision in terms of efficiency;

1) move ctor and copy ctor have the same cost if an object doesn't have any element to do deep copy. Example:
```cpp
class Myclass
{
	int arr[100];
	double x,y,z;
}
```
For the class above; compiler generated, implicitly declared defaulted move ctor and copy ctor won't have any difference.
```cpp
class Myclass
{
	int arr[100];
	double x,y,z;
	string str;
}
```
However for this class above; moving and copying will differ because of string element. Move ctor will change the pointers inside of string object but copy ctor will do deep copy.

2) Nevertheless copy elision is the most favourable among three in terms of efficiency, because it doesn't construct any object. It uses the same object - zero cost.

In conclusion, temporary objects should be encouraged to be used whenever they meet the requirements instead of named objects because they make copy elision possible(in arguments
of functions and return expressions). This is a considerable efficiency advantage.
