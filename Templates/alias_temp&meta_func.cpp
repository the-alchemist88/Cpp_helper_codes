#include <iostream>

using namespace std;

//Example of alias templates used in type_traits.h
template<typename T>
class Myclass
{
public:
    using type = T*;
    constexpr static T value{};
};

template<typename T>
using Myclass_t = typename Myclass<T>::type;

template<typename T>
using Myclass_v = typename Myclass<T>::value;

/****************************************************************************************/

//Possible implementation of integral_constant meta function in type_traits.h
template<typename T, T v>
struct IntegralConstant
{
    constexpr static T value = v;
    using value_type = T;
    using type = IntegralConstant<T,v>;
};

using TrueType = IntegralConstant<bool, true>;
using FalseType = IntegralConstant<bool, false>;

/****************************************************************************************/

//Possible implementation of is_pointer meta function in type_traits.h
template<typename T>
struct IsPointer : FalseType {};

template<typename T>
struct IsPointer<T*> : TrueType {};

template<typename T>
void func(T) {
    static_assert(IsPointer<T>::value, "Only for pointer types!");
}


int main()
{
    cout << boolalpha << "Myclass<int>::type and Myclass_t<int> are the same type? : " 
        << is_same<Myclass<int>::type, Myclass_t<int>>::value << '\n';

    bool b1 = IsPointer<int>::value;
    bool b2 = IsPointer<int*>::value;

    cout << "b1 = " << b1 << " b2 = " << b2 << '\n';

    add_pointer<decltype(b1)>::type bptr = &b1;
    func(bptr);
    //func(b2); // fails at static_assert for pointer type
}
