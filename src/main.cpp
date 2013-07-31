#include "parse_git_st.h"
#include "git_committer.h"
#include <iostream>
#include <sstream>

using namespace std;

void debug_print(GitStatusParser* parser) {
    vector<string*> *v; 
    vector<string*>::iterator it;
    cout << "Branch: " << parser->getBranch() << endl;
    cout << "1. New files: " << endl;
    v = parser->getNewFiles();
    for (it = v->begin(); it != v->end(); it++) {
        cerr << **it << endl;
    }
    cout << "2. Modified files: " << endl;
    v = parser->getModifiedFiles();
    for (it = v->begin(); it != v->end(); it++) {
        cerr << **it << endl;
    }
    cout << "3. Untracked files: " << endl;
    v = parser->getUntrackedFiles();
    for (it = v->begin(); it != v->end(); it++) {
        cerr << **it << endl;
    }
    
}

void debug_commit_untracked_files() {
    GitStatusParser git_status_parser; 
    GitCommitter git_committer;
    git_status_parser.parse(); 
    vector<string*> files = *git_status_parser.getUntrackedFiles();
    vector<string*>::iterator it;
    for (it = files.begin(); it != files.end(); it++) {    
        git_committer.addFile(*it);
    }
    git_committer.commit("git-cc \"test commit\"");
    return 0;
}

int main(int argc, char** argv) {    
}

