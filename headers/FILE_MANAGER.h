#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include "Headers.h"

class FileManager {
public:
    int createNewAccount(const vector<string>& information) noexcept;

    tuple<string, int, bool> logInCheck(const string& username, const string& password) const noexcept;

    pair<int , bool>getUserIdByQuestionId(int QuestionId);

    void showQuestionToMe(int myId);

    tuple< bool , int , int > searchOnQuestion(int fromId , int _QuestionId) noexcept;

    void showQuestionFromMe(int myId);

    string search(vector<string> &container,const string& word, int ith_word) const noexcept;

    void printQuestion(int questionId, int fromId,const string& QuestionText, bool isThread, int toId = -1, bool from = 1) const noexcept;

    void printAnswer(int questionId, int fromId, bool sameLine, bool all, bool isThread);

    void removeWithNumberOfLine(const string &fileName, int num);

    vector<string> loadAllFile(const string &fileName);

    vector<string> removeFromUserFile(const string &fileName, const string &str, int type , bool flag = 0);

    vector<string> removeQuestion(const string &fileName, const string &str);

    vector<string> removeAnswer(const string &fileName, const string &str);

    vector<string> removeThreadQuestion(const string &fileName, const string &str , bool flag);

    vector<string> removeThreadAnswer(const string &fileName, const string &str);

    vector<int> loadSpecificLines(const string &fileName, const string &str, int orderInLine, const string &start,
                                  const string &end);

    bool deleteQuestion(int myId, int questionId);

    int createQuestion(int fromId, int toId, int parentQuestionId, int QuestionId, const string &QuestionText,
                       bool isThread ,int userParentQuestionId);

    void writeFile(vector<string> &file, const string &fileName) noexcept;

    void removeFromQuestionIdes(const string &str);

    void listingSystemUsers() const noexcept;

    void listingTheUserFeed();

    int isAlreadyAnswered(int toId , int QuestionId);

    int insertToQuestionIdesFile(int fromId, bool isThread) noexcept;

    void insertToUserFile(const string &fileName, const string &str, int type);

    int findInVector(const vector<string> &container, const string &str) noexcept;

    void createAnswer(int userId,int toId,int QuestionId,const string & answerText, bool isThreadQuestion = 0 , bool isAnswered = 0);

    void editOnFile(const string &fileName, const string &searchWord, const string &editWord, int type,
                    int orderInLineToSearch, int orderInLineToEdit);
};
#endif //FILE_MANAGER_H
