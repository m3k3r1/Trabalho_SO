#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>

#define SRV_FIFO "res/server_fifo"
#define CLI_FIFO "res/client_%d_fifo"

void cmd_control(char *cmd, char* arg1, char* arg2);

#endif
