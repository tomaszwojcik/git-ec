#include "parse_git_st.h"
#include "git_committer.h"
#include <ncurses.h>
#include <menu.h>
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
}


MENU* init_all_files_menu(GitStatusParser* git_status_parser) {
    vector<string*>* new_files = git_status_parser->getNewFiles();
    vector<string*>* modified_files = git_status_parser->getModifiedFiles();
    vector<string*>* untracked_files = git_status_parser->getUntrackedFiles();
    int total_files = new_files->size() + modified_files->size() + untracked_files->size();

    char buf[256];
    sprintf(buf, "%d", total_files);
    mvprintw(0, 40, buf);
    
            ITEM** items = new ITEM*[total_files  + 1]; //need 1 more space for null entry (end of menu)
    int index = 0;
    for (int i = 0; i < new_files->size(); i++, index++) {
            items[index] = new_item((new_files->at(i))->c_str(), "(new)");
    }
    for (int i = 0; i < modified_files->size(); i++, index++) {
            items[index] = new_item((modified_files->at(i))->c_str(), "(modified)");
    }
    for (int i = 0; i < untracked_files->size(); i++, index++) {
            items[index] = new_item((untracked_files->at(i))->c_str(), "(untracked)");
    }
    items[index] = 0;
    return new_menu(items);
}

int main(int argc, char** argv) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();

    GitStatusParser git_st_parser;
    git_st_parser.parse();

    MENU* files_menu = init_all_files_menu(&git_st_parser);
    move(5, 5);
    post_menu(files_menu);
    refresh();

    bool done = false;
    int key = 0;
    do {
        key = getch();
        switch (key) {
            case KEY_DOWN:
                menu_driver(files_menu, REQ_DOWN_ITEM);
                break;

            case KEY_UP:
                menu_driver(files_menu, REQ_UP_ITEM);
                break;

            case 27: //ESC
                done = true;
                break;
        }
        refresh();
    } while (!done);
    endwin();
}

