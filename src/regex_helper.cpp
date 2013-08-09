#include "regex_helper.h"
#include <iostream>
#include <cstdlib>
#include <regex.h>

using namespace std;

const char* not_a_git_repository_pattern = "fatal";
const char *branch_header_pattern = "^#\\s*On branch\\s*";
const char* new_files_pattern = "^#\\s*new file:\\s*";
const char* modified_file_pattern = "^#\\s*modified:\\s*";
const char* untracked_file_pattern = "^#\\s*[a-zA-Z0-9._/\\-]+$";
const char* indent_pattern = "^#\\s*";

static bool initalized = false;
static regex_t not_a_git_repository;
static regex_t branch_header;
static regex_t new_file;
static regex_t modified_file;
static regex_t untracked_file; //regex for whole line (including filename)
static regex_t indent; //we will remove indent (using regex) from untracked file

// Helper functions
void compileRegex(regex_t *regex, const char *pattern, bool ignoreCase = false) {
    int flags = REG_EXTENDED | REG_NEWLINE | REG_ENHANCED;
    if (ignoreCase) {
        flags |= REG_ICASE;
    }
    int result = regcomp(regex, pattern, flags);
    if (result) {
        cerr << "Failed to compile regular expression: \"";
        switch (result) {
            case REG_NOMATCH: cerr << "The regexec() function failed to match"; break;
            case REG_BADPAT: cerr << "invalid regular expression"; break;
            case REG_ECOLLATE: cerr << "invalid collating element"; break;
            case REG_ECTYPE: cerr << "invalid character class"; break;
            case REG_EESCAPE: cerr << "`\' applied to unescapable character"; break;
            case REG_ESUBREG: cerr << "invalid backreference number"; break;
            case REG_EBRACK: cerr << "brackets `[ ]' not balanced"; break;
            case REG_EPAREN: cerr << "parentheses `( )' not balanced"; break;
            case REG_EBRACE: cerr << "braces `{ }' not balanced"; break;
            case REG_BADBR: cerr << "invalid repetition count(s) in `{ }'"; break;
            case REG_ERANGE: cerr << "invalid character range in `[ ]'"; break;
            case REG_ESPACE: cerr << "ran out of memory"; break;
            case REG_BADRPT: cerr << "i`?', `*', or `+' operand invalid"; break;
            case REG_EMPTY: cerr << "empty (sub)expression"; break;
            case REG_ASSERT: cerr << "cannot happen - you found a bug"; break;
            case REG_INVARG: cerr << "invalid argument, e.g. negative-length string"; break;
            case REG_ILLSEQ: cerr << "iillegal byte sequence (bad multibyte character)"; break;
        }
        cerr << "\"." << endl;
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
        compileRegex(&not_a_git_repository, not_a_git_repository_pattern, true);
        compileRegex(&branch_header, branch_header_pattern);
        compileRegex(&new_file, new_files_pattern);
        compileRegex(&modified_file, modified_file_pattern);
        compileRegex(&untracked_file, untracked_file_pattern);
        compileRegex(&indent, indent_pattern);
    }
}

bool regex::isNotAGitRepository(string* git_st_line) {
    return 0 == regexec(&not_a_git_repository, git_st_line->c_str(), 0, 0, 0);
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

bool regex::isModifiedFile(string* git_st_line) {
    return 0 == regexec(&modified_file, git_st_line->c_str(), 0, 0, 0);
}

string* regex::getModifiedFile(string* git_st_line) {
    return getStringWithoutMatch(&modified_file, git_st_line);
}

bool regex::isUntrackedFile(string* git_st_line) {
    return 0 == regexec(&untracked_file, git_st_line->c_str(), 0, 0, 0);
}

string* regex::getUntrackedFile(string* git_st_line) {
    return getStringWithoutMatch(&indent, git_st_line);    
}


