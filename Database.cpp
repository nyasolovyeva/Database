#include "DataBase.h"
#include <codecvt>

int num_col = 6; //���������� ���������
int len_cell = 15; //����� ������
long len_stroki = len_cell * num_col + 7; //����� �������(5- ����������� "|", 2- "\n")
std::string empty_str(len_stroki - 2, ' '); //������ ��������� �� ��������(������ ������ � �������)(-2 ��-�� \n)
char delimiter = '|'; //����������� ����� �������� � �������

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    std::istringstream iss(str);

    while (std::getline(iss, token, delimiter)) {
        while (token.length() != 0 && std::isspace(token.back())) token.pop_back(); //������� ������� � ����� ������
        result.push_back(token);
    }
    return result;
}

std::string Database::getFullLine(std::fstream& inFile, int value) {
    inFile.seekg(len_stroki * (value - 1), std::ios::beg); //������ ������ ��� ������ �� ������ ������ ��� ������

    std::string line;
    getline(inFile, line);

    inFile.clear(); //����� ����� ��������� ����, ����������� ������

    return line;
}

std::string zap_prob(const std::string& stroka) { //���������� ������ ��������� �� ����� len_cell
    std::string probel(len_cell - stroka.length(), ' ');
    return probel;
}

bool StringToBool(const std::string& s) { return s != "0"; }

Database::Database() {
    num_zap = 0;
    size_tab = 0;
}

Database::~Database() {
    file.close();
}

void Database::create(std::string _filename) {
    if (filename != _filename) {
        file.close();
        filename = _filename;
    }
    clear();

    if (file) {
        std::cout << "Database created: " << filename << std::endl;
    }
    else {
        std::cout << "Database has not been created." << std::endl;
    }
}

bool Database::open(std::string _filename) {
    if (filename != _filename) {
        file.close();
        del_key.clear(); //����� ������� ������ � ���������� ������� (�� ������, ����� � ��� ��� ������ ���� �� �������� ����� �� �����)
        num_zap = 0; //�������� ���-�� ������� � ���� ������
        size_tab = 0; //�������� ������ �������

        filename = _filename;
    }
    if (file.is_open()) { file.close(); } //���� ���� ��� ������, �� �������
    file.open(filename, std::ios::in | std::ios::out); //���� ����� ������������ ios::trunc, �� ������ ��, ��� ���� � ����� �� �����, � ���� ios::app, �� ��������� ����� � ����� ����� � ����������� �� ���������� ������ ������� �� ���������

    if (!file) {
        std::cout << "Error: Could not open database file " << filename << std::endl;
        return false;
    }

    file.seekg(0, std::ios::beg);

    std::string line;

    while (getline(file, line)) { //���� �� ��������� ����� ����� ������ ��������� ������ � ���������� line
        size_tab++;
        if (line == empty_str) {
            del_key.push_back(size_tab);
        }
        else {
            num_zap++;
        }
    }

    file.clear();

    std::cout << "Database opened successfully." << std::endl;
    return true;
}

void Database::remove() {
    file.close(); //���� �� ������� ����, �� �� ������ �������
    if (std::remove(filename.c_str()) == 0) {
        std::cout << "Database deleted: " << filename << std::endl;
    }
    else {
        std::cout << "Error: Could not delete database file " << filename << std::endl;
    }
}

void Database::clear() {
    if (file.is_open()) { file.close(); } //���� ���� ��� ������, �� �������

    //��������� ���� � ������ trunc(����� ������� ��, ��� � �� ����)
    file.open(filename, std::ios::in | std::ios::out | std::ofstream::trunc); //in- ������ �� �����, out- ������ � ����, trunc- �������� ����� � ��������� ����, ��� ���� � ��

    del_key.clear(); //����� ������� ������ � ���������� ������� (�� ������, ����� � ��� ��� ������ ���� �� �������� ����� �� �����)
    num_zap = 0; //�������� ���-�� ������� � ���� ������
    size_tab = 0; //�������� ������ �������

    std::cout << "Database cleared." << std::endl;
}

void Database::save() {
    CreateDirectory(L"./saves", NULL); //.- ������� ����������
    std::fstream save_file("./saves/" + filename, std::ios::in | std::ios::out | std::ofstream::trunc);
    if (!save_file) {
        std::cout << "Error: Could not save database file " << filename << std::endl;
        return;
    }

    file.seekg(0, std::ios::beg);

    std::string line;

    while (getline(file, line)) { //���� �� ��������� ����� ����� ������ ��������� ������ � ���������� line
        save_file << line << std::endl;
    }

    file.clear(); //����� ������� ��������� ����� ���������� ����(������� ��������� ���������� ������ ��� ������) ��� file, ��� ���� ����������(���� �������� ������� ���� ��� ����, ����� �� ����� ������ ������ ��� ���������� � ����)

    std::cout << "Database saved successfully." << std::endl;
}

bool Database::addStudent(const std::string& name, int group, const std::string& name_coach, int num_zan, float fastest_lap) {
    bool flag = (name.length() <= len_cell) && (std::to_string(group).length() <= len_cell) && (name_coach.length() <= len_cell) && \
        (std::to_string(num_zan).length() <= len_cell) && (std::to_string(fastest_lap).length() <= len_cell); //�������� ����� ���� �������� �����, ����� � ������ �� ������

    if (flag) {
        int id;

        if (del_key.size() == 0) {
            id = num_zap + 1;
            file.seekp(0, std::ios::end); //������ ������ ��� ������ � ����� �����
            size_tab++;
        }
        else {
            id = del_key[0]; //������� ������ ���� �� �������
            file.seekp(len_stroki * (id - 1), std::ios::beg); //������ ������ ��� ������ �� ������ ������ ��� ������
            del_key.erase(del_key.begin()); //������� �� ������� ����, ��� ������� �� ������� ������
        }

        std::string fast_lap = std::to_string(fastest_lap); //��������� �� 3 ����� ����� �������
        fast_lap = fast_lap.substr(0, fast_lap.size() - 3); //������� 3 ��������� ����� � ��� ����� ��� ������ ����� ��� 3 ��������� ������ ����� �������

        std::string line = std::to_string(id) + zap_prob(std::to_string(id)) + "|" + name + zap_prob(name) + "|" + std::to_string(group) + zap_prob(std::to_string(group)) + "|" \
            + name_coach + zap_prob(name_coach) + "|" + std::to_string(num_zan) + zap_prob(std::to_string(num_zan)) + "|" + fast_lap + zap_prob(fast_lap) + "\n";

        file << line;

        num_zap++;

        return true;
    }
    else {
        std::cout << "Error: One or more input data is longer than " << len_cell << " characters." << std::endl;
        return false;
    }
}

void Database::deleteByField(std::string value, int field) {
    if (field == 1) {
        std::string search_line = searchByKey(std::stoi(value), field);
        if (!StringToBool(search_line)) { return; }

        file.seekp(len_stroki * (std::stoi(value) - 1), std::ios::beg); //������ ������ �� ������ ������ ��� ������

        file << empty_str + '\n'; //�������� � ������ ������ �� �� �������

        std::vector<std::string> vec = splitString(search_line, '|');
        int id = std::stoi(vec[0]); //���� �� �������� �������� ��������
        del_key.push_back(id);

        num_zap--;
    }
    else {
        if (num_zap == 0) return; //���������, ����� � ����� �� ���� 0 �������

        std::vector<std::string> vec_res;
        std::string line;

        file.seekg(0, std::ios::beg);

        while (std::getline(file, line)) {
            if (line != empty_str) {
                std::vector<std::string> vec_str = splitString(line, delimiter);
                if (vec_str[field - 1] == value) {
                    vec_res.push_back(vec_str[0]); //��������� � ������ �����
                }
            }
        }

        file.clear();

        sort(vec_res.begin(), vec_res.end()); //���������, ����� ������� ���� �� �������

        for (std::string out : vec_res) {
            deleteByField(out, 1);
        }
    }
}

std::string Database::searchByKey(int value, int field) {
    if (value <= 0 || value > size_tab) {
        std::cout << "Incorrect input." << std::endl;
        return "0";
    }

    file.seekg(len_stroki * (value - 1), std::ios::beg); //������ ������ ��� ������ �� ������ ������ ��� ������

    std::string line;
    getline(file, line);

    if (line == empty_str) return "0"; //����� ������ ������ � ������� �������� ������ ������

    std::vector<std::string> vec = splitString(line, '|');

    if (vec[field - 1] == std::to_string(value)) { //���� ����� �����
        return line;
    }
    else return "0";
}

std::vector<std::string> Database::searchByNonKey(std::string value, int field) {
    std::vector<std::string> vec_res;

    std::string line;

    file.seekg(0, std::ios::beg);

    while (std::getline(file, line)) {
        if (line != empty_str) {
            std::vector<std::string> vec_str = splitString(line, delimiter);
            if (vec_str[field - 1] == value) {
                vec_res.push_back(line); //��������� � ������ ������ ��� ������
            }
        }
    }

    file.clear();

    sort(vec_res.begin(), vec_res.end()); //���������, ����� ������� ���� �� �������

    return vec_res;
}

bool Database::searchByField(std::string value, int field) {
    if ((field <= 0 || field > num_col)) { //�������� �� ��, ����� ����� �������� ��� ��������
        std::cout << "Incorrect input." << std::endl;
        return false;
    }

    if (field == 1) {
        std::string line = searchByKey(std::stoi(value), field);
        if (StringToBool(line)) {
            std::cout << "Find: " << line << std::endl;
            return true;
        }
        else {
            std::cout << "There is no such record in the Database." << std::endl;
            return false;
        }
    }
    else {
        if (num_zap == 0) return false; //���������, ����� � ����� �� ���� 0 �������

        std::vector<std::string> vec_res = searchByNonKey(value, field);

        std::cout << "Find: " << std::endl;
        for (std::string out : vec_res) {
            std::cout << "    " << out << std::endl;
        }
        std::cout << std::endl;

        return false;
    }
}

bool Database::editStudent(int id, const std::string& name, int group, const std::string& name_coach, int num_zan, float fastest_lap) {
    if (id <= 0 || id > size_tab) {
        std::cout << "Incorrect input." << std::endl;
        return false;
    }

    std::string str;
    file.seekg(len_stroki * (id - 1), std::ios::beg); //������ ������ ��� ������ �� ������ ������ ��� ������
    getline(file, str);

    if (str == empty_str) {
        std::cout << "There is no entry in the file for this key" << std::endl;
        return false;
    }

    bool flag = (name.length() <= len_cell) && (std::to_string(group).length() <= len_cell) && (name_coach.length() <= len_cell) && \
        (std::to_string(num_zan).length() <= len_cell) && (std::to_string(fastest_lap).length() <= len_cell); //�������� ����� ���� �������� �����, ����� � ������ �� ������

    if (flag) {
        file.seekp(len_stroki * (id - 1), std::ios::beg); //������ ������ ��� ������ �� ������ ������ ��� ������

        std::string fast_lap = std::to_string(fastest_lap); //��������� �� 3 ����� ����� �������
        fast_lap = fast_lap.substr(0, fast_lap.size() - 3); //������� 3 ��������� ����� � ��� ����� ��� ������ ����� ��� 3 ��������� ������ ����� �������


        std::string line = std::to_string(id) + zap_prob(std::to_string(id)) + "|" + name + zap_prob(name) + "|" + std::to_string(group) + zap_prob(std::to_string(group)) + "|" \
            + name_coach + zap_prob(name_coach) + "|" + std::to_string(num_zan) + zap_prob(std::to_string(num_zan)) + "|" + fast_lap + zap_prob(fast_lap) + "\n";

        file << line;
    }
    else {
        std::cout << "Error: One or more input data is longer than " << len_cell << " characters." << std::endl;
        return false;
    }
}

void Database::backup() {
    std::ofstream backup_file("backup_" + filename);
    if (!backup_file) {
        std::cout << "Error: Could not create backup file." << std::endl;
        return;
    }

    file.seekg(0, std::ios::beg);

    std::string line;

    while (getline(file, line)) { //���� �� ��������� ����� ����� ������ ��������� ������ � ���������� line
        backup_file << line << std::endl;
    }

    file.clear();

    std::cout << "Backup created successfully." << std::endl;
}

void Database::restore() {
    std::ifstream backup_file("backup_" + filename);
    this->clear();

    if (!backup_file) {
        std::cout << "Error: Could not restore database." << std::endl;
        return;
    }

    del_key.clear(); //����� ������� ������ � ���������� �������(�� ������ ����� � ��� ��� ������ ���� �� �������� ����� �� �����)
    num_zap = 0; //�������� ���-�� ������� � ���� ������
    size_tab = 0; //�������� ������ �������

    backup_file.seekg(0, std::ios::beg);

    std::string line;

    while (getline(backup_file, line)) { //���� �� ��������� ����� ����� ������ ��������� ������ � ���������� line
        file << line << std::endl;
        size_tab++;
        if (line == empty_str) {
            del_key.push_back(size_tab);
        }
        else {
            num_zap++;
        }
    }

    backup_file.clear();

    std::cout << "Restore database successfully." << std::endl;
}

std::vector<std::vector<std::string>> Database::returnStroki() {
    file.seekg(0, std::ios::beg);     // ���������� ��������� �� ������ �����

    std::string line;
    std::string empty_str(len_stroki - 2, ' '); //������ ��������� �� ��������(-2 ��-�� \n)

    std::vector<std::vector<std::string>> vec;

    while (getline(file, line)) { //���� �� ��������� ����� ����� ������ ��������� ������ � ���������� line
        if (line != empty_str) {
            vec.push_back(splitString(line, '|'));
        }
    }

    std::cout << std::endl;

    file.clear();                     // clear- ���������� ������ � ���, ��� ���� ���������� � ��-�� ����� �� ����� ������ �������� � ������(����� ������ �� ������ �����, ����� �������� � ��������� ���������. clear ���������� ������.)

    return vec;
}


void Database::exportToCSV(const std::string& exportFilename) {
    std::ofstream csvFile(exportFilename);
    if (!csvFile) {
        std::cout << "Error: Could not create export file " << exportFilename << std::endl;
        return;
    }

    file.seekg(0, std::ios::beg);  // ���������� ��������� ������ � ������ �����

    std::string line;
    std::vector<std::string> headers = { "ID", "Name", "Group", "Coach", "Lessons", "Fastest Lap" }; // ��������� ��������

    // ���������� ��������� � CSV
    for (size_t i = 0; i < headers.size(); ++i) {
        csvFile << headers[i];
        if (i != headers.size() - 1) {
            csvFile << ";";
        }
    }
    csvFile << "\n";

    // ������������ ������ ������ ���� ������
    while (std::getline(file, line)) {
        if (line != empty_str) {
            auto cells = splitString(line, delimiter);
            for (size_t i = 0; i < cells.size(); ++i) {
                csvFile << cells[i];
                if (i != cells.size() - 1) {
                    csvFile << ";";
                }
            }
            csvFile << "\n";
        }
    }

    file.clear();  // ���������� ���� ������
    csvFile.close();

    std::cout << "Database exported successfully to " << exportFilename << std::endl;
}