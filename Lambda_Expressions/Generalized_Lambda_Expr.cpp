#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// a simple example of compiler generated type(closure type) when auto keyword is used in lambda expression as parameter type
class Compiler_Generated // exemplar of object f in main
{
public:
    template<typename T>
    auto operator()(T x) const
    {
        cout << x << ' ';
    }
};

template<typename ...Args>
void print(Args&& ...args)
{
    initializer_list<int>{((cout << forward<Args>(args) << '\n'), 0 )...};
}

void func(int i) { cout << "i"; }
void func(double d) { cout << "d"; }
void func(long l) { cout << "l"; }

int main()
{
    vector ivec{ 1,3,5,7,9 };

    //for_each(ivec.begin(), ivec.end(), func); // cannot deduce type of container elements at compile time, and since func has overloads compiler throws error

    auto f = [](auto x) { cout << x << ' '; }; // a trivial generalized lambda expression

    for_each(ivec.begin(), ivec.end(), f); // ok, now x's type deduced as int since ivec is vector<int>
    cout << '\n';
    for_each(ivec.begin(), ivec.end(), [](auto x) {func(x); }); // directly use lambda expression inside algorithm function
    cout << '\n';
    for_each(ivec.begin(), ivec.end(), [](double x) {func(x); });
    cout << '\n';
    for_each(ivec.begin(), ivec.end(), [](long x) {func(x); });
    cout << '\n';

    auto f1 = [](auto&&... param) { print(forward<decltype(param)>(param)...); }; // generalized lambda expression can also be reference, variadic temp etc.

    f1(12, 3.4, 'c', "Generalized Lambda Expression");

    cout << '\n';

    // simple lambda init capture examples

    int x{ 10 };
    auto f2 = [x = x + 5](int a) { return x * a; }; // f2 class has a data member x and initilized using the value of local variable x
    cout << f2(5) << '\n';

    auto f3 = [&x = x](int a) { return x = a; }; // data member x of f3 class is a reference that is bound to local x variable
    cout << f3(35) << '\n';

    // actual usage of lambda init captures stands out in move inits

    unique_ptr<string> uptr{ new string {"init capture example"} };
    cout << (uptr ? "not empty" : "empty") << '\n';
    auto f4 = [uptr = move(uptr)]() { return *uptr; };
    cout << f4() << '\n';
    cout << (uptr ? "not empty" : "empty") << '\n';

}
