#ifndef _SESSION_MGMT_H_
#define  _SESSION_MGMT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stdbool.h"

typedef struct game_stat_t game_stat_t;
 struct game_stat_t{
    int game_result[1];
    int dur;
    game_stat_t* next_game;
} ;

typedef struct {
    int player;
    int pos_x;
    int pos_y;
    bool have_ball;
} game_control_t;

void set_game(game_stat_t** head, int t);
game_stat_t* new_game(int t);

#endif
