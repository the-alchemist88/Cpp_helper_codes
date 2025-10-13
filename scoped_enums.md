# Scoped Enums

Brief reminder about syntax;

Qaulified name syntax:  

	x.y;  
	x->y;  
	x::y;  
	::y  
	
Unqualified name:  

	y

## Conventional Enum Problems(Unscoped Enums - Traditional Enums - Plain Enums)

1) There is implicit conversion from enum type to int type
2) Underlying type of enum type depends on the compiler
3) Scope of the enum constant is the same as enum type. Ex:

```cpp
//#include "screen.h"
enum ScreenColor{Blue, Black, Red, Purple, Brown};

//#include "traffic.h"
enum TrafficLight{Green, Yellow, Red};  // error, redefinition of Red
```

## Scoped enums(C+11)

1) Syntax is as follows:
```cpp
enum class Color{Yellow,  Blue, Black, Purple}; // class keyword here doesn't denote a class type, this is called scoped enum according to standards
```

2) Possibility to decide underlying type:
```cpp
enum class Color: unsigned char{Yellow,  Blue, Black, Purple}; // the default underlying type is int but now it is unsigned char. This syntax also added for unscoped enums by C++11
enum TrafficLight: unsigned int{Green, Yellow, Red}; // valid since C++11
```
Therefore forward declaration of enums is valid:
```cpp
enum Color unsigned int;

class Myclass
{
	Color mclor;
}
```

5) Scope of the enum constants are inside the enum type itself. Ex:
```cpp
//#include "screen.h"
enum class ScreenColor{Blue, Black, Red, Purple, Brown}; // Now enumarator Red doesn't conflict with constant TrafficLight Red

//#include "traffic.h"
enum class TrafficLight{Green, Yellow, Red};

int main()
{
	auto mycolor = ScreenColor::Red;
	auto mylight = TrafficLight::Red;
}
```
4) By C++20 standard "using enum declaration" syntax added to the language. Ex:
```cpp
namespace myspace
{
    namespace details
    {
        enum class ScreenColor {Blue, Black, Red, Purple, Brown};
		enum class TrafficLight{Green, Yellow, Red};

    }
}

int main()
{
	using enum myspace::details::ScreenColor; // now the names inside this scope are injected here
	
	auto x = Blue;
	
	using myspace::details::TrafficLight::Red; // error, collision with myspace::details::ScreenColor::Red
	using myspace::details::TrafficLight::Yellow; // OK
	
	auto y = Yellow;
}
```
4) No implicit conversion to from enum type to int type. Ex:
```cpp
enum class ScreenColor{Blue, Black, Red, Purple, Brown};

int main()
{
	ScreenColor mycolor1{Blue}; // error, qualified name is needed
	ScreenColor mycolor2{ScreenColor::Blue}; // OK
	
	int x = mycolor2; // error, no implicit conversion
	
	int y = static_cast<int>(mycolor2);
}
```
