#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "Headers.h"
#include "Menu.h"
#include "systemOperation.h"
class userInterface {
    Menu* menu;
    string username;
    systemOperation* SystemOperation;
public:
    userInterface();
    void userInterfaceRegisteration();//for (sign in) and (sign up) and (validate) the username and the password
    void signUp();
    void logIn();
    void applyCommands(int Command);
    void mainMenu() noexcept;
    void runUserInterface(); // show commands and return the number the command
    ~userInterface();
};

#endif //USERINTERFACE_H
