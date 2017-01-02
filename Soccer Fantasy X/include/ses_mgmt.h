#ifndef _SESSION_MGMT_H_
#define  _SESSION_MGMT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
<<<<<<< HEAD
#include <time.h>
#include <pthread.h>
=======
#include "stdbool.h"
>>>>>>> master

typedef struct game_stat_t game_stat_t;
 struct game_stat_t{
    int game_result[1];
    int dur;
    game_stat_t* next_game;
} ;

<<<<<<< HEAD
typedef struct{
    int game_result[2];
    int run;
    int seconds, numPlayers;
} game_stat_t;

typedef struct game_control_t game_control_t;
struct game_control_t {
    int role, posX, posY, id;
    pthread_t tid;
    game_control_t * next, * head;
    //bool have_b all;
};
=======
typedef struct {
    int player;
    int pos_x;
    int pos_y;
    bool have_ball;
} game_control_t;

void set_game(game_stat_t** head, int t);
game_stat_t* new_game(int t);
>>>>>>> master

void startGame(game_stat_t * game, game_control_t ** head, int seconds);
void move(game_control_t * player);
void * PlayerMovement(void * arg);
int moveCheck(int x, int y, game_control_t * head);
int randNum(int min, int max);

#endif
