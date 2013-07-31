#include "regex_helper.h"

#include <iostream>
#include <cstdlib>
#include <regex.h>

using namespace std;

const char *branch_header_pattern = "^#\\s*On branch\\s*";
const char* new_files_pattern = "^#\\s*new file:\\s*";

static bool initalized = false; 
static regex_t branch_header;
static regex_t new_file;

// Helper functions
void compileRegex(regex_t *regex, const char *pattern) {
    int result = regcomp(regex, pattern, REG_EXTENDED | REG_NEWLINE);
    if (result) {
        cerr << "Failed to compile regular expression!" << endl;
        exit(1);
    }
}

string* getStringWithoutMatch(regex_t *regex, string* git_st_line) {
    regmatch_t match[1];
    if (!regexec(regex, git_st_line->c_str(), 1, match, 0)) {
        string sub = git_st_line->substr(match[0].rm_eo);
        string* result = new string(sub);
        return result;
    }
    return 0;
}

// Actual functions
void regex::initialize() {
    if (!initalized) {
        compileRegex(&branch_header, branch_header_pattern);
        compileRegex(&new_file, new_files_pattern);
    }
}

bool regex::isBranchHeader(string* git_st_line) {
    return 0 == regexec(&branch_header, git_st_line->c_str(), 0, 0, 0);
}

string* regex::getBranch(string* git_st_line) {
    return getStringWithoutMatch(&branch_header, git_st_line);
}

bool regex::isNewFile(string* git_st_line) {
    return 0 == regexec(&new_file, git_st_line->c_str(), 0, 0, 0);
}

string* regex::getNewFile(string* git_st_line) {
    return getStringWithoutMatch(&new_file, git_st_line);
}
