#include "parse_git_st.h"
#include "regex_helper.h"

#include <iostream>
#include <cstdio> 
#include <cstdlib>
#include <cstring>

using namespace std;

GitStatusParser::GitStatusParser() {
    regex::initialize();
}

GitStatusParser::~GitStatusParser() {
    pipe_buffer.clear();
    new_files.clear();
}

void GitStatusParser::parse() {
    load();
    parseBranch();
    parseNewFiles();

    //debug
    vector<string*>::iterator it;
    for (it = pipe_buffer.begin(); it != pipe_buffer.end(); it++) {
        if (regex::isBranchHeader(*it)) {
            cout << "BRANCH" << *regex::getBranch(*it);
        }
        cout << **it << endl;
    }
}

void GitStatusParser::load() {
    //TODO rewrite this method from C + C++ to pure C++ style (and throw exception if necessary?).
    FILE *file = popen("git status", "r");
    if (!file) {
        cerr << "IO error!" << endl;
        exit(1);
    }
    int bytes;
    char buffer[PIPE_LINE_BUFFER_SIZE];
    pipe_buffer.clear();
    while (fgets(buffer, sizeof(buffer), file) != 0) {
        buffer[strlen(buffer) - 1] = '\0'; // we don't need '\n' at the end
        string* str = new string(buffer);
        pipe_buffer.push_back(str);
    }
    pclose(file);
}

void GitStatusParser::parseBranch() {
    vector<string*>::iterator it;
    for (it = pipe_buffer.begin(); it != pipe_buffer.end(); it++) {
        if (regex::isBranchHeader(*it)) {
            branch = regex::getBranch(*it);
            pipe_buffer.erase(it);
            return;
        }
    }
}

void GitStatusParser::parseNewFiles() {
}
