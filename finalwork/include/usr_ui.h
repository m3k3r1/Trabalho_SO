#ifndef USR_UI_H
#define USR_UI_H
#include "base.h"

// CREATES WINDOW FOR GAME
WINDOW* crt_win(int height, int width, int start_x, int start_y);

// RENDER GAME PLAYERS IN FIELD
void render_players( WINDOW * field, int x, int y,int color,int role);

// CLEANS GAME FIELD
void clean_pos( WINDOW ** f);


#endif
