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
        cout << "Car has just stoppped\n";
    }
};

class Audi : public Car
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
        cout << "Audi has just stoppped\n";
    }
};

class Ferrari :public Car
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
        cout << "Ferrari has just stoppped\n";
    }
};

class Mercedes :public Car
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
        cout << "Mercedes has just stoppped\n";
    }
};

class Toyota : public Car
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
        cout << "Toyota has just stoppped\n";
    }
};

class Volvo : public Car
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
        cout << "Volvo has just stoppped\n";
    }
};

class VolvoXC90 : public Volvo                          // multilevel inheritence
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
        cout << "VolvoXC90 has just stoppped\n";
    }
};

Car* create_random_car()
{
    static mt19937 eng{ random_device{}() };            // create random numbers
    static uniform_int_distribution dist{ 0, 5 };

    switch (dist(eng))
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