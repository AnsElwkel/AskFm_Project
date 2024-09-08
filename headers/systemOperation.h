#ifndef SYSTEMOPERATION_H
#define SYSTEMOPERATION_H
#include "Headers.h"
#include "FILE_MANAGER.h"

class systemOperation {
    FileManager fileManager;
    string username;
    int userId;

    bool validateLogIn(string username, string password) noexcept;

    void _operationsInAccount(const vector<string> &information, int operation, bool isThreadQuestion,
                              int _userId, int questionId, const string &questionText, const string &answerText,
                              int toId = -1, bool isAnswered = false);

public:
    bool validateInfo(string username, string password) noexcept;

    void operationsInAccount(const vector<string> &information, int operation, bool isThreadQuestion,
                             int userId, int questionId, const string &questionText, const string &answerText,
                             int toId = -1, bool isAnswered = false);

    int isAlreadyAnswered(int toId, int questionId);

    pair<int, bool> getUserIdByQuestionId(int QuestionId);
};

#endif //SYSTEMOPERATION_H
