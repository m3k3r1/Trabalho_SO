#ifndef _USR_MGMT_H
#define _USR_MGMT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LOGS_FILE "res/cli_login.txt"
#define ACTIVE_LOGS "res/act_usr.txt"

typedef struct {
    char usr[20];
    char pss[20];
    bool auth;
} login_t;

typedef struct player_t player_t;
struct player_t {
    char usr_name[20];
    int pid;
    player_t* nxt_player;
} ;

void set_crd(char usr[20], char pss[20]);
bool usr_auth(char usr[20], char pss[20]);
void save_player(player_t** head, char usr[20], int pid);
player_t* add_player( char usr[20], int pid);

#endif
