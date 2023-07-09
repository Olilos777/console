#include <iostream>
#include <windows.h>
#include "Commands.h"
#include <filesystem>
#include <string>
#include <conio.h>


// Kody kolorów ANSI
const std::string ANSI_RESET = "\033[0m";
const std::string ANSI_RED = "\033[31m";


// Funkcja do ustawienia koloru tekstu w konsoli Windows
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main() {
    
    Commands c;

    while (true) {
        std::cout << c.getCurrentDir();
        setConsoleColor(FOREGROUND_RED);
        std::cout << " > ";
        setConsoleColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Przywrócenie domyślnego koloru
        std::string input = c.getLine();
        
        std::cout << input; // Wyświetlenie aktualnego wejścia

        //if (ch == '\r') { // Sprawdzenie, czy to klawisz Enter
            std::cout << std::endl; // Przejście do nowej linii

            if (!input.empty() && input.find_first_not_of(" \t") != std::string::npos) {
                if (input == "exit") {
                    break;
                }
                else if (input == "ls") {
                    c.listDir(c.getCurrentDir());
                }
                else if (input.substr(0, 2) == "cd") {
                    std::string path = input.substr(3);
                    c.changeDir(path);
                }
                else if (input == "clear") {
                    c.clear();
                } 
            }

            input.clear(); // Wyczyszczenie aktualnego wejścia
        
    }

    return 0;
}
