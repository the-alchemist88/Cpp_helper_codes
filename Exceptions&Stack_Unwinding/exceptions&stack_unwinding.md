# Exceptions and Stack Unwinding in C++

When an exception is thrown, the runtime searches for a matching catch handler. During this search, the stack is unwound: all automatic objects that were fully constructed between
the throw point and the handler are destroyed. Destructors are called in the reverse order of construction. When using exception handling tools, execution proceeds as follows:

- Control reaches the try block by normal sequential execution. The guarded section in the try block is executed.

- If no exception is thrown during execution of the guarded section, the catch clauses that follow the try block are not executed. Execution continues at the statement after the last
catch clause that follows the associated try block.

- If an exception is thrown during execution of the guarded section or in any routine that the guarded section calls either directly or indirectly, an exception object is created 
from the operand of the throw expression. (This implies that a copy constructor may be involved.) At this point, the compiler looks for a catch clause in a higher 
execution  context that can handle an exception of the type that is thrown, or for a catch handler that can handle any type of exception. The catch handlers are examined in order of
their  appearance after the try block. If no appropriate handler is found, the next dynamically enclosing try block is examined. This process continues until the outermost enclosing 
try block is examined.

- If a matching handler is still not found, or if an exception occurs during the unwinding process but before the handler gets control, std::terminate is called. If an exception occurs
after the exception is thrown but before the unwind begins, std::terminate is called.

- If a matching catch handler is found, and it catches by value, its formal parameter is initialized by copying the exception object. After the formal parameter is initialized, the process
of unwinding the stack begins. This involves the destruction of all automatic  objects that were fully constructed—but not yet destructed—between the beginning of the try block that is
associated with the catch handler and the throw site of the exception. Destruction occurs in reverse order of construction. The catch handler is executed and the program resumes execution
after the last handler—that is, at the first statement or construct that is not a catch handler. Control can only enter a catch handler through a thrown exception, never through a goto
statement or a case label in a switch statement.

## Exceptions in Standard Library

Most standard library exceptions derive from std::exception. The two main semantic categories are:  

- logic_error - when invariants are violated(your program has a bug)

- runtime_error – failures caused by external conditions (e.g., network failure, file system errors)

And several commonly encountered exceptions are:

- bad_alloc - memory allocation failed

- bad_cast - dynamic cast failed

- ios_base::failure - iostreams operation failed


## What to throw?

In practice, exceptions are thrown by copy semantics: compiler uses the argument of throw, as initilizer expression for its own object of same type.
It is common to throw objects derived from std::exception, such as std::runtime_error, since they provide a standard interface (what()). Since they are typically caught by reference,
this avoids unnecessary copying and prevents object slicing when dealing with polymorphic exception types. _std::exception_ is not magic, it is a simple class.

You can derive from std::exception, logic_error or runtime_error or write your own class. What is important to understand basic principle:

- Exception classes are semantic tags, that you can use to differentiate failure causes:

	- If you don't need special handling for some error - use std::runtime_error.

	- If you don't need to handle two failure cases differently - don't create different exceptions.

## Notes:

- The function std::terminate is called in several situations, including:

	- when an exception is not caught

	- when an exception is thrown during stack unwinding

	- when a noexcept function throws an exception

	- when a destructor throws during stack unwinding

	- when an exception occurs while copying the exception object

- By default, std::terminate calls std::abort through the currently installed terminate handler. Terminate handler can be set by _std::set_terminate(...)_.

- Stack unwinding begins when an exception is thrown and continues while the runtime searches for a matching handler. If no suitable handler is found, std::terminate is eventually called.

- The parameter of catch block(exception object) is initialized by throw statement. Thus, copy elision might kick in during this process. However this parameter is typically 
_<const exception&>_ since copy ctor of exception class can also throw exception that leads to calling std::terminate.

- Destructors are implicitly noexcept(true) unless explicitly specified otherwise. If a destructor throws an exception during stack unwinding, std::terminate is called. Move constructors
and move assignment operators are typically declared noexcept, since this allows the standard library and the compiler to perform additional optimizations.

- Constructors may throw exceptions since it is possible to fail when obtaining resources. Note that an object comes to life only if one of its ctors is fully executed. Therefore, its destructor
is not called. Only the destructors of fully constructed objects are executed during stack unwinding. Thus smart pointers is critical when obtaining resources in ctors so that they can
be released during stack unwinding. Ex:

```cpp
class Myclass
{
public:
    Myclass(int n) : mp{ new int[n]} // memory allocated
    {
        //code
        throw std::runtime_error{ "runtime_error!" }; // but this object doesn't come to life
    }

    ~Myclass() // not called because the constructor did not complete successfully
    {
        std::cout << "dtor called\n";
        delete[]mp;
    }

private:
    int* mp;
};


int main()
{
    try
    {
        Myclass m{ 56 };
    }
    catch(const exception& ex)
    {
        std::cout << "exception caught: " << ex.what();
    }
}
```

<ins>Output</ins>   
exception caught: runtime_error!

- If ctor of a dynamically allocated object throws exception, memory that _operator new_ allocates, is deallocated by the compiler at runtime.
 The new expression consists of two steps:

	1) operator new allocates memory
	2) the constructor initializes the object

```cpp
class Myclass
{
	Myclass()
	{
		// code
		throw std::exception;
	}
}

int main()
{
	Myclass* p = new Myclass; 
}
```

## Function try block

A function try block allows exceptions thrown during the execution of the entire function (including member initializer lists in constructors) to be caught. Syntax:

```cpp
int func(int x) try
{
	// throws an exception
	return 1;
}
catch 
{
	auto y = x;
	return 2;
}
```

It is especially useful in constructors to catch exceptions thrown by base classes or member objects during initialization. If a constructor function try block catches an
exception thrown during member initialization, the object cannot be fully constructed. Therefore, the compiler implicitly rethrows the exception after the catch block.

```cpp
class Member
{
public:
    Member(int)
    {
        throw std::runtime_error{ "runtime error!" };
    }
};

class Myclass
{
public:
    Myclass(int x) try : mx{x} // first member type ctor executed
    { 
    }
    catch (const std::exception& ex) // if ctor of a member type throws an exception and gets caught..
    {
        std::cout << "exception caught(constructor): " << ex.what() << '\n';                          
    }

private:
    Member mx;
};

int main()
{
    try
	{
        Myclass{45};
	}
    catch (const std::exception& ex) // rethrow gets caught
	{
		std::cout << "exception caught(main): " << ex.what() << '\n';
	}
}
```

<ins>Output</ins>  
exception caught(constructor): runtime error!  
exception caught(main): runtime error!

## Key Takeaways

- Exceptions propagate until a matching handler is found.
- Stack unwinding destroys automatic objects in reverse order of construction.
- Destructors should not throw exceptions.
- Exceptions should usually be thrown by value and caught by const reference.
- RAII should be used to manage resources safely during exception handling.

 
Resource utilised: https://learn.microsoft.com/en-us/cpp/cpp/exceptions-and-stack-unwinding-in-cpp?view=msvc-170  
Article Date: 11/14/2022
