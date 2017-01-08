#ifndef USR_MGMT_H
#define USR_MGMT_H
#include "base.h"
#include "game_mgmt.h"

#define LOGIN_FILE "res/cli_login.txt"

// CLIENT CREDS STRUCT
typedef struct
{
  char usr[20], pass[20];
  bool auth;
} login_t;


// SERVER FUNCTIONS
int list_player(user_t * head);
void sign_up(char* new_usr, char* new_pwd, char* file_name, bool custom_file);
void free_mem(user_t * head);
bool check_user(user_t * head ,char usr[20]);
bool save_user(user_t ** head, char usr[20], int pid);
user_t * add_user(char usr[20], int pid);
bool usr_auth(char usr[20], char pss[20], char * file_name, bool custom_file);
void logout_user(user_t ** head, siginfo_t * cli, void * context);
void exit_warning(user_t * list);
void write_game_cli(user_t * list, game_t * game);
void write_to_clients(user_t * list, bool gamestart);

// CLIENT FUNCTIONS
// READS CLIENT CREDS
void set_cred(char usr[20], char pass[20]);

#endif
