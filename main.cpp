#include "Database.h"

int main() {
    Database db;
    db.create("students.db");    // �������� ����� ���� ������
    //db.open("students.db");      // �������� ���� ������

    // ���������� ���������
    db.addStudent("Alice", 20, "Ilya", 23, 30.5f);
    db.addStudent("Bob", 21, "Oleg", 34, 30.8f);
    db.addStudent("Charlie", 22, "Curt", 23, 30.6f);
    db.addStudent("Alice", 20, "Ilya", 23, 30.5f);
    db.addStudent("Alice", 20, "Ilya", 23, 30.5f);
    db.addStudent("Alice", 20, "Ilya", 23, 30.5f);

    db.searchByField("Alice", 2);
    db.deleteByField("Alice", 2);

    //db.printAll();  // ����� ���� �������
    //db.backup();
    //db.clear();     // ������� ���� ������
    db.addStudent("Bob", 21, "Oleg", 34, 30.8f);
    db.editStudent(2, "Max", 21, "SuperMax", 34, 30.8f);
    //db.printAll();  // �������� ������� (������ ���� �����)
    //db.restore();
    //db.save();
    //db.remove();    // �������� ���� ������

    /*Database db;
    db.create("students.db");    // �������� ����� ���� ������ 
    //db.open("students.db");      // �������� ���� ������ 

    for (int i = 0; i < 1000; i++) {
        db.addStudent("Alice" + std::to_string(i), 20, "Ilya", 23, 30.5f);
    }
    std::chrono::time_point<std::chrono::steady_clock> Start = std::chrono::high_resolution_clock::now();;

    db.searchByField("Alice56", 2);

    std::chrono::time_point<std::chrono::steady_clock> End = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = End - Start;
    float result = duration.count();
    std::cout << "������ ������� " << result << " ������" << std::endl;*/

    return 0;
}