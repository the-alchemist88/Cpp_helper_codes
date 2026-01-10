#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Fighter
{
public:
    Fighter(const char* name) : m_name{ name }
    {
        ++alive_fighter_num;
        ++everlived_fighter_num;
        alive.push_back(this);
    }

    ~Fighter()
    {
        cout << m_name << " has fallen\n";
        dead.push_back(m_name);
        --alive_fighter_num;
        std::erase(alive, this); // C++20, std::erase instead of remove-erase idiom
    }

    Fighter(const Fighter&) = delete;
    Fighter& operator=(const Fighter&) = delete;

    void ask_help()
    {
        cout << "Hey I'm " << m_name << '\n';
        for (auto p : alive)
            if(p != this)
                cout << p->m_name << "! ";
        cout << "\nGuys, I need back-up!\n\n";
    }

    static void print_figter_num()
    {
        cout << "alive_fighter_num = " << alive_fighter_num << "\n";
        cout << "everlived_fighter_num = " << everlived_fighter_num << "\n\n";
    }

    static void print_fighters()
    {
        cout << "alive fighters: ";
        for (auto p : alive)
            cout << p->m_name << " ";
        cout << '\n';
        cout << "neutralised fighters: ";
        for (auto s : dead)
            cout << s << " ";
        cout << "\n\n";
    }

private:
    string m_name;
    inline static vector<Fighter*> alive{}; // C++17, inline keyword for in-class static data member init
    inline static vector<string> dead{};
    inline static size_t alive_fighter_num{};
    inline static size_t everlived_fighter_num{};
};


// test code
int main()
{
    Fighter f1("Alice");
    Fighter f2("Bob");
    Fighter f3("Charlie");

    Fighter* p1 = new Fighter ("David");
    Fighter* p2 = new Fighter ("Eve");
    Fighter* p3 = new Fighter ("Frank");
    
    f1.ask_help();
    delete p1;
    Fighter::print_fighters();
    f2.ask_help();

    delete p2;
    delete p3;
    Fighter::print_fighters();
    Fighter::print_figter_num();
}
