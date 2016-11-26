#ifndef _USR_MGMT_H
#define _USR_MGMT_H

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

void set_crd(char usr[20], char pss[20]);
bool usr_auth(char usr[20], char pss[20]);

#endif
