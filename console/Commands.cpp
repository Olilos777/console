#include "Commands.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <iomanip>
#include <sstream>
#include <conio.h>

int Commands::n = 0;

std::string Commands::getCurrentDir() {
    return fs::current_path().string();
}

std::string Commands::getLine() {
    std::string input;
    static int i = 0;
    while (true) {
        if (_kbhit()) { // Sprawdü, czy jest dostÍpne wejúcie z klawiatury
            int ch = _getch(); // Odczytaj pojedynczy znak
            if (ch == '\r') { // Sprawdü, czy to klawisz Enter
                std::cout << std::endl; // Przejúcie do nowej linii
                return input; // ZwrÛÊ wprowadzony tekst
            }
            else if (ch == '\b') { // Sprawdü, czy to klawisz Backspace
                if (!input.empty()) {
                    input.pop_back(); // UsuÒ ostatni wpisany znak
                    std::cout << "\b \b"; // UsuÒ znak z wyúwietlanego tekstu
                }
            }
            else if (ch == '\t') {
                //std::vector<std::string> listFiles = listDir(getCurrentDir());
                //for (int i = 0; i <= listFiles.size(); i++) {
                    /*std::cout << listFiles[i];
                    i++;
                    if (i <= listFiles.size() - 1)
                        i = 0;*/
                //}
                // Logika autouzupe≥niania przy uøyciu klawisza Tab
                std::vector<std::string> files = listDir(getCurrentDir(), true);
                //std::cout << files.size();
                autoComplete(input, files);

            }
            else {
                input += static_cast<char>(ch); // Dodaj znak do aktualnego wprowadzenia
                std::cout << static_cast<char>(ch); // Wyúwietl wprowadzony znak
            }
        }
    }
}

std::vector<std::string> Commands::listDir(std::string path, bool returnFlag) {
    std::vector<std::string> output;
    try {
        int i = 0;
        for (const auto& entry : fs::directory_iterator(path)) {
            if (returnFlag == false) {
                std::cout << entry.path().filename() << "   " << returnSize(entry.path()) << std::endl;
            }
            output.push_back(entry.path().filename().string());
        }
    }
    catch (const fs::filesystem_error& ex) {
        std::cerr << "B≥πd: " << ex.what() << std::endl;
    }
    return output;
}

void Commands::changeDir(std::string newPath) {
    // Zmiana bieøπcego katalogu na nowπ úcieøkÍ
    try {
        if (newPath == "..") {
            std::string inputPath = getCurrentDir();
            size_t pos = inputPath.find_last_of("\\");
            if (pos != std::string::npos) {
                inputPath = inputPath.substr(0, pos);
                fs::current_path(inputPath);
            }
            else {
                std::cout << "Error\n";
            }
        }
        else {
            fs::path old(getCurrentDir());
            fs::path nextFolder(newPath);

            fs::path newpath = old / newPath;
            fs::current_path(newpath);
        }
    }
    catch (const fs::filesystem_error& ex) {
        std::cerr << "B≥πd: " << ex.what() << std::endl;
    }
    //std::cout << "par: " << path;
}

std::string Commands::returnSize(const fs::path& path) {
    double bytes = static_cast<double>(fs::file_size(path));
    std::string out;

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2);

    if (bytes < 1024.0) {
        stream << bytes << " bytes";
        out = stream.str();
    }
    else if (bytes < 1048576.0) {
        stream << bytes / 1024.0 << " KB";
        out = stream.str();
    }
    else {
        stream << bytes / 1048576.0 << " MB";
        out = stream.str();
    }

    return out;
}

void Commands::autoComplete(const std::string& input, const std::vector<std::string>& options) {
    if (n >= 1) {
        eraseText(options[n-1].length()-1);
    }
    for (int i = 0; i <= n; i++) {
        //if (n-1 == i) {
            std::cout << input + options[i];
        //}
    }
    if (n <= options.size() - 1) {
        n++;
    }
    else {
        n = 0;
    }
}
//void Commands::autoComplete(const std::string& input, const std::vector<std::string>& options) {
//    std::string commonPrefix;
//    bool firstOption = true;
//
//    for (const std::string& option : options) {
//        if (option.substr(0, input.length()) == input) {
//            if (firstOption) {
//                commonPrefix = option;
//                firstOption = false;
//            }
//            else {
//                for (size_t i = 0; i < commonPrefix.length(); i++) {
//                    if (commonPrefix[i] != option[i]) {
//                        commonPrefix = commonPrefix.substr(0, i);
//                        break;
//                    }
//                }
//            }
//        }
//    }
//
//    if (!commonPrefix.empty()) {
//        std::cout << commonPrefix.substr(input.length());
//    }
//}

void Commands::clear() {
    char fill = ' ';
        COORD tl = { 0,0 };
        CONSOLE_SCREEN_BUFFER_INFO s;
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(console, &s);
        DWORD written, cells = s.dwSize.X * s.dwSize.Y;
        FillConsoleOutputCharacter(console, fill, cells, tl, &written);
        FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
        SetConsoleCursorPosition(console, tl);
}
void Commands::eraseText(int numChars) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //COORD cursorPos;
    COORD cursorPos = {}; // Inicjalizacja wartoúciπ domyúlnπ (0, 0)
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    cursorPos.X = csbi.dwCursorPosition.X - numChars;
    cursorPos.Y = csbi.dwCursorPosition.Y;
    SetConsoleCursorPosition(hConsole, cursorPos);

    DWORD numWritten;
    FillConsoleOutputCharacter(hConsole, ' ', numChars, cursorPos, &numWritten);
}
