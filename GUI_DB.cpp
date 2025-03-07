#include <wx/wx.h>
#include <wx/grid.h>
#include <string>
#include <vector>
#include "Database.h"

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, Database* db);
    bool tableCreated = false;

private:
    Database* db;                 // Указатель на объект базы данных
    wxGrid* grid;                 // Таблица для отображения данных
    wxTextCtrl* valueInput;       // Поле для ввода значения
    wxTextCtrl* fieldInput;       // Поле для ввода номера столбца
    wxTextCtrl* idInput;          // Поле ввода ID студента
    wxTextCtrl* nameInput;        // Поле ввода имени студента
    wxTextCtrl* groupInput;       // Поле ввода группы студента
    wxTextCtrl* coachInput;       // Поле ввода имени тренера
    wxTextCtrl* sessionsInput;    // Поле ввода числа занятий
    wxTextCtrl* lapInput;         // Поле ввода лучшего времени круга

    void InitializeTable();
    void OnCreate(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnRemove(wxCommandEvent& event);
    void OnAddStudent(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnSearch(wxCommandEvent& event);
    void OnEditStudent(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnBackup(wxCommandEvent& event);
    void OnRestore(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnExportToCSV(wxCommandEvent& event);
    void Update();
};

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

MyFrame::MyFrame(const wxString& title, Database* db)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 600)), db(db) {
    // Основной вертикальный слой
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Горизонтальный слой для кнопок
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Создание кнопок
    wxButton* btnCreate = new wxButton(this, wxID_ANY, "Create");
    wxButton* btnOpen = new wxButton(this, wxID_ANY, "Open");
    wxButton* btnSave = new wxButton(this, wxID_ANY, "Save");
    wxButton* btnRemove = new wxButton(this, wxID_ANY, "Remove");
    wxButton* btnAddStudent = new wxButton(this, wxID_ANY, "Add Student");
    wxButton* btnDelete = new wxButton(this, wxID_ANY, "Delete");
    wxButton* btnSearch = new wxButton(this, wxID_ANY, "Search");
    wxButton* btnEditStudent = new wxButton(this, wxID_ANY, "Edit");
    wxButton* btnClear = new wxButton(this, wxID_ANY, "Clear");
    wxButton* btnBackup = new wxButton(this, wxID_ANY, "Backup");
    wxButton* btnRestore = new wxButton(this, wxID_ANY, "Restore");
    wxButton* btnExportToCSV = new wxButton(this, wxID_ANY, "Export to CSV");

    //valueInput = new wxTextCtrl(this, wxID_ANY);
    //fieldInput = new wxTextCtrl(this, wxID_ANY);

    // Добавление кнопок в горизонтальный слой
    buttonSizer->Add(btnCreate, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnOpen, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnSave, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnRemove, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnAddStudent, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnDelete, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnSearch, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnEditStudent, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnClear, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnBackup, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnRestore, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(btnExportToCSV, 1, wxEXPAND | wxALL, 5);

    // Таблица
    grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxSize(800, 300));
    InitializeTable();

    // Добавление кнопок и таблицы в основной слой
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5); // Кнопки
    mainSizer->Add(grid, 1, wxEXPAND | wxALL, 5);       // Таблица

    SetSizer(mainSizer);

    // Привязка событий
    btnCreate->Bind(wxEVT_BUTTON, &MyFrame::OnCreate, this);
    btnOpen->Bind(wxEVT_BUTTON, &MyFrame::OnOpen, this);
    btnSave->Bind(wxEVT_BUTTON, &MyFrame::OnSave, this);
    btnRemove->Bind(wxEVT_BUTTON, &MyFrame::OnRemove, this);
    btnAddStudent->Bind(wxEVT_BUTTON, &MyFrame::OnAddStudent, this);
    btnDelete->Bind(wxEVT_BUTTON, &MyFrame::OnDelete, this);
    btnSearch->Bind(wxEVT_BUTTON, &MyFrame::OnSearch, this);
    btnEditStudent->Bind(wxEVT_BUTTON, &MyFrame::OnEditStudent, this);
    btnClear->Bind(wxEVT_BUTTON, &MyFrame::OnClear, this);
    btnBackup->Bind(wxEVT_BUTTON, &MyFrame::OnBackup, this);
    btnRestore->Bind(wxEVT_BUTTON, &MyFrame::OnRestore, this);
    btnExportToCSV->Bind(wxEVT_BUTTON, &MyFrame::OnExportToCSV, this);
}

void MyFrame::InitializeTable() {
    setlocale(LC_ALL, "RUS"); //С помощью этого записываем вещ числа не через точку, а через запятую и можем работать с русским алфавитом
    // Проверяем, была ли уже создана таблица
    if (!tableCreated) {
        grid->CreateGrid(100, 6); // Создаем таблицу с 6 столбцами

        // Устанавливаем заголовки столбцов
        grid->SetColLabelValue(0, "ID");
        grid->SetColLabelValue(1, "Student name");
        grid->SetColLabelValue(2, "Group №");
        grid->SetColLabelValue(3, "Couch name");
        grid->SetColLabelValue(4, "Number of classes");
        grid->SetColLabelValue(5, "Fastest lap");

        // Устанавливаем размеры столбцов
        grid->SetColSize(0, 150);
        grid->SetColSize(1, 150);
        grid->SetColSize(2, 150);
        grid->SetColSize(3, 150);
        grid->SetColSize(4, 150);
        grid->SetColSize(5, 150);

        tableCreated = true;  // Устанавливаем флаг, что таблица создана
    }
}


void MyFrame::OnCreate(wxCommandEvent&) {
    try {
        std::string userInput;

        wxTextEntryDialog dialog(this, "Enter filename:", "Create", "");
        if (dialog.ShowModal() == wxID_OK)
        {
            userInput = dialog.GetValue().ToStdString();
        }

        // Вызываем метод create из Database
        db->create(userInput);

        // Проверяем, успешно ли файл был создан
        wxMessageBox("Database created successfully!", "Success", wxOK | wxICON_INFORMATION);

        // Инициализируем таблицу в интерфейсе
        if (grid->GetTable()) {
            grid->ClearGrid();  // Очищаем текущую таблицу, если она есть
            if (grid->GetNumberRows()) { grid->DeleteRows(0, grid->GetNumberRows()); }  // Удаляем все строки
        }
        InitializeTable();

    }
    catch (const std::exception& ex) {
        wxMessageBox(wxString::Format("An error occurred: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::OnOpen(wxCommandEvent&) {
    try {
        std::string userInput;

        wxTextEntryDialog dialog(this, "Enter filename:", "Open", "");
        if (dialog.ShowModal() == wxID_OK)
        {
            userInput = dialog.GetValue().ToStdString();
        }

        // Вызываем метод open из Database
        if (db->open(userInput)) {
            wxMessageBox("Database opened successfully!", "Success", wxOK | wxICON_INFORMATION);

            Update();

            // Если таблица открыта, обновляем интерфейс
            wxMessageBox("Database is ready for operations.", "Info", wxOK | wxICON_INFORMATION);
        }
        else {
            wxMessageBox("Failed to open the database. Please check the file.", "Error", wxOK | wxICON_ERROR);
        }
    }
    catch (const std::exception& ex) {
        wxMessageBox(wxString::Format("An error occurred: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}


void MyFrame::OnRemove(wxCommandEvent& event) {
    try {
        // Вызываем метод remove из Database для удаления базы данных
        db->remove();

        // Уведомляем пользователя о результате
        wxMessageBox("Database file has been deleted.", "Success", wxOK | wxICON_INFORMATION);

        // После удаления базы данных, очищаем или скрываем таблицу
        if (grid->GetTable()) {
            grid->ClearGrid(); // Очищаем данные в таблице
            if (grid->GetNumberRows()) { grid->DeleteRows(0, grid->GetNumberRows()); }  // Удаляем все строки
        }

        // Переинициализируем таблицу, если это необходимо (например, когда база данных пуста)
        InitializeTable();  // Это обновит таблицу, например, если ее нужно восстановить с пустыми данными
    }
    catch (const std::exception& ex) {
        wxMessageBox(wxString::Format("An error occurred: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::OnAddStudent(wxCommandEvent&) {
    // Диалог для ввода имени студента
    wxTextEntryDialog nameDialog(this, "Enter student's name:", "Add a student");
    if (nameDialog.ShowModal() != wxID_OK) return;
    wxString name = nameDialog.GetValue().Trim();

    // Диалог для ввода группы
    wxTextEntryDialog groupDialog(this, "Enter the group number (integer):", "Add student");
    if (groupDialog.ShowModal() != wxID_OK) return;
    long group;
    if (!groupDialog.GetValue().ToLong(&group)) {
        wxMessageBox("Group number must be an integer.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Диалог для ввода имени тренера
    wxTextEntryDialog coachDialog(this, "Enter coach's name:", "Add a student");
    if (coachDialog.ShowModal() != wxID_OK) return;
    wxString coachName = coachDialog.GetValue().Trim();

    // Диалог для ввода количества занятий
    wxTextEntryDialog numZanDialog(this, "Enter number of classes (integer):", "Add a student");
    if (numZanDialog.ShowModal() != wxID_OK) return;
    long numZan;
    if (!numZanDialog.GetValue().ToLong(&numZan)) {
        wxMessageBox("Number of classes must be an integer.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Диалог для ввода лучшего времени круга
    wxTextEntryDialog lapTimeDialog(this, "Enter the best lap time (fractional number):", "Add a student");
    if (lapTimeDialog.ShowModal() != wxID_OK) return;
    double fastestLap;
    if (!lapTimeDialog.GetValue().ToDouble(&fastestLap)) {
        wxMessageBox("The best lap time should be a fractional number.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Вызов функции addStudent
    if (db->addStudent(std::string(name.mb_str()), group, std::string(coachName.mb_str()), numZan, static_cast<float>(fastestLap))) {
        Update();
        wxMessageBox("The student has been successfully added!", "Success", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Error adding a student. Check the length of the data.", "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::OnSave(wxCommandEvent& event) {
    try {
        // Вызываем метод save из Database для сохранения базы данных
        db->save();

        // Показываем сообщение о успешном сохранении
        wxMessageBox("Database saved successfully.", "Success", wxOK | wxICON_INFORMATION);
    }
    catch (const std::exception& ex) {
        // В случае ошибки выводим сообщение с описанием ошибки
        wxMessageBox(wxString::Format("An error occurred: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::OnDelete(wxCommandEvent&) {
    wxString value;
    wxString fieldStr;

    wxTextEntryDialog dialogField(this, "In which column should delete:", "Column", "");
    if (dialogField.ShowModal() == wxID_OK)
    {
        fieldStr = dialogField.GetValue();
    }

    wxTextEntryDialog dialogValue(this, "What needs to be deleted:", "Delete", "");
    if (dialogValue.ShowModal() == wxID_OK)
    {
        value = dialogValue.GetValue();
    }

    // Проверяем корректность ввода
    if (value.IsEmpty() || fieldStr.IsEmpty()) {
        wxMessageBox("Please enter both value and field number!", "Error", wxOK | wxICON_ERROR);
        return;
    }

    int field;
    if (!fieldStr.ToInt(&field) || field <= 0) {
        wxMessageBox("Invalid field number! Please enter a positive integer.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Вызываем функцию deleteByField
    try {
        db->deleteByField(value.ToStdString(), field);
        Update();
        wxMessageBox("Record deleted successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
    catch (const std::exception& ex) {
        wxMessageBox(wxString::Format("Error: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::OnSearch(wxCommandEvent&) {
    wxString value;
    wxString fieldStr;

    wxTextEntryDialog dialogField(this, "In which column should search:", "Column", "");
    if (dialogField.ShowModal() == wxID_OK)
    {
        fieldStr = dialogField.GetValue();
    }

    wxTextEntryDialog dialogValue(this, "What needs to be searched:", "Search", "");
    if (dialogValue.ShowModal() == wxID_OK)
    {
        value = dialogValue.GetValue();
    }
    // Проверяем корректность ввода
    if (value.IsEmpty() || fieldStr.IsEmpty()) {
        wxMessageBox("Please enter both value and field number!", "Error", wxOK | wxICON_ERROR);
        return;
    }

    int field;
    if (!fieldStr.ToInt(&field) || field <= 0) {
        wxMessageBox("Invalid field number! Please enter a positive integer.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Выполняем поиск
    try {
        if (field == 1) { // Поиск по ключу
            std::string result = db->searchByKey(std::stoi(value.ToStdString()), field);
            if (result == "0") {
                wxMessageBox("No record found with the given key.", "Search Result", wxOK | wxICON_INFORMATION);
            }
            else {
                wxMessageBox(wxString::Format("Record found:\n%s", result), "Search Result", wxOK | wxICON_INFORMATION);
            }
        }
        else { // Поиск по столбцу
            std::vector<std::string> vec_res = db->searchByNonKey(value.ToStdString(), field);
            if (vec_res.size() == 0) wxMessageBox("No records found in the specified field.", "Search Result", wxOK | wxICON_INFORMATION);
            else {
                int count = 0;
                for (std::string out : vec_res) {
                    count++;
                    wxMessageBox(wxString::Format("Record found:\n%s", out), wxString::Format("Search Result %i/%i", count, vec_res.size()), wxOK | wxICON_INFORMATION);
                }
            }
        }
    }
    catch (const std::exception& ex) {
        wxMessageBox(wxString::Format("Error: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::OnEditStudent(wxCommandEvent&) {
    wxString idStr;
    wxString name;
    wxString groupStr;
    wxString coach;
    wxString sessionsStr;
    wxString lapStr;

    wxTextEntryDialog dialogidStr(this, "By which Id to change the record:", "ID", "");
    if (dialogidStr.ShowModal() == wxID_OK)
    {
        idStr = dialogidStr.GetValue();
    }

    wxTextEntryDialog dialogname(this, "Edit name to:", "Name", "");
    if (dialogname.ShowModal() == wxID_OK)
    {
        name = dialogname.GetValue();
    }

    wxTextEntryDialog dialoggroupStr(this, "Edit group to:", "Group", "");
    if (dialoggroupStr.ShowModal() == wxID_OK)
    {
        groupStr = dialoggroupStr.GetValue();
    }

    wxTextEntryDialog dialogcoach(this, "Edit coach name to:", "Coach name", "");
    if (dialogcoach.ShowModal() == wxID_OK)
    {
        coach = dialogcoach.GetValue();
    }

    wxTextEntryDialog dialogsessionsStr(this, "Edit number of classes to:", "Number of classes", "");
    if (dialogsessionsStr.ShowModal() == wxID_OK)
    {
        sessionsStr = dialogsessionsStr.GetValue();
    }

    wxTextEntryDialog dialoglapStr(this, "Edit fastest lap to:", "Fastest Lap", "");
    if (dialoglapStr.ShowModal() == wxID_OK)
    {
        lapStr = dialoglapStr.GetValue();
    }

    // Проверяем корректность данных
    if (idStr.IsEmpty() || name.IsEmpty() || groupStr.IsEmpty() || coach.IsEmpty() || sessionsStr.IsEmpty() || lapStr.IsEmpty()) {
        wxMessageBox("All fields are required!", "Error", wxOK | wxICON_ERROR);
        return;
    }

    int id, group, sessions;
    double fastestLap;

    if (!idStr.ToInt(&id) || id <= 0) {
        wxMessageBox("Invalid ID! Please enter a positive integer.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (!groupStr.ToInt(&group) || group <= 0) {
        wxMessageBox("Invalid Group! Please enter a positive integer.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (!sessionsStr.ToInt(&sessions) || sessions <= 0) {
        wxMessageBox("Invalid Number of Sessions! Please enter a positive integer.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (!lapStr.ToDouble(&fastestLap) || fastestLap <= 0) {
        wxMessageBox("Invalid Fastest Lap Time! Please enter a positive number.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Вызываем метод editStudent
    try {
        bool success = db->editStudent(id, name.ToStdString(), group, coach.ToStdString(), sessions, static_cast<float>(fastestLap));
        Update();

        if (success) {
            wxMessageBox("Student record updated successfully!", "Success", wxOK | wxICON_INFORMATION);
        }
        else {
            wxMessageBox("Failed to update student record. Please check the input data.", "Error", wxOK | wxICON_ERROR);
        }
    }
    catch (const std::exception& ex) {
        wxMessageBox(wxString::Format("Error: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::OnClear(wxCommandEvent&) {
    db->clear();
    grid->ClearGrid();
    while (grid->GetNumberRows() > 0) {
        grid->DeleteRows(0);
    }
}

void MyFrame::OnBackup(wxCommandEvent& event) {
    try {
        
        db->backup();

        // Показываем сообщение о успешном создании резервной копии
        wxMessageBox("Backup created successfully.", "Success", wxOK | wxICON_INFORMATION);
    }
    catch (const std::exception& ex) {
        // В случае ошибки выводим сообщение с описанием ошибки
        wxMessageBox(wxString::Format("An error occurred: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::OnRestore(wxCommandEvent& event) {
    try {
       
        db->restore();

        Update();

        wxMessageBox("Database restored successfully.", "Success", wxOK | wxICON_INFORMATION);
    }
    catch (const std::exception& ex) {
        
        wxMessageBox(wxString::Format("An error occurred: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::Update() {
    grid->ClearGrid();  // Очищаем таблицу
    if (grid->GetNumberRows()) { grid->DeleteRows(0, grid->GetNumberRows()); }  // Удаляем все строки

    std::vector<std::vector<std::string>> vec = db->returnStroki();

    // Устанавливаем количество строк в зависимости от размера вектора
    grid->AppendRows(vec.size());

    // Обновляем таблицу
    grid->Refresh();

    for (int row = 0; row < vec.size(); ++row)
    {
        for (int col = 0; col < vec[row].size(); ++col)
        {
            grid->SetCellValue(row, col, vec[row][col]);
        }
    }
}

void MyFrame::OnExportToCSV(wxCommandEvent& event) {
    // Открываем диалоговое окно для выбора пути сохранения
    wxFileDialog saveFileDialog(this, _("Save CSV file"), "", "",
        "CSV files (*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        wxMessageBox("Export canceled.", "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    // Получаем путь к выбранному файлу
    std::string exportFilePath = saveFileDialog.GetPath().ToStdString();

    // Вызываем метод exportToCSV из объекта Database
    if (!db) {
        wxMessageBox("Error: No database is open.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    db->exportToCSV(exportFilePath);

    wxMessageBox("Database exported successfully to:\n" + saveFileDialog.GetPath(),
        "Export Complete", wxOK | wxICON_INFORMATION);
}

bool MyApp::OnInit() {
    try {
        
        Database* db = new Database; // Передача имени файла в конструктор
        MyFrame* frame = new MyFrame("Database GUI", db);
        frame->Show(true);
    }
    catch (const std::exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
        return false;
    }
    return true;
}

wxIMPLEMENT_APP(MyApp);
