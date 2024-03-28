#include <iostream>

using namespace std;

// Singleton - 1) Only one object of this class type can be constructed, 2) All codes must have access to this object.

class Singleton
{
public:
    static Singleton& get_instance() // This is not thread-safe
    {
        if (!mp)
            mp = new Singleton; // smart pointer could be used
        return *mp;
    }

    Singleton(const Singleton&) = delete; // now move members implicitly-declared and deleted

    void foo() { cout << "foo()\n"; };
    void bar() { cout << "bar()\n"; };
    void baz() { cout << "baz()\n"; };

	~Singleton()
	{
		delete mp;
	}

private:
    Singleton() { cout << "Singleton object constructed\n"; };
    inline static Singleton* mp{};

};

// Better alternative Meyers_Singleton

class Meyers_Singleton
{
public:
    static Meyers_Singleton& get_instance() // This is thread-safe, because local static variables are guarenteed to be initalized only once even if there are more than one thread
    {
        static Meyers_Singleton instance;
        return instance;
    }

    Meyers_Singleton(const Meyers_Singleton&) = delete; // now move members implicitly-declared and deleted

    void foo() { cout << "m_foo()\n"; };
    void bar() { cout << "m_bar()\n"; };
    void baz() { cout << "m_baz()\n"; };


private:
    Meyers_Singleton() { cout << "Meyers_Singleton object constructed\n"; };

};

int main()
{

    Singleton::get_instance().foo();
    
    Singleton::get_instance().bar();

    auto& sng = Singleton::get_instance();

    sng.baz();

    cout << "\n\n";

    Meyers_Singleton::get_instance().foo();

    Meyers_Singleton::get_instance().bar();

    auto& m_sng = Meyers_Singleton::get_instance();

    m_sng.baz();
}