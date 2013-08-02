#ifndef GIT_COMMITTER_H
#define GIT_COMITTTER_H

#include <vector>
#include <string>

class GitCommitter {

    std::vector<std::string*> files;

    public:
        GitCommitter();
        ~GitCommitter();    

        void addFile(std::string* filename);
        void removeFile(std::string* filename);
        void commit();
        void commit(std::string message);

    private:
        void gitAdd();  
};

#endif
