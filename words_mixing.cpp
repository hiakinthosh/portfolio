/*
Program aims to prove that human race is able to read words
in spite of characters in word mixed up in random order but
with the rule that first and last character are unchanged

You have two ways in menu:

1st to transform the core of the word, omitting word's first and last character

2nd to check out if you are able to read the text while
first and last character are swapped as good as the 1st goes
*/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>

using namespace std;

int prepareWord(string &word) {
    int len = word.size();
    if (word[len-1] == '.' || word[len-1] == ',' || word[len-1] == '?' || word[len-1] == '!')
        word.erase(word.size() - 1);
}

void transformCore(const char *fileName, string &word) {
    srand(time(NULL));
    int core_value = (word.size() - 2); // from word[1] to word[word.size() - 1];
                                        // may in future add sth to make in more randomly way that real
    char buf;
    if (core_value > 1) {
        int asd;
        for (int i = 1; i <= core_value; i++) {
            asd = (rand() % core_value) + 1;
            buf = word[i];
            word[i] = word[asd]; // ?
            word[asd] = buf;
        }
    }

    cout << word << " ";

    fstream fs;
    fs.open(fileName, ios::out | ios::app);
    fs << word << " ";
    fs.close();
}

void swapChars(const char *fileName, string &word) {
    char buf;
    int len = word.size();
    buf = word[0];
    word[0] = word[len-1];
    word[len-1] = buf;

    cout << word << " ";

    fstream fs;
    fs.open(fileName, ios::out | ios::app);
    fs << word << " ";
    fs.close();
}

int main() {
    fstream fs;
    char fileName[80];
    cout << "What file you want to parse? ";
    cin >> fileName;
    fs.open(fileName, ios::in | ios::out);
    //fs.open("src.txt", ios::in | ios::out);

    if (!fs.is_open()) {
        cout << "File opening went wrong\n";
        exit(1);
    }

    cout << "\nWhat you want to do?\n"
         << "1. Transform word's core\n"
         << "2. Swap first and last characters in every word\n"
         << "\nChoice goes: ";
    unsigned short choice;
    cin >> choice;

    string word;
    while (!fs.eof()) {
        fs >> word;
        if (word.size() > 2)
            prepareWord(word);
        switch (choice) {
            case 1: transformCore(fileName, word); break;
            case 2: swapChars(fileName, word); break;
            default: cout << "You didn't make a choice\n"; break; exit(1);
        }
    }

    fs.close();
    return 0;
}
