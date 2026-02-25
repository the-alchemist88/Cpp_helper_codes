# Operator Overloading

In C, a struct object can only be the operand of assignment (=), sizeof, address-of (&), member access (.), pointer-to-member (->). However, in C++, class and enum types can be the operands of many operators.
In this case, the compiler converts the class object expression, which is the operand of the operator, into a call to a function, if a suitable function has been declared.
This mechanism is called **operator overloading**. Operator overloading is simply another way of calling a function. Instead of calling a function by name, when 
a class object is made the operand of an operator, the compiler, according to the rules of the language, converts this class object expression, which has become the operand 
of the operator, into a call to a function. The main purpose of this mechanism is to benefit from the high level of association and perception that operators provide for arithmetic
and pointer types, also for class objects and consequently writing a shorter but still straightforward code.

It is generally advised to use operator overloading only when the operator's role is intiutive enough, in other words client should naturally understand what this operator does. Psuedo code ex:
```cpp
Date mydate{13, 01, 2025};
mydate++;  // intuitive: next day
string name {"Alex"};
auto newname = name + "ander"; // newname == "Alexander"
Matrix m1{2, 2};
auto result = m1 * 3; // result == {6, 6}
```

Note that operator overloading mechanism doesn't have runtime cost because conversion from operand expression to function call occurs at compile time, similar with function overlading 
in this respect.

## Rules

- Operator functions must start with the keyword `operator` and followed by the operator itself:

Myclass& operator=(){} // copy assignment operator of Myclass

- Operators such as `**`, `<>`, or `@` cannot be introduced.

- Some operators cannot be overloaded: 

```cpp
::  (scope resolution)
.   (dot)
? : (ternary)
sizeof
.*  (pointer-to-member access)
typeid
```

- Operator functions can either be:
	free funtion or
	non-static member function

- The following operators must always be implemented as members:

```cpp
 [](subsrcipt)
 ->(member access)
 ()(function call)
 type-cast operators
 assignment operators
```

- At least one of the operand must be either class type or enum type in order for operator overloading to kick in.

- Dot(.) and addressof(&) operators are defined by default for every class.

- Operator functions cannot take default argument with one exception: function call operator.

- The overloads of operators && and || lose short-circuit evaluation.

- It is not possible to change the precedence, grouping, or number of operands of operators:
```cpp
a * b + c > d // same with ((a * b) + c) > d
```
For example order of evaluation for the expression below cannot be done in this way via operator overloading, you have to use braces.
```cpp
a * ((b + c) > d)  
```

- All operator functions can be called by their names:
```cpp
class Myclass {
 public:
  Myclass& operator=(const Myclass&) {
    std::cout << "operator=(const Myclass)\n";
    return *this;
  }
};

int main()
{
  Myclass a, b, c;

  a = b = c; 
  cout << "the above and below statements will produce the same output\n";
  a.operator=(b.operator=(c));
}
```

<ins>Output</ins>  
operator=(const Myclass)  
operator=(const Myclass)  
the above and below statements will produce the same output  
operator=(const Myclass)  
operator=(const Myclass)  

Note that `operator=` function returns reference to Myclass, thus writing chaining expressions such as _a = b = c_ is made possible
and assignment operator evaluates from right to left.

- Member operator functions always is called for left operand and takes _this_ pointer for this object as hidden argument
```cpp
class Demo {
public:
  Demo& operator=(const Demo&) {
    std::cout << "operator=(const Myclass)\n";
    std::cout << "adress of this object " << this << "\n";
    return *this;
  }
};

int main()
{
  Demo d1, d2;

  d1 = d2;

  std::cout << "adress of d1 " << &d1 << "\n";
}
```
<ins>Output</ins>  
operator=(const Myclass)  
adress of this object 0000008C3EAFFC14  
adress of d1 0000008C3EAFFC14  

## Operator Overloading Best Practices

- Don't write global operator function and member operator function for the same operator(ambiguity).
- Operator functions that change the class object(i.e. set operators) ought to be member operator functions(such as assignment operators, prefix/postfix operators).
- Symmetric operator functions(`a + b, b + a`, result doesn't have to be the same) are usually preferred to be global operator functions.
- Overload of `operator==` function can be used in overloading `operator!=`
- Use `operator+` and `operator-` to implement compound assignment in order to avoid code duplication.
```cpp
#include <iostream>
class Vector {
public:
    int x, y;

    Vector(int x = 0, int y = 0) : x(x), y(y) {}

    // Compound assignment: +=
    Vector& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Compound assignment: -=
    Vector& operator-=(const Vector& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Normal + using +=
    friend Vector operator+(Vector lhs, const Vector& rhs) {
        lhs += rhs;  // reuse operator+=
        return lhs;
    }

    // Normal - using -=
    friend Vector operator-(Vector lhs, const Vector& rhs) {
        lhs -= rhs;  // reuse operator-=
        return lhs;
    }

    void print() const {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main() {
    Vector v1(2, 3);
    Vector v2(5, 1);

    Vector sum = v1 + v2; // operator+ kullanıyor
    Vector diff = v1 - v2; // operator- kullanıyor

    sum.print();   // (7, 4)
    diff.print();  // (-3, 2)
}
```
- Use `operator<` to  all relational operators:
```cpp
a < b
a > b    --->   b < a
a <= b	 --->		!(b < a)
a >= b	 --->		!(a < b)
```

## Points to Recall

- Note that some containers and algorithms of STL require `operator==` and `operator<` to be implemented by default for user defined classes.
- Comparison operators(equality and relational operators) typically returns bool.
- Assignment operator(including compound assignments) should almost always has return type of reference to class type (Myclass&).
- For pointer-like types(e.g. smart pointers, iterators in STL), typically dereference operator(*) and member selection(->) operator are overloaded.
- Arrow operator has a specific implementation that places operator function call expression left side of the arrow. Ex: operator->ptr->member.Even though
it is a binary operator, it is implemented as unary operator. Typically its return type is ought to be pointer to a class type.
- Type-cast operators cause user-defined conversion(UDC) and trigger implicit conversions. No need to add return type in the sytax since it is counted as casted-to type. 
- `operator bool()` kicks in logic contexts even if it is declared as explicit. It must be used with care since it will come into play in arithmetic operations implicitly via integral promotion.
As a reminder, logical contexts; for and while loops, if condition, logical operators, ternary operator etc.

Return types of some operators in STL:

| Operator Type       | Typical Return Type |
| ------------------- | ------------------- |
| Assignment          | `T&`                |
| Compound assignment | `T&`                |
| Arithmetic          | `T`                 |
| Comparison          | `bool`              |
| Dereference         | `T&` or `T*`        |
