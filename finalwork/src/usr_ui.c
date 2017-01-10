#include "usr_ui.h"

// CREATES WINDOW FOR GAME
WINDOW* crt_win(int height, int width, int start_x, int start_y){
    WINDOW * game_field;

    game_field = newwin(height, width, start_y, start_x);
    box(game_field, '|' , '-');
    wrefresh(game_field);

    return game_field;
}


// RENDER GAME PLAYERS IN FIELD
void render_players( WINDOW * field, int x, int y, int color,int role){
    wattron(field,COLOR_PAIR(color));
    mvwprintw(field, y, x, "%d", role);
    wattroff(field,COLOR_PAIR(color));
    wrefresh(field);
}

// CLEANS GAME FIELD
void clean_pos( WINDOW ** f){
    for (size_t y = 1; y < 20; y++) {
        for (size_t x = 1; x < 50; x++)
        {
        wattron(*f,COLOR_PAIR(2));
        mvwprintw(*f, y, x, " ");
        wattroff(*f,COLOR_PAIR(2));
        }
    }
}
