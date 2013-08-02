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
    MENU* menu = new_menu(items);
    menu_opts_off(menu, O_ONEVALUE);
    return menu;
}

string* menu_item_to_string(ITEM* item) {
    const char* name = item->name.str;
    return new string(name);
}

bool is_selected_any_file(MENU* menu) {
    ITEM** items = menu_items(menu);
    int count = item_count(menu);
    for (int i = 0; i < count; i++) {
        if (item_value(items[i]) == TRUE) {
            return true;
        }
    }
}

void add_selected_files(MENU* menu, GitCommitter* git_committer) {
    ITEM** items = menu_items(menu);
    int count = item_count(menu);
    for (int i = 0; i < count; i++) {
        if (item_value(items[i]) == TRUE) {
            string* filename = menu_item_to_string(items[i]);
            git_committer->addFile(filename);
        }
    }
}

//TODO add ctrl-c handling (cleanup)
int main(int argc, char** argv) {
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    noecho();

    GitStatusParser git_st_parser;
    GitCommitter git_committer;
    git_st_parser.parse();

    MENU* files_menu = init_all_files_menu(&git_st_parser);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);

    set_menu_fore(files_menu, COLOR_PAIR(1) | A_REVERSE);
    set_menu_grey(files_menu, COLOR_PAIR(3) | A_REVERSE);

    move(5, 5);
    post_menu(files_menu);
    refresh();

    bool done = false;
    int key = 0;
    do {
        key = getch();
        switch (key) {
            case KEY_DOWN:
            case 'j':
                menu_driver(files_menu, REQ_DOWN_ITEM);
                break;

            case KEY_UP:
            case 'k':
                menu_driver(files_menu, REQ_UP_ITEM);
                break;

            case KEY_PPAGE:
                menu_driver(files_menu, REQ_SCR_UPAGE);
                break;

            case KEY_NPAGE:
                menu_driver(files_menu, REQ_SCR_DPAGE);
                break;

            case ' ':
                menu_driver(files_menu, REQ_TOGGLE_ITEM);
                break;

            case 'c': 
                if (is_selected_any_file(files_menu)) {
                    add_selected_files(files_menu, &git_committer);
                    free_menu(files_menu);
                    endwin();
                    git_committer.commit();
                    return 0;
                }
                break;

            case 'q':
            case 27: //ESC
                done = true;
                break;
        }
        post_menu(files_menu);
        refresh();
    } while (!done);
    free_menu(files_menu);
    endwin();
    return 0;
}

