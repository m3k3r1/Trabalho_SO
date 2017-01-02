#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define PIPE_SERV "PIPE_SERV"

typedef struct {
    int n1, n2;
    char op;
    pid_t pid;
} PEDIDO;
