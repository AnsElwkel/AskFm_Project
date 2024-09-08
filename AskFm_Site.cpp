#include "headers/AskFm_Site.h"

void Site::run(){
    _userInterface->userInterfaceRegisteration();
    _userInterface->mainMenu();
    _userInterface->runUserInterface();
}

Site::Site() {
    this->_userInterface = new userInterface();
}

Site::~Site() {
    delete _userInterface;
}
