#include "git_committer.h"

#include <iostream>
#include <algorithm>
#include <unistd.h>

using namespace std;

GitCommitter::GitCommitter() {
}

GitCommitter::~GitCommitter() {

}

void GitCommitter::addFile(string* filename) {
    if (find(files.begin(), files.end(), filename) == files.end()) {
        files.push_back(filename);
    }
}

void GitCommitter::removeFile(string* filename) {
    vector<string*>::iterator it = find(files.begin(), files.end(), filename);
    if (it != files.end()) {
        files.erase(it);
    }
}

void GitCommitter::commit() {
    gitAdd();
    system("git commit");
}

void GitCommitter::commit(string message) {
    gitAdd();
    char message_delimiter = '\'';
    if (message.find(message_delimiter) != string::npos) {
        message_delimiter = '\"';
    }
    string git_command = "git commit -m ";
    git_command += message_delimiter;
    git_command += message;
    git_command += message_delimiter;
    system(git_command.c_str());
}

void GitCommitter::gitAdd() {
    vector<string*>::iterator it;
    string git_command = "git add ";
    for (it = files.begin(); it != files.end(); it++) {
        string command(git_command);
        command += (**it);
        system(command.c_str());
    }
}
