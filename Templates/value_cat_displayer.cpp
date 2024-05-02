template<typename T>
struct ValCat {
	static constexpr const char* p = "PR value";
};

template <typename T>
struct ValCat <T&> {
	static constexpr const char* p = "L value";
};

template <typename T>
struct ValCat <T&&> {
	static constexpr const char* p = "X value";
};

#include <iostream>

#define  print_val_cat(expr)   std::cout << "value category of expr '" #expr "' is : " << ValCat<decltype((expr))>::p << '\n'

#define  test(expr)   std::cout << "my expr is \""#expr"\" " << '\n'

class Myclass {};

Myclass foo();
Myclass& bar();
Myclass&& baz();

int main()
{

	Myclass x{};
	const Myclass y{};

	print_val_cat(x);
	print_val_cat(y);
	print_val_cat(foo());
	print_val_cat(bar());
	print_val_cat(baz());
	print_val_cat(std::move(x));
	print_val_cat(std::move(y));

	print_val_cat("Hello world!");
	print_val_cat(void());
	print_val_cat(nullptr);

	Myclass arr[10]{};
	auto* ptr{arr};
	print_val_cat(*ptr);
	print_val_cat(++ptr);
	print_val_cat(ptr++);
	print_val_cat(ptr + 5);
	print_val_cat(ptr[0]);

	print_val_cat(static_cast<Myclass&&>(x));
}

/*
Output
------
value category of expr 'x' is : L value
value category of expr 'y' is : L value
value category of expr 'foo()' is : PR value
value category of expr 'bar()' is : L value
value category of expr 'baz()' is : X value
value category of expr 'std::move(x)' is : X value
value category of expr 'std::move(y)' is : X value
value category of expr '"Hello world!"' is : L value
value category of expr 'void()' is : PR value
value category of expr 'nullptr' is : PR value
value category of expr '*ptr' is : L value
value category of expr '++ptr' is : L value
value category of expr 'ptr++' is : PR value
value category of expr 'ptr + 5' is : PR value
value category of expr 'ptr[0]' is : L value
value category of expr 'static_cast<Myclass&&>(x)' is : X value
*/

