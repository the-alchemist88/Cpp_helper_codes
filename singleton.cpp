#include <iostream>

using namespace std;

// 1) Singleton - only one object of this class type can be constructed, 2) All codes must have access to this object.

class Singleton
{
public:
    static Singleton& get_instace() // This is not thread-safe
    {
        if (!mp)
            mp = new Singleton; // smart pointer could be used
        return *mp;
    }

    Singleton(Singleton&&) = delete;

    void foo() { cout << "foo()\n"; };
    void bar() { cout << "bar()\n"; };
    void baz() { cout << "baz()\n"; };


private:
    Singleton() { cout << "Singleton object constructed\n"; };
    inline static Singleton* mp{};

};

class Meyers_Singleton
{
public:
    static Meyers_Singleton& get_instace() // This is thread-safe, because local static variables are guarenteed to be initalized only once even if there are more than one thread
    {
        static Meyers_Singleton instance;
        return instance;
    }

    Meyers_Singleton(Meyers_Singleton&&) = delete;

    void foo() { cout << "m_foo()\n"; };
    void bar() { cout << "m_bar()\n"; };
    void baz() { cout << "m_baz()\n"; };


private:
    Meyers_Singleton() { cout << "Meyers_Singleton object constructed\n"; };

};

int main()
{

    Singleton::get_instace().foo();
    
    Singleton::get_instace().bar();

    auto& sng = Singleton::get_instace();

    sng.baz();

    cout << "\n\n";

    Meyers_Singleton::get_instace().foo();

    Meyers_Singleton::get_instace().bar();

    auto& m_sng = Meyers_Singleton::get_instace();

    m_sng.baz();
}