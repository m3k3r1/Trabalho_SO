#ifndef _SESSION_MGMT_H_
#define  _SESSION_MGMT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>

#define width 51 // COMPRIMENTO
#define height 21 // LARGURA

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

void startGame(game_stat_t * game, game_control_t ** head, int seconds);
void move(game_control_t * player);
void * PlayerMovement(void * arg);
int moveCheck(int x, int y, game_control_t * head);
int randNum(int min, int max);

#endif
