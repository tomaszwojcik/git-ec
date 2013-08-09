#include "parse_git_st.h"
#include "git_committer.h"
#include "exceptions.h"
#include <ncurses.h>
#include <menu.h>
#include <iostream>
#include <sstream>

using namespace std;

MENU* init_all_files_menu(GitStatusParser* git_status_parser, WINDOW* window) {
    vector<string*>* new_files = git_status_parser->getNewFiles();
    vector<string*>* modified_files = git_status_parser->getModifiedFiles();
    vector<string*>* untracked_files = git_status_parser->getUntrackedFiles();
    int total_files = new_files->size() + modified_files->size() + untracked_files->size();


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
    set_menu_win(menu, window);
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

void draw_title_line() {
    const char* title = "GIT EASY COMMIT";
    int x, y;
    attron(COLOR_PAIR(3) | A_REVERSE | A_BOLD);
    getmaxyx(stdscr, y, x);
    for (int i = 0; i < x; i++) {
        mvaddch(0, i, ' ');
    }
    mvprintw(0, (x / 2) - (strlen(title) / 2), title);
    attroff(A_REVERSE | A_BOLD);
}

void draw_help() {
    attron(COLOR_PAIR(1));
    mvprintw(1, 1, "<up>, k - navigate up, <down>, j - navigate down");;
    mvprintw(2, 1, "<space> - select/deselect file, c - commit selected file(s)");
    mvprintw(3, 1, "<esc>, q - quit");
    attron(A_BOLD);
    mvprintw(5, 1, "All files");
    attroff(A_BOLD);
}

// Global variables
WINDOW* menu_window;
MENU* files_menu;
GitStatusParser git_st_parser;
GitCommitter git_committer;
bool terminated = false;

void init_git_parser() {
    try {
        git_st_parser.parse();
    } catch (NotAGitRepositoryException e) {
        cerr << "Missing git repository! Use git-ec in the git repository or any of it's subdirectories." << endl;
        cerr.flush();
        exit(1);
    }
}

void init_curses() {
    initscr();
    keypad(stdscr, TRUE);
    start_color();
    noecho();
    curs_set(0);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
}

void init_menu_window() {
    int rows, cols;
    int menu_y = 6;
    getmaxyx(stdscr, rows, cols);
    int menu_height = (rows - menu_y) - 2;
    if (menu_height < 2) {
        menu_height = 2;
    }
    menu_window = newwin(menu_height, cols, 6, 0);
    files_menu = init_all_files_menu(&git_st_parser, menu_window);
    set_menu_format(files_menu, menu_height, 1);
    set_menu_fore(files_menu, COLOR_PAIR(2) | A_REVERSE | A_BOLD);
    set_menu_grey(files_menu, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
    post_menu(files_menu);
    refresh();
    wrefresh(menu_window);
}

void cleanup() {
    if (files_menu) {
        free_menu(files_menu);
    }
    endwin();
}

//TODO add ctrl-c handling (cleanup)
int main(int argc, char** argv) {
    init_git_parser();
    init_curses();
    init_menu_window();
    
    draw_title_line();
    draw_help();

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
        wrefresh(menu_window);
        refresh();
    } while (!done);
    free_menu(files_menu);
    endwin();
    return 0;
}

