# NAMESPACES

- Namespaces are employed to form scopes. In order to use an identifier in a namespace, that identifier must be qualified(with scope resolution operator) with the name of that namespace.
- Namespaces are typically seen in header files, as in STL. For example **std** namespace is placed in many header files.
- A namespace that has been located in global namespace can plainly be called as **namespace** in C++ jargon. 
- A namespace cannot appear inside a function.
- Elements of namespaces have static storage such as global variables and initialized when namespace is formed.
- Namespaces are cumulative, which means that contents of namespaces that has been named with the same name can be added to each other, as applied in STL.
- Namespaces can be nested in each other.
  
  Two functions with the same name can be used as in the example:

```cpp
// demo.h
#pragma once

namespace Demo
{
	bool foo(int); // This is a function name in Demo namespace
}

bool foo(int); // This is a free function 

// demo.cpp
#include "demo.h"

bool Demo::foo(int) 
{
	return false;
}

bool foo(int)
{
	return true;
}
```
Bir isimalanı içindeki isim, o isimalanı dışında, isimalanı adı ile nitelenmeden kullanılablir mi? Evet. Bunun üç yolu var:

1) using declaration
2) using namespace (directive) declaration
3) ADL(argument dependant look-up)

   1) using std::cout;

Bu bir bildirimdir. Bu bildirimin bir kapsamı var ve bildirim o kapsamda etkin. Using bildirimi, bildirime konu ismi, bildirimin yapıldığı kapsama 
enjekte(inject) eder. Yani o isim sanki o kapsamda bildirilmiş gibi ele alınır.

   2) using namespace std;

using namespace direktifi, isimalanı içindeki isimleri görünür kılar. Ancak using bildirimi gibi isimleri enjekte etmez. Aynı isimde başka bir varlık
bildirilirse sentaks hatası olmaz ancak ismin kullanılma anında nitelenmezse hata olur.
```cpp
namespace Demo
{
	int x;
}

int x;
//using Demo::x; // error, x declared before
using namespace Demo;

int main()
{
	//x = 10; // error, ambiguity
	::x = 10;
	Demo::x = 15;
}
```
   3) ADL(Argument dependant look-up)
   
Bir fonksiyon çağrısı nitelenmiş bir isim kullanılarak yapıldığında, eğer fonksiyona gönderilen argüman ya da argümanlardan biri bir isimalanı içinde 
tanımlanan bir türe ilişkin ise(isimalanı içinde tanımlanan bir türden olmak zorunda değil) söz konusu fonksiyon ismi, o isimalanında da(argümanın 
ilişkin olduğu) aranır.

Örnek:
```cpp
namespace Demo
{
	class Myclass {};
	void foo(Myclass)
	{
		std::cout << "Demo::foo(Myclass)\n";
	}
	
	void bar(std::vector<Myclass>)
	{
		std::cout << "Demo::bar(vector<Myclass>)\n";
	}
	
	void baz(Myclass)
	{
		std::cout << "Demo::baz(Myclass)\n";
	}
	
	void fun(Myclass)
	{
		std::cout << "Demo::baz(Myclass)\n";
	}
}

void baz(Demo::Myclass)
{
	std::cout << "baz(Myclass)\n";
}

int main()
{
	Demo::Myclass mx;
	std::vector<Demo::Myclass> vx;
	void fun(int);

	foo(mx); // ADL
	bar(vx); // ADL
	baz(mx); // error - ambiguity, ADL vs free function, none has priority on name look-up
	fun(mx); // error - calls fun(int) in local scope, it masks the name "fun" on name look-up 
}
```
Herb Sutter'ın bloğundan alınan başka bir örnek:
```cpp
namespace A
{
	struct X{};
	struct Y{};
	void f(int);
	void g(X);
}

namespace B
{
	void f(int i)
	{
		f(i); // calls B::f (endless recursion)
	}

	void g(A::X x)
	{
		g(x); // Error ambiguous between  B::g(ordinary lookup) and A::g(argument-dependent lookup) 
	}

	void h(A::Y y)
	{
		h(y); // calls B::h (endless recursion): ADL examines A namespace and finds no A::h, so only  B::h from ordinary lookup is used
	}
}
```

İsimalanlarının belirgin özellikleri:

-İsimsiz isimalanı(unnamed or anonymous) isimalanı static anahtar sözcüğünü kullanmadan iç bağlantı(internal linkage) oluşturmak için kullanılır. Bunlar
başlık dosyalarında değil cpp dosyalarında kullnmak içindir. static anahtar sözcüğüne göre avantajları:

1) static anahtar sözcüğünün birçok farklı bağlamda kullanılması farklı işlevlerinin olması(overloaded - aşırı yüklenmesi)

2) static anahtar sözcüğü class'ları niteleyemez.

- Ayrıca isimsiz isimalanları nested isimalanları için e kullanılabilir. İsimsiz isimalanı örneği:
```cpp
namespace
{
	int g;
	int foo(){};
}

int main()
{
	foo(12);
	g = 15;
}
```
- Yukarıdaki isimsiz isimalanı örneğiyle aynı etkiye sahip isimlendirilmiş isimalanı kullanımı örneği:
```cpp
namespace xyz15
{
	int x;
	int foo(){};
}

using namespace xyz15;

int main()
{
	foo(12);
	g = 15;
}
```
-namespace alias sentaksının kullanılışı:
```cpp
namespace Demonstaration_xyz_proj
{
	namespace Details
	{
		int ival;
	}
}

void foo()
{
	namespace Demo = Demonstaration_xyz_proj::Details;// kapsamı var
	Demo::ival++;
}

- Modern C++ ile artık,

namespace A
{
	namespace B
	{
		namespace C
		{

		}
	}
}
```
ifadesi aşağıdaki ifadeyle denk:
```cpp
namespace A::B::C
{
}
```
- inline namespaces. Buradaki inline anahtar sözcüğünün inline fonksiyonlar veya inline değişkenlerle ilgisi yoktur. inline anahrar sözcüğünün isimalalanlarında
kullanımı, kendi elemanlarını onu kapsayan isimalanı içinde görünür kılar. Çoğu durumda inline namespaceaşağıdaki örnekteki gibi bir bildirimle aynı 
etkiyi taşır.
```cpp
namespace A
{
	namespace B
	{
		int ival{5};
	}

	using namespace B; // Bu noktadan itibaren ival A isimalanındaymış bigibi kullanılabilir
}

namespace X
{
	inline namespace Y
	{
		int ival{ 5 };
	}
}

int main()
{
	A::ival = 10;
	X::ival = 10;
}
```
Farklılıklarını bulunduğu noktalar da vardır:
```cpp
namespace Myspace1
{
	int a{5};
	namespace Nested1
	{
		class A{};
		int a{ 10 };
	}
	using namespace Nested1;
	void fun(Nested1::A) {};
}

namespace Myspace2
{
	int b{ 15 };
	inline namespace Nested2
	{
		class B{};
		int b{ 20 };
	}

	void bar(Nested2::B) {};
}

int main()
{
	Myspace1::Nested1::A x;
	Myspace2::Nested2::B y;
	
	fun(x); // error, ADL doesn't kick in
	bar(y); // ok, ADL kicks in
	
	std::cout << Myspace1::a; // ok
	std::cout << Myspace2::b; // error, ambiguity, inline namespace is used
}
```
