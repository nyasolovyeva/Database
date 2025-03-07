#include "DataBase.h"
#include <codecvt>

int num_col = 6; //Количество столбиков
int len_cell = 15; //Длина ячейки
long len_stroki = len_cell * num_col + 7; //Длина строчки(5- разделители "|", 2- "\n")
std::string empty_str(len_stroki - 2, ' '); //Строка состоящая из пробелов(Пустая строка в таблице)(-2 из-за \n)
char delimiter = '|'; //Разделитель между ячейками в таблице

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    std::istringstream iss(str);

    while (std::getline(iss, token, delimiter)) {
        while (token.length() != 0 && std::isspace(token.back())) token.pop_back(); //Удаляем пробелы в конце ячейки
        result.push_back(token);
    }
    return result;
}

std::string Database::getFullLine(std::fstream& inFile, int value) {
    inFile.seekg(len_stroki * (value - 1), std::ios::beg); //Ставим курсор для чтения на начало нужной нам строки

    std::string line;
    getline(inFile, line);

    inFile.clear(); //Иначе будет выставлен флаг, блокирующий чтение

    return line;
}

std::string zap_prob(const std::string& stroka) { //Заполнение ячейки пробелами до длины len_cell
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
        del_key.clear(); //Нужно удалить вектор с удаленными ключами (На случай, когда у нас был открыт файл до создания этого же файла)
        num_zap = 0; //Обнуляем кол-во записей в базе данных
        size_tab = 0; //Обнуляем размер таблицы

        filename = _filename;
    }
    if (file.is_open()) { file.close(); } //Если файл был открыт, то закрыть
    file.open(filename, std::ios::in | std::ios::out); //Если будем использовать ios::trunc, то удалим то, что было в файле до этого, а если ios::app, то указатель будет в конце файла и переместить на записанные раннее строчки не получится

    if (!file) {
        std::cout << "Error: Could not open database file " << filename << std::endl;
        return false;
    }

    file.seekg(0, std::ios::beg);

    std::string line;

    while (getline(file, line)) { //Пока не достигнут конец файла класть очередную строку в переменную line
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
    file.close(); //Если не закрыть файл, то не сможем удалить
    if (std::remove(filename.c_str()) == 0) {
        std::cout << "Database deleted: " << filename << std::endl;
    }
    else {
        std::cout << "Error: Could not delete database file " << filename << std::endl;
    }
}

void Database::clear() {
    if (file.is_open()) { file.close(); } //Если файл был открыт, то закрыть

    //Открываем файл в режиме trunc(чтобы удалить то, что в нём было)
    file.open(filename, std::ios::in | std::ios::out | std::ofstream::trunc); //in- чтение из файла, out- запись в файл, trunc- открытие файла с удалением того, что было в нём

    del_key.clear(); //Нужно удалить вектор с удаленными ключами (На случай, когда у нас был открыт файл до создания этого же файла)
    num_zap = 0; //Обнуляем кол-во записей в базе данных
    size_tab = 0; //Обнуляем размер таблицы

    std::cout << "Database cleared." << std::endl;
}

void Database::save() {
    CreateDirectory(L"./saves", NULL); //.- текущая директория
    std::fstream save_file("./saves/" + filename, std::ios::in | std::ios::out | std::ofstream::trunc);
    if (!save_file) {
        std::cout << "Error: Could not save database file " << filename << std::endl;
        return;
    }

    file.seekg(0, std::ios::beg);

    std::string line;

    while (getline(file, line)) { //Пока не достигнут конец файла класть очередную строку в переменную line
        save_file << line << std::endl;
    }

    file.clear(); //После полного прочтения файла выставится флаг(который запрещает дальнейшее чтение или запись) для file, что файл закончился(Этой функцией снимаем флаг для того, чтобы мы могли дальше читать или записывать в файл)

    std::cout << "Database saved successfully." << std::endl;
}

bool Database::addStudent(const std::string& name, int group, const std::string& name_coach, int num_zan, float fastest_lap) {
    bool flag = (name.length() <= len_cell) && (std::to_string(group).length() <= len_cell) && (name_coach.length() <= len_cell) && \
        (std::to_string(num_zan).length() <= len_cell) && (std::to_string(fastest_lap).length() <= len_cell); //Проверка длины всех введённых строк, чтобы в ячейки всё влезло

    if (flag) {
        int id;

        if (del_key.size() == 0) {
            id = num_zap + 1;
            file.seekp(0, std::ios::end); //Ставим курсор для записи в конец файла
            size_tab++;
        }
        else {
            id = del_key[0]; //Достаем первый ключ из вектора
            file.seekp(len_stroki * (id - 1), std::ios::beg); //Ставим курсор для записи на начало нужной нам строки
            del_key.erase(del_key.begin()); //Удалить из вектора ключ, под который мы вставим запись
        }

        std::string fast_lap = std::to_string(fastest_lap); //Округлить до 3 знака после запятой
        fast_lap = fast_lap.substr(0, fast_lap.size() - 3); //Удалить 3 последних числа в вещ числе для вывода числа без 3 последних знаков после запятой

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

        file.seekp(len_stroki * (std::stoi(value) - 1), std::ios::beg); //Ставим курсор на начало нужной нам строки

        file << empty_str + '\n'; //Заменяем в нужной строке всё на пробелы

        std::vector<std::string> vec = splitString(search_line, '|');
        int id = std::stoi(vec[0]); //Ключ по которому проходит удаление
        del_key.push_back(id);

        num_zap--;
    }
    else {
        if (num_zap == 0) return; //Проверяем, чтобы в файле не было 0 записей

        std::vector<std::string> vec_res;
        std::string line;

        file.seekg(0, std::ios::beg);

        while (std::getline(file, line)) {
            if (line != empty_str) {
                std::vector<std::string> vec_str = splitString(line, delimiter);
                if (vec_str[field - 1] == value) {
                    vec_res.push_back(vec_str[0]); //Добавляем в вектор ключи
                }
            }
        }

        file.clear();

        sort(vec_res.begin(), vec_res.end()); //Сортируем, чтобы вывести всех по порядку

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

    file.seekg(len_stroki * (value - 1), std::ios::beg); //Ставим курсор для чтения на начало нужной нам строки

    std::string line;
    getline(file, line);

    if (line == empty_str) return "0"; //Когда вместо строки с данными получили пустую строку

    std::vector<std::string> vec = splitString(line, '|');

    if (vec[field - 1] == std::to_string(value)) { //Если ключи равны
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
                vec_res.push_back(line); //Добавляем в вектор строки для вывода
            }
        }
    }

    file.clear();

    sort(vec_res.begin(), vec_res.end()); //Сортируем, чтобы вывести всех по порядку

    return vec_res;
}

bool Database::searchByField(std::string value, int field) {
    if ((field <= 0 || field > num_col)) { //Проверка на то, чтобы номер столбика был реальным
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
        if (num_zap == 0) return false; //Проверяем, чтобы в файле не было 0 записей

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
    file.seekg(len_stroki * (id - 1), std::ios::beg); //Ставим курсор для чтения на начало нужной нам строки
    getline(file, str);

    if (str == empty_str) {
        std::cout << "There is no entry in the file for this key" << std::endl;
        return false;
    }

    bool flag = (name.length() <= len_cell) && (std::to_string(group).length() <= len_cell) && (name_coach.length() <= len_cell) && \
        (std::to_string(num_zan).length() <= len_cell) && (std::to_string(fastest_lap).length() <= len_cell); //Проверка длины всех введённых строк, чтобы в ячейки всё влезло

    if (flag) {
        file.seekp(len_stroki * (id - 1), std::ios::beg); //Ставим курсор для записи на начало нужной нам строки

        std::string fast_lap = std::to_string(fastest_lap); //Округлить до 3 знака после запятой
        fast_lap = fast_lap.substr(0, fast_lap.size() - 3); //Удалить 3 последних числа в вещ числе для вывода числа без 3 последних знаков после запятой


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

    while (getline(file, line)) { //Пока не достигнут конец файла класть очередную строку в переменную line
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

    del_key.clear(); //Нужно удалить вектор с удаленными ключами(На случай когда у нас был открыт файл до создания этого же файла)
    num_zap = 0; //Обнуляем кол-во записей в базе данных
    size_tab = 0; //Обнуляем размер таблицы

    backup_file.seekg(0, std::ios::beg);

    std::string line;

    while (getline(backup_file, line)) { //Пока не достигнут конец файла класть очередную строку в переменную line
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
    file.seekg(0, std::ios::beg);     // Сбрасываем указатели на начало файла

    std::string line;
    std::string empty_str(len_stroki - 2, ' '); //Строка состоящая из пробелов(-2 из-за \n)

    std::vector<std::vector<std::string>> vec;

    while (getline(file, line)) { //Пока не достигнут конец файла класть очередную строку в переменную line
        if (line != empty_str) {
            vec.push_back(splitString(line, '|'));
        }
    }

    std::cout << std::endl;

    file.clear();                     // clear- сбрасывает ошибку о том, что файл закончился и из-за этого мы можем дальше работать с файлом(После чтения за концом файла, поток перейдет в ошибочное состояние. clear сбрасывает ошибки.)

    return vec;
}


void Database::exportToCSV(const std::string& exportFilename) {
    std::ofstream csvFile(exportFilename);
    if (!csvFile) {
        std::cout << "Error: Could not create export file " << exportFilename << std::endl;
        return;
    }

    file.seekg(0, std::ios::beg);  // Установить указатель чтения в начало файла

    std::string line;
    std::vector<std::string> headers = { "ID", "Name", "Group", "Coach", "Lessons", "Fastest Lap" }; // Заголовки столбцов

    // Записываем заголовки в CSV
    for (size_t i = 0; i < headers.size(); ++i) {
        csvFile << headers[i];
        if (i != headers.size() - 1) {
            csvFile << ";";
        }
    }
    csvFile << "\n";

    // Экспортируем каждую строку базы данных
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

    file.clear();  // Сбрасываем флаг ошибок
    csvFile.close();

    std::cout << "Database exported successfully to " << exportFilename << std::endl;
}