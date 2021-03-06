#include "header.h"

int main(int argc, char *argv[])
{
    int fd, cli_pipe, n_bytes, res, x = 0;
    char str[80];
    char cli_pipe_name[20], cmd[20];
    pid_t  cli[5];
    PEDIDO p;

    fd_set conj;
    struct timeval tempo;
    int ret;

    mkfifo(PIPE_SERV , 0600);
    fd = open(PIPE_SERV, O_RDWR);
    printf("Chegou o primeiro cliente ...\n" );

    do {

        FD_ZERO(&conj);
        FD_SET(0, &conj);
        FD_SET(fd, &conj);
        tempo.tv_sec = 1;
        tempo.tv_usec = 500000;

        ret = select(fd + 1, &conj, NULL, NULL , &tempo);
        if (!ret) //Theres no data
        {
            //printf(".");
            fflush(stdout);
        }
        else if( ret > 0) // Theres data
        {
            if (FD_ISSET(0, &conj)) { // Theres daa in the keyboard
                scanf("%s" ,cmd);
                if ( !strcmp(cmd, "mostra")) {
                    for (size_t i = 0; i < 5; i++){
                            printf("Cliente %d - %d \n", (i + 1), cli[i] );
                    }
                }
            }

            if (FD_ISSET(fd, &conj)) { //Theres data in the fifo
                n_bytes = read(fd, &p, sizeof(p));
                sprintf(cli_pipe_name ,"PIPE_CLI%d", p.pid );
                cli[x] = p.pid;
                x++;
                if (n_bytes == sizeof(p) ) {
                    printf("Chegou um pedido ... (%d bytes)\n" ,n_bytes);
                    printf("PEDIDO : %d%c%d \n", p.n1, p.op, p.n2 );
                    cli_pipe = open(cli_pipe_name, O_WRONLY|O_CREAT, 0600);
                    sleep(5);
                    res = p.n1 + p.n2;
                    write( cli_pipe, &res, sizeof(res) );
                }
            }
        }
    } while( p.op != '!' && strcmp(cmd, "exit"));
    close(cli_pipe);
    close(fd);
    unlink(PIPE_SERV);
    return 0;
}
