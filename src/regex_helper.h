#ifndef REGEX_HELPER_H
#define REGEX_HELPER_H

#include <string>

namespace regex {
    void initialize();
    bool isNotAGitRepository(std::string *git_st_line);
    bool isNothingToCommit(std::string *git_st_line);
    bool isBranchHeader(std::string *git_st_line);
    std::string *getBranch(std::string *git_st_line);
    bool isNewFile(std::string* git_st_line);
    std::string* getNewFile(std::string* git_st_line);
    bool isModifiedFile(std::string* git_st_line);
    std::string* getModifiedFile(std::string* git_st_line);
    bool isUntrackedFile(std::string* git_st_line);
    std::string* getUntrackedFile(std::string* git_st_line);
}

#endif

