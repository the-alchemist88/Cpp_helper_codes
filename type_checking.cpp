#include <iostream>
#include <type_traits>

using namespace std;

/*Checking types in various ways in compile-time*/

//Explicit(Full) Specialization
template<typename T>
void int_check(T) = delete;

template<>
void int_check(int) { cout << "int_check\n"; }

//Three different usage of enable_if(in decision of function return type, function parameter type, template parameter type)
template<typename T>
enable_if_t<is_pointer_v<T>> pointer_check(T) { cout << "pointer_check\n"; }

template<typename T>
void double_check(T x, enable_if_t<is_same_v<T, double>, double> = 0.)
{
	cout << "double_check\n";
}

template<typename T, typename U = enable_if_t<is_class_v<T>>>
void class_check(T)
{
	cout << "class_check\n";
}

//static_assert
template<typename T>
void array_check(T)
{
	static_assert(is_array_v<T>, "Only for arrays!");
	cout << "array_check\n";
}

struct Mystruct{};

int main()
{
	Mystruct m;
	int ival{};
	double dval{};
	const char arr[10]{};

	/*Test*/

	int_check(ival);
	//int_check(dval);
	double_check(dval);
	//double_check(m);
	pointer_check(&m);
	//pointer_check(ival);
	class_check(m);
	//class_check(arr);
	array_check<const char[]>(arr);
	//array_check<double[]>(arr);
}