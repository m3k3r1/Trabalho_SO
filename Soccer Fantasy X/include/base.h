#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define SRV_FIFO "res/server_fifo"
#define CLI_FIFO "res/client_%d_fifo"

typedef struct {
    pid_t pid;
} cli_info_t;

typedef struct {
    int game_result;
} game_stat_t;

void cmd_control(char *cmd, char* arg1, char* arg2);
