#include "thd_mgmt.h"
#include "usr_ui.h"


void* rdr_map(WIN* p_win){
    int ch;

    create_box(p_win);
    while( ((ch = getch()) != KEY_F(1))){
        refresh();
        create_box(p_win);
    }

    pthread_exit(0);
};
