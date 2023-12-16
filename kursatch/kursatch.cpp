#include <iostream>
#include <filesystem>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <thread>


namespace fs = std::filesystem;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD coord;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
std::string destinationPath = "C:\\";
std::string CoppyPath;

void displayPath(const std::string& title, const fs::path& path, int count) {
    std::cout << "\033[0m";
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(hConsole, coord);
    //CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
    int columns = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    for (int i = 0; i < count+1; i++)
    {
        std::cout << std::string(columns, ' ') << std::endl;
    }
    SetConsoleCursorPosition(hConsole, coord);
    
    //system("cls");
    std::cout << title << ": " << path << std::endl;
}

//void filling() {
//
//}
//
//void checkWindowSize() {
//    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
//    DWORD mode = 0;   
//    GetConsoleMode(hStdin, &mode);
//    SetConsoleMode(hStdin, mode | ENABLE_WINDOW_INPUT);
//
//    INPUT_RECORD ir[1];
//    DWORD count;
//
//    while (true) {
//        ReadConsoleInput(hStdin, ir, 1, &count);
//        if (ir[0].EventType == WINDOW_BUFFER_SIZE_EVENT) {
//            for (auto& folder : _folders) {
//                folder.disp();
//            }
//        }
//    }
//}

void insert(const char* sourcePath, const char* destinationPath) {
    try {
        std::ifstream sourceFile(sourcePath, std::ios::binary);
        std::ofstream destinationFile(destinationPath, std::ios::binary);

        if (!sourceFile.is_open() || !destinationFile.is_open()) {
            throw std::runtime_error("Unable to open files!");
        }

        // Установка указателя чтения в начало файла
        sourceFile.seekg(0, std::ios::beg);

        // Получение размера файла
        sourceFile.seekg(0, std::ios::end);
        std::streampos fileSize = sourceFile.tellg();
        sourceFile.seekg(0, std::ios::beg);

        // Выделение буфера для копирования данных
        char* buffer = new (std::nothrow) char[fileSize];
        if (!buffer) {
            throw std::runtime_error("Memory allocation failed!");
        }

        // Чтение данных из исходного файла
        sourceFile.read(buffer, fileSize);
        if (sourceFile.gcount() != fileSize) {
            throw std::runtime_error("Error reading from source file!");
        }

        // Запись данных в целевой файл
        destinationFile.write(buffer, fileSize);
        if (!destinationFile) {
            throw std::runtime_error("Error writing to destination file!");
        }

        // Освобождение буфера
        delete[] buffer;

        // Закрытие файлов
        sourceFile.close();
        destinationFile.close();

        std::cout << "File copied successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}


//void copyFile(const char* sourcePath, const char* destinationPath) {
//    std::ifstream sourceFile(sourcePath, std::ios::binary);
//    std::ofstream destinationFile(destinationPath, std::ios::binary);
//
//    if (!sourceFile.is_open() || !destinationFile.is_open()) {
//        std::cerr << "Unable to open files!" << std::endl;
//        return;
//    }
//
//    // Установка указателя чтения в начало файла
//    sourceFile.seekg(0, std::ios::beg);
//
//    // Получение размера файла
//    sourceFile.seekg(0, std::ios::end);
//    std::streampos fileSize = sourceFile.tellg();
//    sourceFile.seekg(0, std::ios::beg);
//
//    // Выделение буфера для копирования данных
//    char* buffer = new char[fileSize];
//
//    // Чтение данных из исходного файла
//    sourceFile.read(buffer, fileSize);
//
//    // Запись данных в целевой файл
//    destinationFile.write(buffer, fileSize);
//
//    // Освобождение буфера
//    delete[] buffer;
//
//    // Закрытие файлов
//    sourceFile.close();
//    destinationFile.close();
//
//    std::cout << "File copied successfully!" << std::endl;
//}

class FileObject
{
public:
    FileObject(fs::path _name, bool _target, int _type, int _index); // Конструктор для fs::path
    //FileObject(std::string _name, bool _target, int _type); // Конструктор для std::string
    void switching_choice();
    void creating_an_output_line();
    void disp();
    //void disp(int _selectedFolderIndex);

    std::string out;
    fs::path name;
    std::string color;
    int type;
    bool target;
    int index;
};

FileObject::FileObject(fs::path _name, bool _target, int _type, int _index) {
    name = _name;
    index = _index;
    if (_type == 1) {
        color = "\033[1;36m";
    }
    else if (_type == 2) {
        color = "\033[1;34m";
    }
    else if (_type == 0) {
        color = "\033[1;31m";
    }
    color = color.erase(color.size() - 1) + (_target ? ";47m" : ";40m");

    /*color.erase(color.size() - 1) =+ (target ? ";47m" : ";40m");*/
    type = _type;
    target = _target;
    creating_an_output_line();
}

//FileObject::FileObject(std::string _name, bool _target, int _type) {
//    name = fs::path(_name); // Преобразуем строку в fs::path
//    if (_type == 1) {
//        color = "\033[1;36;40m";
//    }
//    else if (_type == 2) {
//        color = "\033[1;34;40m";
//    }
//    else if (_type == 0) {
//        color = "\033[1;31;40m";
//    }
//    color += (_target ? ";40" : ";47");
//    type = _type;
//    target = _target;
//}

void FileObject::switching_choice() {

    // Устанавливаем новый цвет фона в зависимости от target
    color = color.erase(color.size() - 4) + (target ? ";40m" : ";45m"); // Цвет фона - черный если target == true, иначе белый
    creating_an_output_line();
    disp();


    target = !target; // Переключаем значение target

}

void FileObject::creating_an_output_line() {
    out = color + name.string() + "\033[1;37;40m";
}

void FileObject::disp() {
    coord.X = 0;
    coord.Y = index+1;
    SetConsoleCursorPosition(hConsole, coord);
    //CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
    int columns = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    std::cout << std::string(columns, ' ');
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << out << std::endl;
}

//void FileObject::disp(int _selectedFolderIndex) {
//    coord.X = 0;
//    coord.Y = _selectedFolderIndex;
//    SetConsoleCursorPosition(hConsole, coord);
//    std::cout << out << std::endl;
//}

std::vector<FileObject> get_objects(std::string _patch) {
    std::vector<FileObject> folders;

    // Создаем объект FileObject для ".."
    FileObject parentFolder("..", true, 0, folders.size());

    folders.push_back(parentFolder);

    for (const auto& entry : fs::directory_iterator(_patch)) {
        if (fs::is_directory(entry)) {
            folders.push_back(FileObject(entry.path().filename(), false, 1, folders.size()));
        }
        else
        {
            folders.push_back(FileObject(entry.path().filename(), false, 2, folders.size()));
        }
    }
    return folders;
}


void copy(std::string _CoppyPath) {
    if (CoppyPath != _CoppyPath) {
        CoppyPath = _CoppyPath;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        coord.X = consoleInfo.dwCursorPosition.X;
        coord.Y = consoleInfo.dwSize.Y - 1;
        SetConsoleCursorPosition(hConsole, coord);

        std::cout << "------Скопированно------" << _CoppyPath 
            << std::string((consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1) - 24 - _CoppyPath.size(), ' ');
    }
}



int main() {
    
    
    //SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //std::string sourcePath = "C:/"; // Устанавливаем начальный путь как C:/
    
    //std::string dispout;
    displayPath("Destination", destinationPath,0);

    int selectedFolderIndex = 0; // Индекс выбранной папки


    //----------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------


    //  **СОЗДАТЬ СТРУКТУРУ КОТОРАЯ БУДЕТ ХРАНИТЬ [ТИП ФАЙЛА][?ПОЗИЦИЮ][fs::path][string dispout]   
    //  СДЕЛАТЬ ФУНКЦИИ ДЛЯ ВЫВОДА И ВООБШЕ ОБЛАГОРОДИТЬ КОД


    //----------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------
    std::vector<FileObject> folders = get_objects(destinationPath);
    //std::vector<FileObject> folders;

    //// Создаем объект FileObject для ".."
    //FileObject parentFolder("..", true, 0, folders.size());

    //folders.push_back(parentFolder);

    //for (const auto& entry : fs::directory_iterator(destinationPath)) {
    //    if (fs::is_directory(entry)) {
    //        folders.push_back(FileObject(entry.path().filename(), false, 1, folders.size()));
    //    }
    //    else
    //    {
    //        folders.push_back(FileObject(entry.path().filename(), false, 2, folders.size()));
    //    }
    //}



    //std::vector<fs::path> folders;
    //folders.push_back("..");
    //dispout = dispout + ".." + "\n";
    ////folders.push_back("\033[1;34;47m..\033[0m");

    //for (const auto& entry : fs::directory_iterator(destinationPath)) {
    //    if (fs::is_directory(entry)) {
    //        dispout += "Папка>> " + (entry.path().filename()).string() + "\n";
    //    }
    //    else
    //    {
    //        dispout += "Файл >> " + (entry.path().filename()).string() + "\n";
    //         
    //    }
    //    folders.push_back(entry.path().filename());
    //}

    for (auto& folder : folders) {
        folder.disp();
    }

    coord.X = 0;
    coord.Y = selectedFolderIndex + 1;
    SetConsoleCursorPosition(hConsole, coord);

    //std::thread t(checkWindowSize);

    while (true) {

        

        // Ввод клавиш
        char key = _getch();
        switch (key) {
        case 72: // Стрелка вверх
            if (selectedFolderIndex > 0) {
                folders[selectedFolderIndex].switching_choice();
                folders[--selectedFolderIndex].switching_choice();
                //folder[selectedFolderIndex].disp(selectedFolderIndex);
                //coord.X = 8;
                //SetConsoleCursorPosition(hConsole, coord);
                //std::cout << "\033[0m" << folders[selectedFolderIndex].string();
                //--selectedFolderIndex;
                //coord.Y = selectedFolderIndex + 1;
                //coord.X = 8;
                //SetConsoleCursorPosition(hConsole, coord);
                //std::cout << "\033[1;34;47m" << folders[selectedFolderIndex].string();
            }
            break;
        case 80: // Стрелка вниз
            if (selectedFolderIndex < static_cast<int>(folders.size()) - 1) {
                folders[selectedFolderIndex].switching_choice();
                folders[++selectedFolderIndex].switching_choice();
                //coord.X = 8;
                //SetConsoleCursorPosition(hConsole, coord);
                //std::cout << "\033[0m" << folders[selectedFolderIndex].string();
                //++selectedFolderIndex;
                //coord.Y = selectedFolderIndex + 1;
                //coord.X = 8;
                //SetConsoleCursorPosition(hConsole, coord);
                //std::cout << "\033[1;34;47m" << folders[selectedFolderIndex].string();

            }
            break;
        case 13: // Enter
            
            if (folders[selectedFolderIndex].name == "..") {
                displayPath("Destination", fs::path(destinationPath).parent_path().string(), folders.size());
                destinationPath = fs::path(destinationPath).parent_path().string();
                folders = get_objects(destinationPath);
                for (auto& folder : folders) {
                    folder.disp();
                }
            }
            else if(!fs::is_directory(destinationPath / folders[selectedFolderIndex].name)) {
                //folders = get_objects(destinationPath);
                copy((fs::path(destinationPath) / folders[selectedFolderIndex].name).string());
                //CoppyPath = (fs::path(destinationPath) / folders[selectedFolderIndex].name).string();
                /*CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
                GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
                coord.X = bufferInfo.dwSize.X - 1;
                coord.Y = bufferInfo.dwSize.Y - 2;
                SetConsoleCursorPosition(hConsole, coord);

                CoppyPath = (fs::path(destinationPath) / folders[selectedFolderIndex]).string();
                std::cout << "\n" << "\033[1;34;47m" << "------Скопированно------" << folders[selectedFolderIndex];

                coord.X = 8;
                coord.Y = selectedFolderIndex + 1;
                SetConsoleCursorPosition(hConsole, coord);
                break;*/
            }
            else {
                displayPath("Destination", (fs::path(destinationPath) / folders[selectedFolderIndex].name).string(), folders.size());
                destinationPath = (fs::path(destinationPath) / folders[selectedFolderIndex].name).string();
                selectedFolderIndex = 0;
                folders = get_objects(destinationPath);
                for (auto& folder : folders) {
                    folder.disp();
                }
            }
        //    displayPath("Destination", destinationPath);

        //    selectedFolderIndex = 0;
        //    coord.Y = selectedFolderIndex + 1;
        //    coord.X = 8;

        //    folders.clear();
        //    dispout = "";

        //    folders.push_back("..");
        //    dispout = dispout + ".." + "\n";
        //    //folders.push_back("\033[1;34;47m..\033[0m");

        //    for (const auto& entry : fs::directory_iterator(destinationPath)) {
        //        if (fs::is_directory(entry)) {
        //            dispout += "Папка>> \033[2;34;47m" + (entry.path().filename()).string() + "\033[0m\n";
        //        }
        //        else
        //        {
        //            dispout += "Файл >> \033[3;34;47m" + (entry.path().filename()).string() + "\033[0m\n";

        //        }
        //        folders.push_back(entry.path().filename());
        //    }

        //    std::cout << dispout;
        //    


        //    coord.X = 8;
        //    coord.Y = selectedFolderIndex + 1;
        //    SetConsoleCursorPosition(hConsole, coord);
        //    std::cout << "\033[1;34;47m" << folders[selectedFolderIndex].string();

        //    

            break;
        case 'c': // копировать
        case 'C': // копировать
        case 'с': // копировать
        case 'С': // копировать
            if (folders[selectedFolderIndex].name != "..") {
                copy((fs::path(destinationPath) / folders[selectedFolderIndex].name).string());
            }
            break;
        case 'v': //  вставить
        case 'V': //  вставить
        case 'м': //  вставить
        case 'М': //  вставить
            //if (folders[selectedFolderIndex].name != "..") {
                insert(CoppyPath.c_str(), (fs::path(destinationPath)).string().c_str());
            //}
            break;
        case 'q': // Для выхода из программы
            return 0;
        default:
            break;
        }
    }

    return 0;
}






//#include <string>
//#include <iostream>
//#include <filesystem>
//#include <fstream>
//#include <Windows.h>
//
//using namespace std;
//
//void copyFile(const std::filesystem::path& source, const std::filesystem::path& destination) {
//    std::ifstream src(source, std::ios::binary);
//    std::ofstream dst(destination, std::ios::binary);
//    dst << src.rdbuf();
//}
//
//int main() {
//    setlocale(LC_ALL, "Russian");
//    SetConsoleCP(1251);
//    SetConsoleOutputCP(1251);
//    std::string path;
//    while (true) {
//        std::cout << "Введите путь: ";
//        getline(cin, path);
//
//        std::filesystem::path p(path);
//
//        if (std::filesystem::exists(p)) {
//            if (std::filesystem::is_directory(p)) {
//                std::cout << "Содержимое папки:\n";
//                for (const auto& entry : std::filesystem::directory_iterator(p))
//                    std::cout << entry.path() << '\n';
//            }
//            else if (std::filesystem::is_regular_file(p)) {
//                std::string destination;
//                std::cout << "Введите путь для копирования файла: ";
//                std::cin >> destination;
//                copyFile(p, destination);
//                std::cout << "Файл скопирован.\n";
//                break;
//            }
//        }
//        else {
//            std::cout << "Путь" << path << " не существует.\n";
//        }
//    }
//    return 0;
//}


