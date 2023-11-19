#include <iostream>
#include <vector>
#include <list>

using namespace std;

template<typename C>
void print(const C& container, std::ostream& os = std::cout, const char* delimiter = " ")
{
    for (const auto& el : container)
        os << el << delimiter;
    cout << '\n';
}

template<typename InIter, typename OutIter> 
OutIter Copy(InIter beg, InIter end, OutIter destbeg) // typical copy function
{
    while (beg != end)
        *destbeg++ = *beg++;
    return destbeg;
}

template<typename C>
class BackInsertIter // possible implementation of back_insert_iterator
{
public:
    BackInsertIter(C& c) : mc{ c } {} // initializes the reference by the ctor argument (container)
    BackInsertIter& operator++() { return *this; } // eliminate the effects of dereference, postfix increment, and prefix increment by returning *this
    BackInsertIter operator++(int) { return *this; }
    BackInsertIter& operator*() { return *this; }
    BackInsertIter& operator=(typename const C::value_type& val) // copy and add the element to the back
    { 
        mc.push_back(val); 
        return *this;
    }
private:
    C& mc; // wraps the reference to the object which it receives as ctor argument (container)
};

template<typename C>
BackInsertIter<C> BackInserter(C& c) // possible implementation of back_inserter
{
    return BackInsertIter(c);
}

template<typename C>
class FrontInsertIter // possible implementation of front_insert_iterator
{
public:
    FrontInsertIter(C& c) : mc{ c } {} // initializes the reference by the ctor argument (container)
    FrontInsertIter& operator++() { return *this; } // eliminate the effects of dereference, postfix increment, and prefix increment by returning *this
    FrontInsertIter operator++(int) { return *this; }
    FrontInsertIter& operator*() { return *this; }
    FrontInsertIter& operator=(typename const C::value_type& val) // copy and add the element to the front
    {
        mc.push_front(val); 
        return *this;
    }
private:
    C& mc; // wraps the reference to the object which it receives as ctor argument (container)
};

template<typename C>
FrontInsertIter<C> FrontInserter(C& c) // possible implementation of back_inserter
{
    return FrontInsertIter(c);
}

int main()
{
    vector<int> ivec1{ 1,2,3 }, ivec2, ivec3, ivec4;
    list<int> ilis1{ 10, 9, 8, 7 ,6 }, ilis2;

    BackInsertIter iter1{ ivec1 }; // different ways of using BackInsertIterator class and BackInserter function
    iter1 = 4;
    BackInserter(ivec1) = 5;

    cout << "ivec1: ";
    print(ivec1);
    
    BackInsertIter iter2{ ivec2 };

    Copy(begin(ivec1), end(ivec1), iter2);

    cout << "ivec2: ";
    print(ivec2);

    Copy(begin(ivec1), end(ivec1), BackInsertIter{ ivec3 });

    cout << "ivec3: ";
    print(ivec3);

    Copy(begin(ivec1), end(ivec1), BackInserter(ivec4));

    cout << "ivec4: ";
    print(ivec4);

    Copy(begin(ilis1), end(ilis1), FrontInserter(ilis2));

    cout << "ilis2: ";
    print(ilis2);
}

