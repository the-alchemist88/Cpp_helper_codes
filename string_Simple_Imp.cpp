#define _CRT_SECURE_NO_WARNINGS // in MSVS needed

#include <iostream>

class String
{
public:

    String() : length{}, p{ const_cast<char*>("") }
    {
        //std::cout << "String()\n";
    }

    String(const char* s) : length{ strlen(s) }, p{ new char[length + 1] } // intentionally non-explicit
    {
        //std::cout << "String(const char* s)\n";
        strcpy(p, s);
    }

    ~String()
    {
        std::cout << "~String(): " << this << "\n";
        if (p != "")    // if the object is already moved-from or just default constructed then no need to delete its resource
            delete[] p; // my choice is giving them value of "" as default constructed value(in default ctor and move members)
                        // in order to be able to print also moved-from and default constructed objects
    }                   

    String& operator=(const String& rhs)
    {
        //std::cout << "String& operator=(const String& rhs)\n";
        if (this != &rhs)
        {
            delete[] p;
            length = rhs.length;
            p = new char[length + 1];
            strcpy(p, rhs.p);
        }

        return *this;
    }

    String& operator=(const char* rhs)
    {
        //std::cout << "String& operator=(const char* rhs)\n";
        if (strcmp(p, rhs))
        {
            if (strcmp(p,""))
                delete[] p;
            length = strlen(rhs);
            p = new char[length + 1];
            strcpy(p, rhs);
        }

        return *this;
    }

    String(const String& rhs) : length{ rhs.length }, p{ new char[length + 1] }
    {
        //std::cout << "String(const String& rhs)\n";
        strcpy(p, rhs.p);
    }

    String& operator=(String&& rhs) noexcept
    {
        //std::cout << "String& operator=(String&& rhs)\n";
        if (this != &rhs)
        {
            if(p != "")
                delete[] p; // delete the resource of the object that will be moved to
            length = rhs.length;
            p = rhs.p;
            rhs.p = const_cast<char*>(""); // these values can change. For example, rhs.p = nullptr
            rhs.length = 0; // the purpose is to check in dtor that it doesn't delete the resource of a moved-from object(already owned by another object)
        }

        return *this;
    }

    String(String&& rhs) noexcept : length{ rhs.length }, p{rhs.p}
    {
        //std::cout << "String(String&& rhs)\n";
        rhs.p = const_cast<char*>("");
        rhs.length = 0;
    }

    size_t get_length()
    {
        return length;
    }

    friend std::ostream& operator<<(std::ostream& os, const String& s)
    {
        return os << s.p;
    }

private:
    size_t length;
    char* p;
};


// test code
int main()
{
    String s1{ "Hello world!" };    // String(const char* s)

    std::cout << "s1 = " << s1 << '\n';

    String s2 = std::move(s1);      // move ctor

    std::cout << "moved-from s1: " << s1 << '\n';
    std::cout << "moved-to s2: " << s2 << '\n';

    s1 = "Hello again!";
    std::cout << "assigned-to s1: " << s1 << '\n';

    String s3;                      // default ctor
    String s4 = "Hey there!";       // String(const char* s), non-explicit
    s3 = std::move(s4);             // move assignment
    String s5{ s3 };                // copy ctor

    std::cout << "s3: " << s3 << '\n';
    std::cout << "moved-from s4: " << s4 << '\n';
    std::cout << "s5: " << s5 << '\n';

    std::cout << "lentgh of s3 = " << s3.get_length() << '\n';
    std::cout << "lentgh of s4 = " << s4.get_length() << '\n';
    std::cout << "lentgh of s5 = " << s5.get_length() << "\n\n";
}   // dtors


