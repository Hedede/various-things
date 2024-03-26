#include <iostream>
//#include <stdlib.h>
//#include <stdio.h>
#include <windows.h>
#include <strings.h>
#include <hddbplib.h>

using namespace std;

    const int MAXITEMS = 4;

class quest {
    private:
        class question {
            private:
                string question;
                string answers[12];
                string actions[12];
            public:
                bool SetQuestion(const string str) {
                    question = str;
                    return true;
                }
                
                bool AddAnswer(int id, const string answer) {
                    answers[id] = answer;
                    return true;
                }
                
                string ShowAnswer(int id) {
                    return answers[id];
                }
        } questions[128];
    public:
        void Start();
        bool AddQuestion(int, const string);
        bool QuestionAddAnswer(int, int, const string);
};

//class quest::question
bool quest::QuestionAddAnswer (int id, int answer, const string str) {
   
    return true;
}

bool quest::AddQuestion (int id, const string str) {
    questions[id].SetQuestion(str);
    return true;
}

void quest::Start () {
    AddQuestion(1, "û?)");
    questions[1].AddAnswer(1, "loool");
    cout << questions[1].ShowAnswer(1) << endl;
}

int main () {                
    float f = .0;
    quest BepeQuest;
    BepeQuest.Start();
    cout << rdtsc();
    cin.get();
    return 0;
}

