#include "headers/Menu.h"

void Menu::showMenu() const noexcept {
    cout << "Menu:" << endl;
    for (int i = 0; i < menu.size(); i++)
        cout << i + 1 << ": " << menu[i] << endl;
}

void Menu::showCommand(const string& command) const noexcept {
    cout << command << endl;
}

bool Menu::isCorrectCommand(int command, int lower, int Upper) const noexcept {
    if (command < lower || command > Upper)
        return cout << "Invalid command entered!" << endl, 0;
    return true;
}

bool Menu::containSpaces(const string& sentence) const noexcept {
    if (sentence.find(' ') != string::npos)
        return cout << "Please don't write spaces" << endl, true;
    return false;
}
