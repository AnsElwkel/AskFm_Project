#include "headers/systemOperation.h"
#include <fstream>

//Call private function validateLogIn
bool systemOperation::validateInfo(string username, string password) noexcept {
    return validateLogIn(username, password);
}

//Log In process
bool systemOperation::validateLogIn(string userName, string password) noexcept {
    auto [_username , id , ret] = fileManager.logInCheck(userName, password);
    if (_username != "NULL" and id != -1) {
        username = _username;
        userId = id;
    }else {
        cout << "Invalid username or password." << endl;
    }
    return ret;
}

//Call private function
void systemOperation::operationsInAccount(const vector<string>& information, int operation, bool isThreadQuestion,
                               int userId, int questionId, const string& questionText, const string& answerText,
                               int toId,bool isAnswered) {
    _operationsInAccount(information, operation, isThreadQuestion, userId, questionId, questionText, answerText, toId);
}

//Main function to apply each command in service
void systemOperation::_operationsInAccount(const vector<string>& information, int operation, bool isThreadQuestion,
                               int _userId, int questionId, const string& questionText, const string& answerText,
                               int toId ,bool isAnswered) {
    string fileName;
    if (operation == 0) {
        int uniqueId = fileManager.createNewAccount(information);
        userId = uniqueId;
    } else if (operation == 1)
        fileManager.showQuestionToMe(userId);
    else if (operation == 2) //Print Question from Me
        fileManager.showQuestionFromMe(userId);
    else if (operation == 3) {
        fileManager.createAnswer(userId, toId, questionId, answerText, isThreadQuestion , isAnswered);
    } else if (operation == 4) {
        if (!fileManager.deleteQuestion(userId, questionId))
            cout << "Question not found" << endl;
    } else if (operation == 5) {
        int thQuestionId = -1, userParentQuestionId = -1;
        bool cancel = false;
        if (!isThreadQuestion) {
            questionId = fileManager.insertToQuestionIdesFile(userId , 0);
        } else {
            bool find = 0;
            tie(find , userParentQuestionId , thQuestionId) = fileManager.searchOnQuestion(userId , questionId);
            if (!find) {
                cout << "Question not found" << endl;
                cancel = true;
            }
        }
        if (!cancel) {
            int state = fileManager.createQuestion(userId , _userId , questionId , (isThreadQuestion ? thQuestionId : questionId ), questionText , isThreadQuestion , userParentQuestionId);
            if(state == 0)
                cout << "Parent Question not answered\n";
            else if (state == -1)
                cout << "Question not found\n";
        }
    } else if (operation == 6)
        fileManager.listingSystemUsers();
    else if (operation == 7) {
        fileManager.listingTheUserFeed();
    }
}


pair<int , bool> systemOperation::getUserIdByQuestionId(int QuestionId) {
    return fileManager.getUserIdByQuestionId(QuestionId);
}

//Check state of answer on certain question and show warning if it's already answered
int systemOperation::isAlreadyAnswered(int toId , int QuestionId) {
    return this->fileManager.isAlreadyAnswered(toId , QuestionId);
}
