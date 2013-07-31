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
    modified_files.clear();
}

void GitStatusParser::parse() {
    load();
    parseBranch();
    parseNewFiles();
    parseModifiedFiles();
    parseUntrackedFiles();
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

//TODO create a method to iterate over pipe buffer and pass specific methods for parsing as an argument

void GitStatusParser::parseBranch() {
    vector<string*>::iterator it = pipe_buffer.begin();
    while (it != pipe_buffer.end()) {
        if (regex::isBranchHeader(*it)) {
            branch = regex::getBranch(*it);
            pipe_buffer.erase(it);
            return;
        } else {
            it++;
        }
    }
}

void GitStatusParser::parseNewFiles() {
    vector<string*>::iterator it = pipe_buffer.begin();
    while (it != pipe_buffer.end()) {
        if (regex::isNewFile(*it)) {
            new_files.push_back(regex::getNewFile(*it));
            pipe_buffer.erase(it);
        } else {
            it++;
        }
    }
}

void GitStatusParser::parseModifiedFiles() {
    vector<string*>::iterator it = pipe_buffer.begin();
    while (it != pipe_buffer.end()) {
        if (regex::isModifiedFile(*it)) {
            modified_files.push_back(regex::getModifiedFile(*it));
            pipe_buffer.erase(it);
        } else { 
            it++;
        }
    } 
}

void GitStatusParser::parseUntrackedFiles() {
    vector<string*>::iterator it = pipe_buffer.begin();
    while (it != pipe_buffer.end()) {
        if (regex::isUntrackedFile(*it)) {
            untracked_files.push_back(regex::getUntrackedFile(*it));
            pipe_buffer.erase(it);
        } else { 
            it++;
        }
    } 
    
}
