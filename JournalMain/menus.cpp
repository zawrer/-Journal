#include "menus.h"
#include "tasks.h"
#include <iostream>
#include <cstdlib>
void menu_groups(){
    int choice;

    while (true) {
        system("clear");
        std::cout << "============УПРАВЛЕНИЕ ГРУППАМИ=============" << std::endl;
        std::cout << "1. Добавить группу" << std::endl;
        std::cout << "2. Удалить группу" << std::endl;
        std::cout << "3. Список групп" << std::endl;
        std::cout << "0. Вернуться в главное меню" << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "Ваш выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            system("clear");
            add_group(); 
            break;
        case 2:
            system("clear");
            del_group();
            break; 
        case 3:
            system("clear");
            get_groups(); 
            break;
        case 0:
            return; 
        default:
            std::cout << "Введите корректное число!" << std::endl;
        }

        std::cout << "\nНажмите Enter, чтобы продолжить...";
        std::cin.ignore(1000, '\n');
        std::cin.get();
    }
}

void menu_students() {
    int choice;
    while (true) {
        system("clear");
        std::cout << "============УПРАВЛЕНИЕ СТУДЕНТАМИ===========" << std::endl;
        std::cout << "1. Добавить студента" << std::endl;
        std::cout << "2. Удалить студента" << std::endl;
        std::cout << "3. Список студентов" << std::endl;
        std::cout << "0. Назад в главное меню" << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "Ваш выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1: system("clear"); add_student(); break;
        case 2: system("clear"); del_student(); break;
        case 3: system("clear"); get_students(); break;
        case 0: return;
        default: std::cout << "Введите корректное число!" << std::endl;
        }

        std::cout << "\nНажмите Enter, чтобы продолжить...";
        std::cin.ignore(1000, '\n');
        std::cin.get();
    }
}

void menu_subjects() {
    int choice;
    while (true) {
        system("clear");
        std::cout << "===========УПРАВЛЕНИЕ ПРЕДМЕТАМИ============" << std::endl;
        std::cout << "1. Добавить предмет" << std::endl;
        std::cout << "2. Удалить предмет" << std::endl;
        std::cout << "3. Список предметов" << std::endl;
        std::cout << "0. Назад в главное меню" << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "Ваш выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1: system("clear"); add_subject(); break;
        case 2: system("clear"); del_subject(); break;
        case 3: system("clear"); get_subjects(); break;
        case 0: return;
        default: std::cout << "Введите корректное число!" << std::endl;
        }

        std::cout << "\nНажмите Enter, чтобы продолжить...";
        std::cin.ignore(1000, '\n');
        std::cin.get();
    }
}
void menu_grades() {
    int choice;
    while (true) {
        system("clear");
        std::cout << "=============УПРАВЛЕНИЕ ОЦЕНКАМИ============" << std::endl;
        std::cout << "1. Добавить оценки" << std::endl;
        std::cout << "2. Отредактировать оценки" << std::endl;
        std::cout << "0. Назад в главное меню" << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "Ваш выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1: system("clear"); add_marks(); break; // Вызов из tasks.cpp
        case 2: system("clear"); edit_marks(); break; // Вызов из tasks.cpp
        case 0: return;
        default: std::cout << "Введите корректное число!" << std::endl;
        }

        std::cout << "\nНажмите Enter, чтобы продолжить...";
        std::cin.ignore(1000, '\n');
        std::cin.get();
    }
}