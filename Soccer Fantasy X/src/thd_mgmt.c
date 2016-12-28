#include "thd_mgmt.h"

void* init_fld(void* th){
    int ch;

    initscr();
    start_color();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_win_params(th);
    attron(COLOR_PAIR(1));
    printw("Press F1 to exit");
    refresh();
    attroff(COLOR_PAIR(1));
    create_box(th);

    while((ch = getch()) != KEY_F(1)){
        refresh();
        create_box(th);
    }
    pthread_exit(0);
};
