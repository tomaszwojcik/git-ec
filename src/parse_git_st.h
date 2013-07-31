#ifndef __PARSE_GIT_ST_H
#define __PARSE_GIT_ST_H

#include <vector>
#include <string>

const int PIPE_LINE_BUFFER_SIZE = 256;

class GitStatusParser {
    std::vector<std::string*> pipe_buffer;

    std::string branch;
    std::vector<std::string*> new_files;
    std::vector<std::string*> modified_files;
    std::vector<std::string*> untracked_files;

    public:

    GitStatusParser();
    ~GitStatusParser();
    void parse();

    std::string getBranch() { return branch; }
    std::vector<std::string*>* getNewFiles() { return &new_files; }
    std::vector<std::string*>* getModifiedFiles() { return &modified_files; }
    std::vector<std::string*>* getUntrackedFiles() { return &untracked_files; }

    private:

    void load();
    void parseBranch();
    void parseNewFiles();
    void parseModifiedFiles();
    void parseUntrackedFiles();

};

#endif
