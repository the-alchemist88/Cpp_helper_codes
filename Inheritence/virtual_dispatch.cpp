#include "car.h" 
#include <cstdlib> 

void car_game(Car* p)
{
    Car* pnewcar = p->clone();
    p->test_car();
    (void)getchar();
    system("cls");
}

//void car_game(Car& p)     // ok, reference semantics allow virtual dispatch
//{
//p->test_car();
//(void)getchar();
//system("cls");
//}

//void car_game(Car p)      // object slicing.. virtual dispatch cannot operate
//{
//p->test_car();
//(void)getchar();
//system("cls");
//}

int main()
{
    Car* ptr{};
    for (size_t i{}; i < 10; ++i)
    {
        ptr = create_random_car();
        cout << *ptr << "\n";
        car_game(ptr);
    }

}