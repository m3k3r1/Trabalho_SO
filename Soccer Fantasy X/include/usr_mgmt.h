#ifndef _USR_MGMT_H_
#define _USR_MGMT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

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
bool usr_auth(char usr[20], char pss[20], char * file_name, bool custom_file);
void save_player(player_t** head, char usr[20], int pid);
player_t* add_player( char usr[20], int pid);
bool chk_player(player_t * head ,char usr[20]);
int list_player(player_t *head );
void sign_in(char* new_usr, char* new_pwd, char* file_name, bool custom_file);
void exit_warning(player_t* list);
void free_mem(player_t* head);

#endif
