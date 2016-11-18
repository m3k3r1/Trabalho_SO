#include "header.h"

int main(int argc, char *argv[])
{
    int fd, cli_pipe, n_bytes, res;
    char str[80];
    char cli_pipe_name[20];
    PEDIDO p;

    mkfifo(PIPE_SERV , 0600);
    fd = open(PIPE_SERV, O_RDWR);
    //printf("Chegou o primeiro cliente ...\n" );

    do {
        n_bytes = read(fd, &p, sizeof(p));
        sprintf(cli_pipe_name ,"PIPE_CLI%d", p.pid );
        if (n_bytes == sizeof(p) ) {
            printf("Chegou um pedido ... (%d bytes)\n" ,n_bytes);
            printf("PEDIDO : %d%c%d \n", p.n1, p.op, p.n2 );
            cli_pipe = open(cli_pipe_name, O_WRONLY|O_CREAT, 0600);
            res = p.n1 + p.n2;
            write( cli_pipe, &res, sizeof(res) );
        }
    } while( p.op != '!' );


    close(cli_pipe);

    close(fd);
    unlink(PIPE_SERV);
    return 0;
}
