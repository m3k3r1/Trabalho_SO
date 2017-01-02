#ifndef _SESSION_MGMT_H_
#define  _SESSION_MGMT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define width 51 // COMPRIMENTO
#define height 21 // LARGURA

typedef struct{
    int game_result[2];
    int seconds;
} game_stat_t;

typedef struct game_control_t game_control_t;
struct game_control_t {
    int role, posX, posY, id;
    pid_t user;
    game_control_t * next;
    //bool have_b all;
};

#endif
