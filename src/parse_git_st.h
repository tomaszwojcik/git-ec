#ifndef __PARSE_GIT_ST_H
#define __PARSE_GIT_ST_H

#include <vector>
#include <string>

const int PIPE_LINE_BUFFER_SIZE = 256;

class GitStatusParser {
    std::vector<std::string*> pipe_buffer;

    std::string* branch;
    std::vector<std::string*> new_files;

    public:

    GitStatusParser();
    ~GitStatusParser();
    void parse();

    private:

    void load();
    void parseBranch();
    void parseNewFiles();

};

#endif
