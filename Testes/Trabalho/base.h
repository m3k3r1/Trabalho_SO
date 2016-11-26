#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define LOGS_FILE "cli_login.txt"
#define SRV_FIFO "server_fifo"
#define CLI_FIFO "client_%d_fifo"

typedef struct {
    char usr[20];
    char pss[20];
    bool auth;
} login_t;

typedef struct {
    int pid;
} cli_info_t;
