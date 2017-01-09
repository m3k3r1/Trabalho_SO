#ifndef USR_UI_H
#define USR_UI_H
#include <ncurses.h>
#include "base.h"

struct pos{
    int x, y;
};

struct thread_data{
    WINDOW *field;
    struct pos p;
};

// CREATES WINDOW FOR GAME
WINDOW* crt_win(int height, int width, int start_x, int start_y);

// REFRESH PLAYER POS
void move_pl(WINDOW* field, int y, int x,int color, int role);

// RENDER GAME PLAYERS IN FIELD
void render_players( WINDOW * field, int x, int y,int color,int role);

// CLEANS GAME FIELD
void clean_pos( WINDOW ** f);


#endif
