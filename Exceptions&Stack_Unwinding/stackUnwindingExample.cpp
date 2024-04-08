/*
Stack unwinding example
-------------------------

The following example demonstrates how the stack is unwound when an exception is thrown. Execution on the thread jumps from the throw statement in C to
the catch statement in main, and unwinds each function along the way. Notice the order in which the Dummy objects are created and then destroyed as they
go out of scope. Also notice that no function completes except main, which contains the catch statement. Function A never returns from its call to B(),
and B never returns from its call to C(). If you uncomment the definition of the Dummy pointer and the corresponding delete statement, and then run the
program, notice that the pointer is never deleted. This shows what can happen when functions do not provide an exception guarantee. If you comment out
the catch statement, you can observe what happens when a program terminates because of an unhandled exception.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <memory>

using namespace std;

class MyException
{
public:
    MyException() = default;
    MyException(const MyException&)
    {
        cout << "MyException copy ctor throws exception\n";
        //throw MyException{}; // If an exception occurs after the exception is thrown but before the unwind begins, std::terminate is called.
    }
};

class Dummy
{
public:
    Dummy(string s) : MyName(s) { PrintMsg("Def. constructed Dummy "); }
    Dummy(const Dummy& other) : MyName(other.MyName) { PrintMsg("Copy constructed Dummy "); }
    ~Dummy() { PrintMsg("Destroyed Dummy "); }
    void PrintMsg(string s) { cout << s << MyName << " at the adress of: " << this << "\n"; }
    void SetName(string s) { MyName = s; }
private:
    string MyName;
};

void C(Dummy d)
{
    cout << "Entering Function C" << "\n";
    d.SetName("C");
    auto Dunique = make_unique<Dummy>("D"); // Exception safe
    throw MyException();

    cout << "Exiting Function C" << "\n";
}

void B(Dummy d)
{
    cout << "Entering Function B" << "\n";
    d.SetName("B");
    C(d);
    cout << "Exiting Function B" << "\n";
}

void A(Dummy d)
{
    cout << "Entering Function A" << "\n";
    d.SetName("A");
    //  Dummy* pd = new Dummy("new Dummy"); //Not exception safe!!!
    B(d);
    //   delete pd;
    cout << "Exiting Function A" << "\n";
}

void MyTerminate()
{
    cout << "MyTerminate() terminates the program\n";
    exit(EXIT_FAILURE);
}

int main()
{
    set_terminate(MyTerminate); // If a matching handler is not found, or if an exception occurs during the unwinding process but before the handler 
                                // gets control, the predefined run-time function terminate is called. 

    cout << "Entering main" << "\n";
    try
    {
        Dummy d("M");
        A(d);
    }
    catch (MyException& e)
    {
        cout << "Caught an exception of type: " << typeid(e).name() << "\n";
    }

    //catch (MyException e)  // This catch statement takes the exception object by copy
    //{                      // therefore, exception object's copy ctor is called before stack unwinding starts(refer to item 4 in exceptions&stack_unwinding.txt.txt)
    //    cout << "Caught an exception of type: " << typeid(e).name() << "\n";
    //}

    cout << "Exiting main" << "\n";
}
