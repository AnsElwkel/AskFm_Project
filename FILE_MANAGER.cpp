#include "headers/FILE_MANAGER.h"

#include <format>

#include "headers/systemOperation.h"

/** Check information of logIn true or false */
tuple<string, int, bool> FileManager::logInCheck(const string& username,const string &password) const noexcept{
    string line, fileName = "Files/Private/users";
    ifstream fin(fileName);
    while (getline(fin, line)) {
        stringstream ss(line);
        string tmp1, tmp2;
        int id;
        ss >> tmp1 >> tmp2 >> id;
        if (tmp1 == username and tmp2 == password) {
            fin.close();
            return {username, id, true};
        }
    }
    fin.close();
    return {"NULL", -1, false};
}

int FileManager::createNewAccount(const vector<string>& information) noexcept {
    int newId = 100;
    string line, fileName = "Files/Private/users";
    ifstream fin(fileName);
    while (getline(fin, line)) {
        stringstream ss(line);
        string name, pass;
        int id;
        ss >> name >> pass >> id;
        newId = ++id;
    }
    fin.close();
    ofstream fout(fileName, ios::out | ios::app);
    fout << information[0] << ' ' << information[1] << ' ' << newId << endl;
    fout.close();
    fileName = "Files/Public/" + to_string(newId);
    fout.open(fileName, ios::out | ios::app);
    fout << information[0] << ' ' << information[2] << ' ' << information[3] << ' ' << newId << endl;
    fout << "QUESTION_SECTION:" << endl;
    fout << "ANSWER_SECTION:" << endl;
    fout << "THREAD_QUESTION_SECTION:" << endl;
    fout << "THREAD_ANSWER_SECTION:" << endl;
    fout << "~~~" << endl;
    fout.close();
    return newId;
}

string FileManager::search(vector<string> &container,const string& word, int ith_word) const noexcept {
    for (const auto &line: container) {
        if (line.find(word) != string::npos) {
            stringstream ss(line);
            string ret, ignore;
            int n = ith_word - 1;

            while (n--)
                ss >> ignore;

            getline(ss, ret);
            ret = ret.substr(std::min(static_cast<int>(ret.size()), 1), max(static_cast<int>(ret.size()) - 1, 0));
            return ret;
        }
    }
    return "NULL";
}

void FileManager::showQuestionToMe(int myId) {
    string fileName1 = "Files/Public/" + to_string(myId);
    vector<string> file = loadAllFile(fileName1), Questions, Answers, thQuestions, thAnswers;
    vector<int> questionsLines = loadSpecificLines(fileName1, to_string(myId), 1, "QUESTION_SECTION:",
                                                   "ANSWER_SECTION:")
            , answersLines = loadSpecificLines(fileName1, to_string(myId), 1, "ANSWER_SECTION:",
                                               "THREAD_QUESTION_SECTION:")
            , thQuestionsLines = loadSpecificLines(fileName1, to_string(myId), 1, "THREAD_QUESTION_SECTION:",
                                                   "THREAD_ANSWER_SECTION:")
            , thAnswersLines = loadSpecificLines(fileName1, to_string(myId), 1, "THREAD_ANSWER_SECTION:", "~~~");


    for (const auto &i: answersLines)
        Answers.push_back(file[i - 1]);

    for (const auto &i: thAnswersLines)
        thAnswers.push_back(file[i - 1]);

    string line, fileName = "Files/Private/users";
    ifstream fin(fileName);
    while (getline(fin, line)) {
        stringstream ss(line);
        string user, pass;
        int id;
        ss >> user >> pass >> id;
        if (id == myId) continue;

        fileName1 = "Files/Public/" + to_string(id);
        file.clear();
        file = loadAllFile(fileName1);
        questionsLines = loadSpecificLines(fileName1, to_string(myId), 2, "QUESTION_SECTION:", "ANSWER_SECTION:");
        thQuestionsLines = loadSpecificLines(fileName1, to_string(myId), 2, "THREAD_QUESTION_SECTION:",
                                             "THREAD_ANSWER_SECTION:");
        for (const auto &i: questionsLines)
            Questions.push_back(file[i - 1]);
        for (const auto &i: thQuestionsLines)
            thQuestions.push_back(file[i - 1]);
    }

    for (const auto &question: Questions) {
        stringstream ss(question);
        int fromId, Id, toId, questionId;
        bool isAnswer, isThread;
        string questionText;
        ss >> fromId >> toId >> questionId >> isAnswer >> isThread;
        getline(ss, questionText);
        questionText = questionText.substr(1, questionText.size() - 1);
        printQuestion(questionId, fromId, questionText, 0);
        if (!isAnswer)
            continue;
        string answerText = search(Answers, to_string(questionId), 4);

        if (answerText == "NULL")
            throw runtime_error("Question not found");

        cout << "\t\tAnswer: " << answerText << "\n\n";

        if (!isThread) continue;

        for (const auto &thQuestion: thQuestions) {
            stringstream ss(thQuestion);
            int fromId, toId, parentId, qId;
            bool isAns;
            string QuestionText;
            ss >> fromId >> toId >> parentId >> qId >> isAns;
            getline(ss, QuestionText);
            QuestionText = QuestionText.substr(1, QuestionText.size() - 1);

            if (parentId == questionId) {
                printQuestion(qId, fromId, QuestionText, 1);
                if (!isAns) continue;

                string answerText = search(thAnswers, to_string(qId), 4);
                if (answerText == "NULL")
                    throw runtime_error("Question not found");
                cout << "\t\tThread:\t\tAnswer: " << answerText << "\n\n";
            }
        }
    }
}

void FileManager::showQuestionFromMe(int myId) {
    string fileName1 = "Files/Public/" + to_string(myId);
    vector<string> file = loadAllFile(fileName1), Questions, thQuestions;
    vector<int> questionLines = loadSpecificLines(fileName1, to_string(myId), 1, "QUESTION_SECTION:", "ANSWER_SECTION:")
            ,
            thQuestionLines = loadSpecificLines(fileName1, to_string(myId), 1, "THREAD_QUESTION_SECTION:",
                                                "THREAD_ANSWER_SECTION:");
    for (const auto &i: questionLines)
        Questions.push_back(file[i - 1]);
    for (const auto &i: thQuestionLines)
        thQuestions.push_back(file[i - 1]);

    for (const auto &Question: Questions) {
        stringstream ss(Question);
        int fromId, toId, questionId;
        bool isAnswer, isThread;
        string QuestionText;
        ss >> fromId >> toId >> questionId >> isAnswer >> isThread;
        getline(ss, QuestionText);
        QuestionText = QuestionText.substr(1, QuestionText.size() - 1);
        printQuestion(questionId, toId, QuestionText, 0, -1, 0);
        if (!isAnswer) {
            cout << "Not Answered Yet !" << endl;
            continue;
        }
        printAnswer(questionId, toId, 1, 0, 0);
    }
    for (const auto &Question: thQuestions) {
        stringstream ss(Question);
        int fromId, toId, parent, questionId;
        bool isAnswer;
        string QuestionText;
        ss >> fromId >> toId >> parent >> questionId >> isAnswer;
        getline(ss, QuestionText);
        QuestionText = QuestionText.substr(1, QuestionText.size() - 1);
        printQuestion(questionId, toId, QuestionText, 0, -1, 0);
        if (!isAnswer) {
            cout << "Not Answered Yet !" << endl;
            continue;
        }
        printAnswer(questionId, toId, 1, 0, 1);
    }
}

void FileManager::printQuestion(int questionId, int fromId, const string& QuestionText, bool isThread, int toId, bool from) const noexcept{
    ostringstream oss;
    oss << " To user id(" << to_string(toId) << ")";
    cout << (isThread ? "\t\tThread: " : "") << "Question Id (" << questionId << ") " << (from ? "from" : "to") <<
            " user id(" << fromId << ")"
            << (~toId ? oss.str() : "") << "\tQuestion:" << QuestionText << endl;
}

void FileManager::printAnswer(int questionId, int fromId, bool sameLine, bool all, bool isThread)  {
    string fileName = "Files/Public/" + to_string(fromId);

    vector<string> file = loadAllFile(fileName), answers;
    vector<int> answerLines;
    if (!isThread)
        answerLines = loadSpecificLines(fileName, to_string(fromId), 1, "ANSWER_SECTION:", "THREAD_QUESTION_SECTION:");
    else
        answerLines = loadSpecificLines(fileName, to_string(fromId), 1, "THREAD_ANSWER_SECTION:", "~~~");

    for (const auto &i: answerLines)
        answers.push_back(file[i - 1]);

    string answerText = search(answers, to_string(questionId), 4);
    if (answerText == "NULL")
        throw runtime_error("answer does not exist");
    if (!sameLine)
        cout << "\t\t";

    cout << "Answer: " << answerText << "\n";
}

void FileManager::writeFile(vector<string> &file, const string &fileName) noexcept {
    ofstream fout(fileName, ios::out | ios::trunc);
    for (const auto &line: file)
        fout << line << endl;
    fout.close();
}

void FileManager::removeWithNumberOfLine(const string &fileName, int num) {
    vector<string> lines = loadAllFile(fileName);
    string line;
    ifstream fin(fileName);
    if (fin.fail())
        throw runtime_error("File not found , function -> removeWithNumberOfLine");
    lines.erase(lines.begin() + num - 1);
    writeFile(lines, fileName);
}

vector<string> FileManager::loadAllFile(const string &fileName) {
    vector<string> file;
    ifstream fin(fileName);
    if (fin.fail())
        throw runtime_error("File not found (function-> loadAllFile)");
    string line;
    while (getline(fin, line)) {
        file.push_back(line);
    }

    return file;
}

void FileManager::removeFromQuestionIdes(const string &str) {
    string line, fileName = "Files/Public/QuestionIdes";
    ifstream fin(fileName);
    int numOfLine = 0;
    while (getline(fin, line)) {
        numOfLine++;
        istringstream iss(line);
        string comp = "", temp1 = "", temp2 = "";
        iss >> temp1 >> temp2;
        comp = temp1 + ' ' + temp2;
        if (comp == str) {
            break;
        }
    }
    fin.close();
    removeWithNumberOfLine(fileName, numOfLine);
}

vector<string> FileManager::removeFromUserFile(const string &fileName, const string &str, int type, bool flag) {
    vector<string> removedLines;
    if (type == 1)
        removedLines = removeQuestion(fileName, str);
    else if (type == 2)
        removedLines = removeAnswer(fileName, str);
    else if (type == 3)
        removedLines = removeThreadQuestion(fileName, str, flag);
    else if (type == 4)
        removedLines = removeThreadAnswer(fileName, str);
    else
        throw invalid_argument("invalid type (function-> removeFromUserFile)");

    return removedLines;
}

vector<string> FileManager::removeQuestion(const string &fileName, const string &str) {
    vector<string> file = loadAllFile(fileName), ret;
    string line;
    ifstream fin(fileName);
    if (fin.fail())
        throw runtime_error("File not found (function-> removeQuestion)");
    vector<int> removedlines = loadSpecificLines(fileName, str, 3, "QUESTION_SECTION:", "ANSWER_SECTION:");
    for (auto i: removedlines) {
        ret.push_back(file[i - 1]);
        file.erase(file.begin() + i - 1);
    }
    writeFile(file, fileName);
    return ret;
}

vector<string> FileManager::removeAnswer(const string &fileName, const string &str) {
    vector<string> file = loadAllFile(fileName), ret;
    string line;
    ifstream fin(fileName);
    if (fin.fail())
        throw runtime_error("File not found (function-> removeQuestion)");
    vector<int> removedlines = loadSpecificLines(fileName, str, 3, "ANSWER_SECTION:", "THREAD_QUESTION_SECTION:");
    for (auto i: removedlines) {
        ret.push_back(file[i - 1]);
        file.erase(file.begin() + i - 1);
    }
    writeFile(file, fileName);
    return ret;
}

vector<string> FileManager::removeThreadQuestion(const string &fileName, const string &str, bool flag) {
    vector<string> file = loadAllFile(fileName), ret;
    string line;
    ifstream fin(fileName);
    if (fin.fail())
        throw runtime_error("File not found (function-> removeQuestion)");
    vector<int> removedlines = loadSpecificLines(fileName, str, (flag ? 4 : 3), "THREAD_QUESTION_SECTION:",
                                                 "THREAD_ANSWER_SECTION:");

    for (auto i: removedlines) {
        ret.push_back(file[i - 1]);
        file.erase(file.begin() + i - 1);
    }
    writeFile(file, fileName);
    return ret;
}

vector<string> FileManager::removeThreadAnswer(const string &fileName, const string &str) {
    vector<string> file = loadAllFile(fileName), ret;
    string line;
    ifstream fin(fileName);
    if (fin.fail())
        throw runtime_error("File not found (function-> removeQuestion)");
    vector<int> removedlines = loadSpecificLines(fileName, str, 3, "THREAD_ANSWER_SECTION:", "~~~");
    for (auto i: removedlines) {
        ret.push_back(file[i - 1]);
        file.erase(file.begin() + i - 1);
    }
    writeFile(file, fileName);
    return ret;
}

int FileManager::isAlreadyAnswered(int toId, int QuestionId) {
    const string fileName = "Files/Public/" + to_string(toId);
    vector<int> answerIndex = loadSpecificLines(fileName, to_string(QuestionId), 3, "ANSWER_SECTION:",
                                                "THREAD_QUESTION_SECTION:");
    if (answerIndex.empty())
        return -1;
}

int FileManager::insertToQuestionIdesFile(int fromId, bool isThread) noexcept {
    const string fileName = "Files/Public/QuestionIdes";
    int newQuestionId = 1;
    string line;
    ifstream fin(fileName);
    if (!fin.fail()) {
        int id, QuestionId, isThread;
        while (getline(fin, line)) {
            stringstream ss(line);
            ss >> id >> QuestionId >> isThread;
            newQuestionId = QuestionId + 1;
        }
        fin.close();
    }
    ofstream fout(fileName, ios::out | ios::app);
    fout << fromId << ' ' << newQuestionId << ' ' << isThread << '\n';
    fout.close();
    return newQuestionId;
}

void FileManager::insertToUserFile(const string &fileName, const string &str, int type) {
    string line = "";
    if (type == 1)
        line = "QUESTION_SECTION:";
    else if (type == 2)
        line = "ANSWER_SECTION:";
    else if (type == 3)
        line = "THREAD_QUESTION_SECTION:";
    else if (type == 4)
        line = "THREAD_ANSWER_SECTION:";
    else
        throw invalid_argument("invalid type (function-> insertToUserFile)");

    vector<string> file = loadAllFile(fileName);
    if (file.empty())
        throw runtime_error("Failed to load File (function-> insertToUserFile)");
    int idx = findInVector(file, line);
    if (idx == -1)
        throw runtime_error("line not found (function-> insertToUserFile->findInVector)");
    file.insert(file.begin() + idx + 1, str);
    writeFile(file, fileName);
}

int FileManager::findInVector(const vector<string> &container, const string &str) noexcept {
    for (int idx = 0; idx < container.size(); idx++) {
        if (container[idx] == str)
            return idx;
    }
    return -1;
}

/** Answering on certain question */
void FileManager::createAnswer(int userId, int toId, int QuestionId, const string &answerText, bool isThreadQuestion,
                               bool isAnswered) {
    string fileName = "Files/Public/" + to_string(userId),
            str = to_string(userId) + ' ' + to_string(toId) + ' ' + to_string(QuestionId) + ' ' + answerText;
    if (isAnswered) {
        if (isThreadQuestion)
            removeThreadAnswer(fileName, to_string(toId));
        else
            removeAnswer(fileName, to_string(toId));
    }

    insertToUserFile(fileName, str, (isThreadQuestion ? 4 : 2));
    fileName = "Files/Public/" + to_string(toId),
            editOnFile(fileName, to_string(QuestionId), "1", isThreadQuestion ? 3 : 1, (isThreadQuestion ? 4 : 3),
                       (isThreadQuestion ? 5 : 4));
}

void FileManager::editOnFile(const string &fileName, const string &searchWord, const string &editWord, int type,
                             int orderInLineToSearch, int orderInLineToEdit) {
    string start = "", end = "";
    if (type == 1)
        start = "QUESTION_SECTION:", end = "ANSWER_SECTION:";
    else if (type == 2)
        start = "ANSWER_SECTION:", end = "THREAD_QUESTION_SECTION:";
    else if (type == 3)
        start = "THREAD_QUESTION_SECTION:", end = "ANSWER_SECTION:";
    else if (type == 4)
        start = "THREAD_ANSWER_SECTION:", end = "~~~";
    else
        throw invalid_argument("invalid type (function-> editOnFile)");

    vector<string> file = loadAllFile(fileName);
    if (file.empty())
        throw runtime_error("File not found (function->editOnFile)");
    vector<int> idx = loadSpecificLines(fileName, searchWord, orderInLineToSearch, start, end);
    if (idx.empty())
        throw runtime_error("Failed to load idx of line (function->editOnFile->loadSpecificLines)");

    vector<string> contentOfLine;
    stringstream ss(file[idx[0] - 1]);
    while (!ss.eof()) {
        string tmp = "";
        ss >> tmp;
        contentOfLine.push_back(tmp);
    }

    if (contentOfLine.empty())
        throw logic_error("File not found (function->editOnFile->loadContentOfLine)");

    contentOfLine[orderInLineToEdit - 1] = editWord;

    string editedLine = "";
    for (const auto &i: contentOfLine)
        editedLine += i, editedLine += " ";

    file[idx[0] - 1] = editedLine;
    writeFile(file, fileName);
}

/** Load specific lines by given certain search word and order of this word in line*/
vector<int> FileManager::loadSpecificLines(const string &fileName, const string &str, int orderInLine,
                                           const string &start, const string &end) {
    vector<int> ret;
    string line;
    ifstream fin(fileName);
    if (fin.fail())
        throw runtime_error("Failed to open file (function-> loadSpecificLines)");
    int i = 0;
    while (getline(fin, line)) {
        i++;
        if (line == start)
            break;
    }
    while (getline(fin, line) and line != end) {
        i++;
        vector<string> words;
        stringstream ss(line);
        while (!ss.eof()) {
            string word;
            ss >> word;
            words.push_back(word);
        }
        if (words[min(orderInLine - 1, (int) words.size())] == str)
            ret.push_back(i);
    }
    fin.close();

    return ret;
}

/** Delete question from system */
bool FileManager::deleteQuestion(int myId, int questionId) {
    /**
     * -Remove from user file
     * -Remove answer from reciever file
     * -Remove any thread and its answer that parent is this question
     * -Remove quesiton id from QuestionIdes file
     * -return true -> found and to erase from set of uniqueQuestionIdes
     */

    // Remove quesiton id from QuestionIdes file
    bool isThreadQuestion = 0, isExist = 0;
    string line, fileName = "Files/Public/QuestionIdes";
    ifstream fin(fileName);
    while (getline(fin, line)) {
        stringstream ss(line);
        int fromId, Qid;
        bool isTh;
        ss >> fromId >> Qid >> isTh;
        if (Qid == questionId) {
            isExist = 1;
            if (isTh)
                isThreadQuestion = 1;
            break;
        }
    }
    fin.close();

    if (!isExist)
        return false;

    removeFromQuestionIdes(to_string(myId) + ' ' + to_string(questionId));

    fileName = "Files/Public/" + to_string(myId);
    if (isThreadQuestion) {
        vector<string> removedLines = removeFromUserFile(fileName, to_string(questionId), 3, 1);

        istringstream iss(removedLines[0]); //1 elements only

        int fromId, toId, parId, QuestionId;
        bool isAnswer;
        string QuestionText;
        iss >> fromId >> toId >> parId >> QuestionId >> isAnswer;
        getline(iss, QuestionText);
        QuestionText = QuestionText.substr(1, QuestionText.size() - 1);

        if (isAnswer) {
            fileName = "Files/Public/" + to_string(toId);
            // Remove answer from reciever file
            vector<string> removedLinesOfAnswer = removeFromUserFile(fileName, to_string(QuestionId), 4);

        }
    } else {
        // Remove from user file
        vector<string> removedLines = removeFromUserFile(fileName, to_string(questionId), 1);

        istringstream iss(removedLines[0]); //1 elements only

        int fromId, toId, QuestionId;
        bool isAnswer, isThread;
        string QuestionText;
        iss >> fromId >> toId >> QuestionId >> isAnswer >> isThread;
        getline(iss, QuestionText);
        QuestionText = QuestionText.substr(1, QuestionText.size() - 1);

        if (isAnswer) {
            fileName = "Files/Public/" + to_string(toId);
            // Remove answer from receiver file
            vector<string> removedLinesOfAnswer = removeFromUserFile(fileName, to_string(QuestionId), 2);

            if (isThread) {
                // Remove any thread and its answer that parent is this question
                fileName = "Files/Private/users";
                ifstream fin(fileName);
                string line;
                while (getline(fin, line)) {
                    stringstream ss(line);
                    string name, pass;
                    int userId;
                    ss >> name >> pass >> userId;
                    fileName = "Files/Public/" + to_string(userId);
                    vector<string> removedLinesOfThreadQuestions = removeFromUserFile(
                        fileName, to_string(QuestionId), 3);

                    for (const auto &deletedLine: removedLinesOfThreadQuestions) {
                        istringstream iss(deletedLine);
                        int fromId, toId, parentId, thQuestionId;
                        bool isAnswer;
                        string questionText;
                        iss >> fromId >> toId >> parentId >> thQuestionId >> isAnswer;

                        removeFromQuestionIdes(to_string(fromId) + ' ' + to_string(thQuestionId));
                        getline(iss, QuestionText);
                        QuestionText = QuestionText.substr(1, QuestionText.size() - 1);
                        if (isAnswer) {
                            fileName = "Files/Public/" + to_string(toId);
                            vector<string> removedLinesOfThreadAnswers = removeFromUserFile(
                                fileName, to_string(thQuestionId), 4);
                        }
                    }
                }
                fin.close();
            }
        }
    }

    return true;
}

/** Add new question to the system */
int FileManager::createQuestion(int fromId, int toId, int parentQuestionId, int QuestionId, const string &QuestionText,
                                bool isThread, int userParentQuestionId) {
    /**
     * Add information line of question to user file
     * Add information to question ides file
     * if (isThread) -> check first that parent question is answered or not
     *      if(answered) edit line of parent Question , else return 0
     */
    string fileName = "Files/Public/" + to_string(userParentQuestionId);
    if (isThread) {
        vector<string> file = loadAllFile(fileName);
        if (file.empty())
            throw runtime_error("Failed to load file (function->createQuestion)");
        vector<int> parentQuestionLine = loadSpecificLines(fileName, to_string(parentQuestionId), 3,
                                                           "QUESTION_SECTION:", "ANSWER_SECTION:");
        bool isAnswered = 0;
        if (parentQuestionLine.empty())
            return -1;

        stringstream ss(file[parentQuestionLine[0] - 1]);
        vector<string> words;
        while (!ss.eof()) {
            string word;
            ss >> word;
            words.push_back(word);
        }
        if (words[3] == "0")
            return 0;

        words[4] = "1"; // flag to contain threads
        string editedLine = "";
        for (const string &word: words) {
            editedLine += word, editedLine += " ";
        }
        file[parentQuestionLine[0] - 1] = editedLine;
        writeFile(file, fileName);
    }

    fileName = "Files/Public/" + to_string(fromId);

    const string addedLine = (isThread
                                  ? (to_string(fromId) + " " + to_string(toId) + " " + to_string(parentQuestionId) + " "
                                     + to_string(QuestionId) + " 0 " + QuestionText)
                                  : (to_string(fromId) + " " + to_string(toId) + " " + to_string(QuestionId) + " 0 0 " +
                                     QuestionText));
    insertToUserFile(fileName, addedLine, (isThread ? 3 : 1));

    return 1;
}

/** Show Users and id of each of them*/
void FileManager::listingSystemUsers() const noexcept {
    string line, fileName = "Files/Private/users";
    ifstream fin(fileName);
    while (getline(fin, line)) {
        stringstream ss(line);
        string user, pass;
        int userId;
        ss >> user >> pass >> userId;
        cout << "ID: " << userId << "\t\t" << "Name: " << user << endl;
    }
    fin.close();
}

/** Show all question of community */
void FileManager::listingTheUserFeed() {
    vector<string> file, thQuestion, thAnswers;

    string line, fileName = "Files/Private/users";
    ifstream fin(fileName);
    while (getline(fin, line)) {
        stringstream ss(line);
        string user, pass;
        int id;
        ss >> user >> pass >> id;
        vector<string> Questions;
        string fileName = "Files/Public/" + to_string(id);
        file.clear();
        file = loadAllFile(fileName);
        vector<int> QuestionLines = loadSpecificLines(fileName, to_string(id), 1, "QUESTION_SECTION:",
                                                      "ANSWER_SECTION:");
        for (const auto &i: QuestionLines)
            Questions.push_back(file[i - 1]);

        for (const auto &line: Questions) {
            stringstream ss(line);

            int fromId, toId, QuestionId;
            bool isAnswer, isThreadQuestion;
            string QuestionText;
            ss >> fromId >> toId >> QuestionId >> isAnswer >> isThreadQuestion;
            getline(ss, QuestionText);
            QuestionText = QuestionText.substr(1, QuestionText.size() - 1);
            printQuestion(QuestionId, fromId, QuestionText, 0, toId);
            if (!isAnswer) continue;

            printAnswer(QuestionId, toId, 0, 0, 0);
        }
    }
    fin.close();

    fin.open(fileName);
    while (getline(fin, line)) {
        stringstream ss(line);
        string user, pass;
        int id;
        ss >> user >> pass >> id;
        string fileName = "Files/Public/" + to_string(id);
        file.clear();
        file = loadAllFile(fileName);
        vector<int> thQuestionLines = loadSpecificLines(fileName, to_string(id), 1, "THREAD_QUESTION_SECTION:",
                                                        "THREAD_ANSWER_SECTION:"),
                thAnswerLines = loadSpecificLines(fileName, to_string(id), 1, "THREAD_ANSWER_SECTION:", "~~~");
        for (const auto &i: thQuestionLines)
            thQuestion.push_back(file[i - 1]);
        for (const auto &i: thAnswerLines)
            thAnswers.push_back(file[i - 1]);
    }
    fin.close();

    for (const auto &question: thQuestion) {
        stringstream ss(question);
        int fromId, toId, parentQuestion, QuestionId;
        bool isAnswer;
        string QuestionText;

        ss >> fromId >> toId >> parentQuestion >> QuestionId >> isAnswer;
        getline(ss, QuestionText);
        QuestionText = QuestionText.substr(1, QuestionText.size() - 1);

        cout << "Thread Parent Question ID (" << parentQuestion << ") Question Id (" << QuestionId << ") from user ("
                << fromId << ") To user id(" << toId << ")\tQuestion: " << QuestionText << endl;

        if (!isAnswer)
            continue;

        string answerText = search(thAnswers, to_string(QuestionId), 4);
        cout << "\t\tAnswer: " << answerText << endl;
    }
}

/**  Get user id of certain question and check the question is exist or not also */
pair<int, bool> FileManager::getUserIdByQuestionId(int QuestionId) {
    const string fileName = "Files/Public/QuestionIdes";
    ifstream fin(fileName);
    if (fin.fail())
        throw runtime_error("Failed to open file , function -> getUserIdByQuestionId(int QuestionId)");
    pair<int, bool> ret = {-1, -1};
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        int id, Qid;
        bool isThread;
        ss >> id >> Qid >> isThread;
        if (Qid == QuestionId)
            ret = {id, isThread};
    }
    return ret;
}

/**  Check if Parent question of thread question is exist or not/
*    return that exist or not , userId of parent question and new id of thread question
*/
tuple<bool, int, int> FileManager::searchOnQuestion(int fromId, int _QuestionId) noexcept{
    bool find = 0;
    int userParentQuestionId = -1, newThQuestionId = -1;
    string line, fileName = "Files/Public/QuestionIdes";
    ifstream fin(fileName);
    while (getline(fin, line)) {
        stringstream ss(line);
        int id, QuestionId, isThread;
        ss >> id >> QuestionId >> isThread;
        if (!isThread and QuestionId == _QuestionId)
            userParentQuestionId = id, find = 1;

        newThQuestionId = ++QuestionId;
    }
    fin.close();
    if (find) {
        ofstream fout(fileName, ios::out | ios::app);
        fout << fromId << ' ' << newThQuestionId << ' ' << 1 << endl;
        fout.close();
    }

    return {find, userParentQuestionId, newThQuestionId};
}
