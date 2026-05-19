#include <iostream>
#include <cstdlib>
#include "menus.h"
#include "tasks.h"

int main(){

    setlocale(LC_ALL, "Russian");

    init_db();
    int choise;

    while (true) {
        system("clear");
        std::cout << "=====================  ЖУРНАЛ  =====================" << std::endl;
        std::cout << "1.Управление группами" << std::endl;
        std::cout << "2.Управление студентами" << std::endl;
        std::cout << "3.Управление предметами" << std::endl;
        std::cout << "4.Управление оценками" << std::endl;
        std::cout << "5.вычисление среднего балла" << std::endl;
        std::cout << "0.Выход из программы" << std::endl;
        std::cout << "====================================================" << std::endl;
        std::cout << "Ваш выбор: ";

        if (!(std::cin >> choise )){
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;

        }

        switch (choise){
        case 1:
            menu_groups();
            break;
        case 2:
            menu_students();
            break;
        case 3:
            menu_subjects();
            break;      
        case 4:
            menu_grades();
            break;
        case 5:
            system("clear");
            std::cout << "Подсчёт среднего балла по предметам";
            break;
        case 0:
            system("clear");
            std::cout <<  "Выход из программы..." << std::endl;
            return 0;
        default:
            std::cout<< "Введите коректное число";
        }

        std::cout << "\nНажмите на Enter, чтобы продолжить...";
        std::cin.ignore(1000, '\n');
        std::cin.get();
    }
    return 0;
}