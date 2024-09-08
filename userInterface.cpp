#include "headers/userInterface.h"

void userInterface::userInterfaceRegisteration() {
    menu->menu.push_back("Sign Up");
    menu->menu.push_back("Log In");
    menu->showMenu();
    int choice = 0;
    menu->showCommand("Enter number in range 1 - 2: ");
    cin >> choice;
    while (!menu->isCorrectCommand(choice, 1, 2)) {
        cin >> choice;
        cout << endl;
    }

    if (choice == 1)
        signUp();
    else
        logIn();
}

// LogIn process
void userInterface::logIn() {
    string username, password;
    do {
        menu->showCommand("Enter username:");
        cin >> username;
        cout << endl;
        menu->showCommand("Enter password:");
        cin >> password;
        cout << endl;
        this->username = username;
    } while (!SystemOperation->validateInfo(username, password));
}

// Main menu after open the account
void userInterface::mainMenu() noexcept {
    menu->menu.clear();
    menu->menu.push_back("Print Question To Me");
    menu->menu.push_back("Print Question From Me");
    menu->menu.push_back("Answer Question");
    menu->menu.push_back("Delete Question");
    menu->menu.push_back("Ask Question");
    menu->menu.push_back("List System Users");
    menu->menu.push_back("Feed");
    menu->menu.push_back("Log out");
}

// Main site process
void userInterface::runUserInterface() {
    int choice = 0;
    menu->showMenu();
    do {
        do {
            menu->showCommand("Enter number in range 1 - 8: ");
            cin >> choice;
            cout << endl;
        } while (menu->isCorrectCommand(choice, 1, 8) == false);
        if (choice != 8)
            applyCommands(choice); //apply and update data
    } while (choice != 8);
}

// Sign Up process
void userInterface::signUp() {
    string username, password, name, email;
    do {
        menu->showCommand("Enter Username (No spaces):");
        cin.ignore();
        getline(cin, username);
        cout << endl;
    } while (menu->containSpaces(username));
    do {
        menu->showCommand("Enter Password (No spaces):");
        getline(cin, password);
        cout << endl;
    } while (menu->containSpaces(password));
    do {
        menu->showCommand("Enter Name (No spaces):");
        getline(cin, name);
        cout << endl;
    } while (menu->containSpaces(name));
    do {
        menu->showCommand("Enter email (No spaces):");
        getline(cin, email);
        cout << endl;
    } while (menu->containSpaces(email));
    this->username = username;
    vector<string> inforamtion = {username, password, name, email};
    SystemOperation->operationsInAccount(inforamtion, 0, 0, 0, 0, "", "", 0);
}

// API for site
void userInterface::applyCommands(int choice) {
    bool cancel = false, thread = 0;
    int id, questionId = -1, toId, isAnswered = 0;
    string questionText, answerText;

    // Answer On Questions
    if (choice == 3) {
        menu->showCommand("Enter Question id or -1 to cancel:");
        cin >> questionId;
        if (~questionId) {
            // Search on question and return user id of sender and state of question (thread or not)
            tie(toId, thread) = SystemOperation->getUserIdByQuestionId(questionId);
            if (~toId) {
                // Check the question is answer or not
                isAnswered = SystemOperation->isAlreadyAnswered(toId, questionId);
                if (~isAnswered)
                    cout << "Warning: Already answered. Answer will be updated\n";

                menu->showCommand("Enter Answer:");
                cin.ignore();
                getline(cin, answerText);

            } else {
                cout << "Question " << questionId << " does not exist" << endl;
                cancel = true;
            }
        } else
            cancel = true;
    } else if (choice == 4) {
        menu->showCommand("Enter Question id or -1 to cancel:");
        cin >> questionId;
    }

    //Make new question
    else if (choice == 5) {
        menu->showCommand("Enter User id or -1 to cancel:");
        cin >> id;
        if (~id) {
            menu->showCommand("For thread question: Enter Question id or -1 for new question:");
            cin >> questionId;
            if (~questionId) {
                //thread
                thread = 1;
            }
            menu->showCommand("Enter question text:");
            cin.ignore();
            getline(cin, questionText);
        } else cancel = true;
    }

    // Send information to backend to execute the process
    if (!cancel)
        SystemOperation->operationsInAccount({username}, choice, thread, id, questionId, questionText, answerText, toId,
                                  isAnswered); // Working with data and Updating
}

userInterface::userInterface() {
    this->menu = new Menu();
    this->SystemOperation = new systemOperation();
}

userInterface::~userInterface() {
    delete menu;
    delete SystemOperation;
}
