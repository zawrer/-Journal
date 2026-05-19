#include "tasks.h"
#include <iostream>
#include <sqlite3.h>
#include <iomanip>

void init_db(){
    sqlite3* DB = nullptr;
    char* errorMessage = nullptr;

    if (sqlite3_open("journal.db", &DB) != SQLITE_OK){
        std::cerr << "ошибка подключения к БД: " << sqlite3_errmsg(DB) << std::endl; 
        return;
    }

    std::string sql_groups =
    "CREATE TABLE IF NOT EXISTS groups("
    "id INT PRIMARY KEY NOT NULL,"
    "name TEXT NOT NULL);";

    std::string sql_students = 
    "CREATE TABLE IF NOT EXISTS students("
    "id INT PRIMARY KEY NOT NULL,"
    "name TEXT NOY NULL,"
    "group_id INT NOT NULL);";

    std::string sql_subjects =
    "CREATE TABLE IF NOT EXISTS subjects("
    "id INT PRIMARY KEY NOT NULL,"
    "name TEXT NOT NULL);";

    std::string sql_grades =
    "CREATE TABLE IF NOT EXIST grades"
    "id INT PRIMARY KEY NOT NULL,"
    "student.id INT NOT NULL,"
    "subject.id INT NOT NULL," 
    "value INT NOT NULL,"
    "attempt INT NOT NULL);";

    int rc1 = sqlite3_exec(DB, sql_groups.c_str(), nullptr, nullptr, &errorMessage);
    int rc2 = sqlite3_exec(DB, sql_students.c_str(), nullptr, nullptr, &errorMessage);
    int rc3 = sqlite3_exec(DB, sql_subjects.c_str(), nullptr, nullptr, &errorMessage);
    int rc4 = sqlite3_exec(DB, sql_grades.c_str(), nullptr, nullptr, &errorMessage);

    if (rc1 != SQLITE_OK || rc2 != SQLITE_OK || rc3 != SQLITE_OK || rc4 != SQLITE_OK){
        std::cerr << "ошибка при создании таблиц: " << errorMessage << std::endl; 
        sqlite3_free(errorMessage);
    }else{
        std::cout << "База успешно инициализирована!" << std::endl;
    }

    sqlite3_close(DB);
}

void add_group(){
    sqlite3* DB =nullptr;
    std::string name;
    int id;
    if (sqlite3_open("journal.db", &DB) != SQLITE_OK){
        std::cerr << "ошибка подключения к БД: " << sqlite3_errmsg(DB) << std::endl; 
        return;
    }

    std::cout << "Введите номер (ID) группы: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Введите название группы: ";
    std::getline(std::cin, name );

    const char* sql = "INSERT INTO groups (id, name) VALUES (?, ?);";
    sqlite3_stmt* stmt = nullptr;
    
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_close(DB);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE){
        std::cout << "Успех! Группа создана." << std::endl;
    }else{
        std::cout << "Ошибка: НЕ УДАЛОСЬ СОЗДАТЬ ГРУППУ (Возможно группа с таким (ID) уже есть)" << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void del_group() {
    sqlite3* DB = nullptr;
    int id;
    if (sqlite3_open("journal.db", &DB) != SQLITE_OK){
        std::cerr << "ошибка подключения к БД: " << sqlite3_errmsg(DB) << std::endl; 
        return;
    }
    const char* sql ="SELECT * FROM groups;";
    sqlite3_stmt* stmt = nullptr;
     if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_close(DB);
        return;
    }
    std::cout << "\n" << std::left
              << std::setw(12) << "ID группы"
              << std::setw(20) << "Название группы";
    std::cout << std::string(35, '_') << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW){
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::cout << std::left
                  << std::setw(12) << id 
                  << std::setw(20) << (name ? name : "[НЕТ ИМЕНИ]") << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}
void get_groups() {
    sqlite3* DB = nullptr;
    if (sqlite3_open("journal.db", &DB) != SQLITE_OK){
        std::cerr << "ошибка подключения к БД: " << sqlite3_errmsg(DB) << std::endl; 
        return;
    }
    const char* sql ="SELECT * FROM groups;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_close(DB);
        return;
    }
    std::cout << "\n" << std::left
              << std::setw(12) << "ID группы"
              << std::setw(20) << "Название группы" << std::endl;
    std::cout << std::string(35, '_') << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW){
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::cout << std::left
                  << std::setw(12) << id 
                  << std::setw(20) << (name ? name : "[НЕТ ИМЕНИ]") << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void add_student() {
    sqlite3* DB = nullptr;
    std::string name;
    int gid, id;

    if (sqlite3_open("journal.db", &DB) != SQLITE_OK) {
        std::cerr << "Ошибка подключения к БД: " << sqlite3_errmsg(DB);
        return;
    }

    std::cout << "Введите номер группы (куда добавить студента): ";
    std::cin >> gid;
    std::cout << "Введите ID студента: ";
    std::cin >> id;
    std::cin.ignore(); 
    std::cout << "Введите ФИО студента: ";
    std::getline(std::cin, name);

    
    const char* sql_check = "SELECT * FROM groups WHERE id = ?;";
    sqlite3_stmt* stmt_check = nullptr;
    sqlite3_prepare_v2(DB, sql_check, -1, &stmt_check, nullptr);
    sqlite3_bind_int(stmt_check, 1, gid);

   
    if (sqlite3_step(stmt_check) == SQLITE_ROW) {
        const char* sql_insert = "INSERT INTO students (id, name, group_id) VALUES (?, ?, ?);";
        sqlite3_stmt* stmt_insert = nullptr;
        sqlite3_prepare_v2(DB, sql_insert, -1, &stmt_insert, nullptr);

        sqlite3_bind_int(stmt_insert, 1, id);
        sqlite3_bind_text(stmt_insert, 2, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt_insert, 3, gid);

        if (sqlite3_step(stmt_insert) == SQLITE_DONE) {
            std::cout << "Успех! Студент добавлен в группу " << gid << std::endl;
        } else {
            std::cout << "Ошибка: Не удалось добавить студента (возможно, ID уже занят)." << std::endl;
        }
        sqlite3_finalize(stmt_insert);
    } else {
        std::cout << "Ошибка: Группы с ID " << gid << " не существует! Сначала создайте группу." << std::endl;
    }

    sqlite3_finalize(stmt_check);
    sqlite3_close(DB);
}

void del_student() {
    sqlite3* DB = nullptr;
    int id;

    if (sqlite3_open("journal.db", &DB) != SQLITE_OK) {
        std::cerr << "Ошибка подключения к БД: " << sqlite3_errmsg(DB);
        return;
    }

    std::cout << "Введите ID студента для удаления: ";
    std::cin >> id;

    const char* sql = "DELETE FROM students WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (sqlite3_changes(DB) > 0) {
            std::cout << "Успех! Студент удалён." << std::endl;
        } else {
            std::cout << "Ошибка: Студент с таким ID не найден." << std::endl;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void get_students() {
    sqlite3* DB = nullptr;
    if (sqlite3_open("journal.db", &DB) != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД" << std::endl;
        return;
    }

    const char* sql = "SELECT * FROM students;";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);

    std::cout << "\n" << std::left
              << std::setw(12) << "ID Студ."
              << std::setw(25) << "ФИО"
              << std::setw(12) << "ID Группы" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int gid = sqlite3_column_int(stmt, 2);

        std::cout << std::left
                  << std::setw(12) << id
                  << std::setw(25) << (name ? name : "")
                  << std::setw(12) << gid << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void add_subject() {
    sqlite3* DB = nullptr;
    std::string name;
    int id;

    if (sqlite3_open("journal.db", &DB) != SQLITE_OK) {
        std::cerr << "Ошибка подключения к БД: " << sqlite3_errmsg(DB);
        return;
    }

    std::cout << "Введите ID предмета: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Введите название предмета: ";
    std::getline(std::cin, name);

    const char* sql = "INSERT INTO subjects (id, name) VALUES (?, ?);";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Успех! Предмет добавлен." << std::endl;
    } else {
        std::cout << "Ошибка: Не удалось добавить предмет." << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void del_subject() {
    sqlite3* DB = nullptr;
    int id;

    if (sqlite3_open("journal.db", &DB) != SQLITE_OK) {
        std::cerr << "Ошибка подключения к БД: " << sqlite3_errmsg(DB);
        return;
    }

    std::cout << "Введите ID предмета для удаления: ";
    std::cin >> id;

    const char* sql = "DELETE FROM subjects WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (sqlite3_changes(DB) > 0) {
            std::cout << "Успех! Предмет удалён." << std::endl;
        } else {
            std::cout << "Ошибка: Предмет не найден." << std::endl;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void get_subjects() {
    sqlite3* DB = nullptr;
    if (sqlite3_open("journal.db", &DB) != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД" << std::endl;
        return;
    }

    const char* sql = "SELECT * FROM subjects;";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);

    std::cout << "\n" << std::left
              << std::setw(15) << "ID Предмета"
              << std::setw(25) << "Название предмета" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        std::cout << std::left
                  << std::setw(15) << id
                  << std::setw(25) << (name ? name : "") << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

// --- ОЦЕНКИ (GRADES) ---

void add_marks() {
    sqlite3* DB = nullptr;
    int id, studid, subid, value, attempt;

    if (sqlite3_open("journal.db", &DB) != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД" << std::endl;
        return;
    }

    // Запрашиваем данные
    std::cout << "Введите ID записи оценки: "; std::cin >> id;
    std::cout << "Введите ID студента: ";     std::cin >> studid;
    std::cout << "Введите ID предмета: ";     std::cin >> subid;
    std::cout << "Введите оценку: ";          std::cin >> value;
    std::cout << "Введите номер попытки: ";   std::cin >> attempt;
    std::cin.ignore();

    // Запрос для проверки: существуют ли студент И предмет одновременно
    const char* sql_check = "SELECT EXISTS(SELECT 1 FROM students WHERE id=?) AND EXISTS(SELECT 1 FROM subjects WHERE id=?);";
    sqlite3_stmt* stmt_check = nullptr;

    if (sqlite3_prepare_v2(DB, sql_check, -1, &stmt_check, nullptr) != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса проверки: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_close(DB);
        return;
    }

    sqlite3_bind_int(stmt_check, 1, studid);
    sqlite3_bind_int(stmt_check, 2, subid);
    sqlite3_step(stmt_check);

    int exists = sqlite3_column_int(stmt_check, 0); // Получаем 1 (true) или 0 (false)
    sqlite3_finalize(stmt_check);

    if (exists == 1) {
        // Если всё существует — добавляем оценку
        const char* sql_insert = "INSERT INTO grades (id, student_id, subject_id, value, attempt) VALUES (?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt_insert = nullptr;

        sqlite3_prepare_v2(DB, sql_insert, -1, &stmt_insert, nullptr);
        sqlite3_bind_int(stmt_insert, 1, id);
        sqlite3_bind_int(stmt_insert, 2, studid);
        sqlite3_bind_int(stmt_insert, 3, subid);
        sqlite3_bind_int(stmt_insert, 4, value);
        sqlite3_bind_int(stmt_insert, 5, attempt);

        if (sqlite3_step(stmt_insert) == SQLITE_DONE) {
            std::cout << "Успешно! Оценка выставлена." << std::endl;
        } else {
            std::cout << "Ошибка: Не удалось добавить оценку. Возможно, этот ID записи уже занят." << std::endl;
        }
        sqlite3_finalize(stmt_insert);
    } else {
        std::cout << "Ошибка: Студента с ID " << studid << " или Предмета с ID " << subid << " не существует в базе!" << std::endl;
    }

    sqlite3_close(DB);
}

void edit_marks() {
    sqlite3* DB = nullptr;
    int id, value, attempts;

    if (sqlite3_open("journal.db", &DB) != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД" << std::endl;
        return;
    }

    std::cout << "Введите ID записи оценки для редактирования: "; std::cin >> id;
    std::cout << "Введите новую оценку: ";                       std::cin >> value;
    std::cout << "Сколько попыток добавить к текущим: ";          std::cin >> attempts;
    std::cin.ignore();

    // Сначала узнаем текущее количество попыток
    const char* sql_select = "SELECT attempt FROM grades WHERE id=?;";
    sqlite3_stmt* stmt_select = nullptr;
    sqlite3_prepare_v2(DB, sql_select, -1, &stmt_select, nullptr);
    sqlite3_bind_int(stmt_select, 1, id);

    if (sqlite3_step(stmt_select) == SQLITE_ROW) {
        int current_attempts = sqlite3_column_int(stmt_select, 0);
        int new_attempt = current_attempts + attempts; // Прибавляем новые попытки
        sqlite3_finalize(stmt_select);

        // Обновляем оценку и попытки
        const char* sql_update = "UPDATE grades SET value=?, attempt=? WHERE id=?;";
        sqlite3_stmt* stmt_update = nullptr;
        sqlite3_prepare_v2(DB, sql_update, -1, &stmt_update, nullptr);

        sqlite3_bind_int(stmt_update, 1, value);
        sqlite3_bind_int(stmt_update, 2, new_attempt);
        sqlite3_bind_int(stmt_update, 3, id);

        if (sqlite3_step(stmt_update) == SQLITE_DONE) {
            std::cout << "Успешно! Данные изменены. Всего попыток: " << new_attempt << std::endl;
        } else {
            std::cout << "Ошибка при обновлении оценки." << std::endl;
        }
        sqlite3_finalize(stmt_update);
    } else {
        std::cout << "Ошибка: Оценка с ID " << id << " не найдена." << std::endl;
        sqlite3_finalize(stmt_select);
    }

    sqlite3_close(DB);
}

void get_average_mark() {
    sqlite3* DB = nullptr;
    int id;

    if (sqlite3_open("journal.db", &DB) != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД" << std::endl;
        return;
    }

    std::cout << "Введите ID студента для расчета среднего балла: ";
    std::cin >> id;

    const char* sql = "SELECT value FROM grades WHERE student_id = ?;";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    int sum = 0;
    int count = 0;

  
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        sum += sqlite3_column_int(stmt, 0);
        count++;
    }

    if (count > 0) {
        double avg = static_cast<double>(sum) / count;
        std::cout << "Средний балл студента (ID: " << id << ") равняется = " << avg << std::endl;
    } else {
        std::cout << "У студента с ID " << id << " нет оценок. Наверное, он не аттестован ;)" << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}