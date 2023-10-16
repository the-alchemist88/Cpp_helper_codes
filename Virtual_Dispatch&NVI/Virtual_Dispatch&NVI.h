#pragma once

#include <iostream>
#include <random>

using namespace std;

class Car               // base class
{
public:

    void test_car()     // NVI(Herb Sutter's acronym) --> Non-Virtual Interface: Calling private virtual functions of the base class in public section
    {
        start();        // Since these calls are made with hidden Car*, virtual dispatch mechanism can operate here
        run();
        stop();
    }

    virtual Car* clone() = 0; // There is no virtual ctor in C++, instead there is virtual clone idiom that constructs the exact same derived class object
                              // See the trivial implementation below in overriders

    friend ostream& operator<<(ostream& os, const Car& rhs) // An example about how to use a global function(operator <<()) in virtual dispatch
    {
        rhs.printName(os);
        return os;
    }

    virtual void printName(ostream&) const = 0;  

private:
    virtual void start()
    {
        cout << "Car has just started\n";
    }

    virtual void run()
    {
        cout << "Car is running now\n";
    }

    virtual void stop()
    {
        cout << "Car has just stopped\n";
    }
};

class Audi : public Car         // derived class-1
{

private:
    virtual void start() override
    {
        cout << "Audi has just started\n";
    }

    virtual void run() override
    {
        cout << "Audi is running now\n";
    }

    virtual void stop() override
    {
        cout << "Audi has just stopped\n";
    }

    virtual Car* clone() override
    {
        return new Audi(*this);
    }

    void printName(ostream& os) const override
    {
        os << "I am an Audi\n";
    }

};

class Ferrari :public Car       // derived class - 2
{
private:
    virtual void start() override
    {
        cout << "Ferrari has just started\n";
    }

    virtual void run() override
    {
        cout << "Ferrari is running now\n";
    }

    virtual void stop() override
    {
        cout << "Ferrari has just stopped\n";
    }

    virtual Car* clone() override
    {
        return new Ferrari(*this);
    }

    void printName(ostream& os) const override
    {
        os << "I am a Ferrari\n";
    }

};

class Mercedes :public Car      // derived class - 3
{
private:
    virtual void start() override
    {
        cout << "Mercedes has just started\n";
    }

    virtual void run() override
    {
        cout << "Mercedes is running now\n";
    }

    virtual void stop() override
    {
        cout << "Mercedes has just stopped\n";
    }

    virtual Car* clone() override
    {
        return new Mercedes(*this);
    }

    void printName(ostream& os) const override
    {
        os << "I am a Mercedes\n";
    }

};

class Toyota : public Car       // derived class - 4
{
private:
    virtual void start() override
    {
        cout << "Toyota has just started\n";
    }

    virtual void run() override
    {
        cout << "Toyota is running now\n";
    }

    virtual void stop() override
    {
        cout << "Toyota has just stopped\n";
    }

    virtual Car* clone() override
    {
        return new Toyota(*this);
    }

    void printName(ostream& os) const override
    {
        os << "I am a Toyota\n";
    }

};

class Volvo : public Car            // derived class - 5
{
private:
    virtual void start() override
    {
        cout << "Volvo has just started\n";
    }

    virtual void run() override
    {
        cout << "Volvo is running now\n";
    }

    virtual void stop() override
    {
        cout << "Volvo has just stopped\n";
    }

    virtual Car* clone() override
    {
        return new Volvo(*this);
    }

    void printName(ostream& os) const override
    {
        os << "I am a Volvo\n";
    }
};

class VolvoXC90 final: public Volvo     // derived class - 5 : dervied class - 1 (multilevel inheritence)
{
private:
    virtual void start() override
    {
        cout << "VolvoXC90 has just started\n";
    }

    //virtual void run() override                       // now that this is commented; the virtual function("run"),
    //{                                                 // in upper classes in multilevel inheritance hierachy, which is overridden will be called --> Iniside volvo
    //    cout << "VolvoXC90 is running now\n";
    //}

    virtual void stop() override
    {
        cout << "VolvoXC90 has just stopped\n";
    }

    virtual Car* clone() override
    {
        return new VolvoXC90(*this);
    }

    void printName(ostream& os) const override
    {
        os << "I am a VolvoXC90\n";
    }
};

Car* create_random_car()
{
    static mt19937 eng{ random_device{}() };            // create random numbers
    static uniform_int_distribution dist{ 0, 5 };

    switch (dist(eng))                                  // select the car brand to construct
    {
    case 0: return new Volvo;
    case 1: return new Audi;
    case 2: return new Toyota;
    case 3: return new Mercedes;
    case 4: return new Ferrari;
    case 5: return new VolvoXC90;
    }

    return nullptr;
}

