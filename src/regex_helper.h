#ifndef REGEX_HELPER_H
#define REGEX_HELPER_H

#include <string>

namespace regex {
    void initialize();
    bool isBranchHeader(std::string *git_st_line);
    std::string *getBranch(std::string *git_st_line);
    bool isNewFile(std::string* git_st_line);
    std::string* getNewFile(std::string* git_st_line);
}

#endif

