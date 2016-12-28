#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    team_t teams[2];
} game_stat_t;

typedef struct
{
  teammate_t * players;
} team_t;

typedef struct
{
  int posX, posY, id;
} teammate_t;
