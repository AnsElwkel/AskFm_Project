#ifndef MENU_H
#define MENU_H
#include "Headers.h"

class Menu {
public:
    vector<string> menu;

    void showMenu()const noexcept;

    void showCommand(const string& command) const noexcept;

    bool isCorrectCommand(int command, int lower, int Upper) const noexcept;

    bool containSpaces(const string& sentence) const noexcept;
};


#endif //MENU_H
