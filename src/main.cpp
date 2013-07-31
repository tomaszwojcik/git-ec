#include "parse_git_st.h"
#include <iostream>

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

int main(int argc, char** argv) {
    GitStatusParser git_status_parser;
    git_status_parser.parse();
    debug_print(&git_status_parser);
    return 0;
}

