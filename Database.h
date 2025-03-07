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
    std::vector<int> del_key; //������ � ��������� �������
    int num_zap; //���������� �������(�����)
    int size_tab; //������ �������(������ + ������ ������)

public:
    Database();
    ~Database();

    int getSize() { return num_zap; }

    void create(std::string _filename);
    bool open(std::string _filename);
    void remove();
    void clear();
    void save();

    bool addStudent(const std::string& name, int group, const std::string& name_coach, int num_zan, float fastest_lap); // � ��������� ������������
    void deleteByField(std::string value, int field); // �������� �� ����
    bool searchByField(std::string value, int field); // ����� �� ����
    std::string searchByKey(int value, int field); //����� �� �����
    std::vector<std::string> searchByNonKey(std::string value, int field);
    bool editStudent(int id, const std::string& name, int group, const std::string& name_coach, int num_zan, float fastest_lap); // �������������� ������
    void backup(); // �������� ��������� �����
    void restore(); // �������������� �� ��������� �����
    std::vector<std::vector<std::string>> returnStroki();
    std::string getFullLine(std::fstream& inFile, int value);
    std::string getCell(std::string line, int col);
    void exportToCSV(const std::string& csvFilename); // ������� � CSV
};