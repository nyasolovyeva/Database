#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "windows.h"

class Database {
private:
    std::fstream file;
    std::string filename;
    std::vector<int> del_key; //Вектор с удалёнными ключами
    int num_zap; //Количество записей(строк)
    int size_tab; //Размер таблицы(Записи + пустые строки)

public:
    Database();
    ~Database();

    int getSize() { return num_zap; }

    void create(std::string _filename);
    bool open(std::string _filename);
    void remove();
    void clear();
    void save();

    bool addStudent(const std::string& name, int group, const std::string& name_coach, int num_zan, float fastest_lap); // С проверкой уникальности
    void deleteByField(std::string value, int field); // Удаление по полю
    bool searchByField(std::string value, int field); // Поиск по полю
    std::string searchByKey(int value, int field); //Поиск по ключу
    std::vector<std::string> searchByNonKey(std::string value, int field);
    bool editStudent(int id, const std::string& name, int group, const std::string& name_coach, int num_zan, float fastest_lap); // Редактирование записи
    void backup(); // Создание резервной копии
    void restore(); // Восстановление из резервной копии
    std::vector<std::vector<std::string>> returnStroki();
    std::string getFullLine(std::fstream& inFile, int value);
    std::string getCell(std::string line, int col);
    void exportToCSV(const std::string& csvFilename); // Экспорт в CSV
};