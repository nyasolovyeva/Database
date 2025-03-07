#include "Database.h"

int main() {
    Database db;
    db.create("students.db");    // Создание новой базы данных
    //db.open("students.db");      // Открытие базы данных

    // Добавление студентов
    db.addStudent("Alice", 20, "Ilya", 23, 30.5f);
    db.addStudent("Bob", 21, "Oleg", 34, 30.8f);
    db.addStudent("Charlie", 22, "Curt", 23, 30.6f);
    db.addStudent("Alice", 20, "Ilya", 23, 30.5f);
    db.addStudent("Alice", 20, "Ilya", 23, 30.5f);
    db.addStudent("Alice", 20, "Ilya", 23, 30.5f);

    db.searchByField("Alice", 2);
    db.deleteByField("Alice", 2);

    //db.printAll();  // Вывод всех записей
    //db.backup();
    //db.clear();     // Очистка базы данных
    db.addStudent("Bob", 21, "Oleg", 34, 30.8f);
    db.editStudent(2, "Max", 21, "SuperMax", 34, 30.8f);
    //db.printAll();  // Проверка очистки (должно быть пусто)
    //db.restore();
    //db.save();
    //db.remove();    // Удаление базы данных

    /*Database db;
    db.create("students.db");    // Создание новой базы данных 
    //db.open("students.db");      // Открытие базы данных 

    for (int i = 0; i < 1000; i++) {
        db.addStudent("Alice" + std::to_string(i), 20, "Ilya", 23, 30.5f);
    }
    std::chrono::time_point<std::chrono::steady_clock> Start = std::chrono::high_resolution_clock::now();;

    db.searchByField("Alice56", 2);

    std::chrono::time_point<std::chrono::steady_clock> End = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = End - Start;
    float result = duration.count();
    std::cout << "Прошло времени " << result << " секунд" << std::endl;*/

    return 0;
}