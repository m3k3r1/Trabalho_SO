#ifndef _USR_MGMT_H_
#define _USR_MGMT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LOGS_FILE "res/cli_login.txt"

typedef struct {
    char usr[20];
    char pss[20];
    bool auth;
} login_t;

typedef struct player_t player_t;
struct player_t {
    char usr_name[20];
    pid_t pid;
    player_t* nxt_player;
} ;

void set_crd(char usr[20], char pss[20]);
bool usr_auth(char usr[20], char pss[20], char * filename);
void save_player(player_t** head, char usr[20], int pid);
player_t* add_player( char usr[20], int pid);
bool chk_player(player_t * head ,char usr[20]);
int list_player();
//void sign_in();
//void free_node();

#endif
