#include "header.h"

int main(int argc, char *argv[])
{
    int fd, i, cli_pipe, res;
    char str[80];
    char cli_pipe_name[20];
    PEDIDO p;

    if( access(PIPE_SERV, F_OK) ){
        printf("Não existe o pipe %s\n", PIPE_SERV );
        return 0;
    }

    p.pid = getpid();
    sprintf(cli_pipe_name ,"PIPE_CLI%d", p.pid );

    mkfifo(cli_pipe_name, 0600);
    cli_pipe = open(cli_pipe_name,  O_RDWR);

    fd = open(PIPE_SERV, O_WRONLY|O_CREAT, 0600);
    do {
        printf("NUM1? " );
        scanf("%d", &p.n1);
        printf("NUM2? " );
        scanf("%d", &p.n2);
        printf("OP? " );
        scanf(" %c", &p.op);
        printf("PEDIDO : %d%c%d \n", p.n1, p.op, p.n2 );

        i = write(fd, &p, sizeof(p));
        printf("Enviei um pedido ...(%d bytes)\n", sizeof(p) );
        read(cli_pipe, &res, sizeof(res));
        printf("RESULTADO: %d \n", res);
    } while (p.op != '.' && p.op != '!');
    close(cli_pipe);
    close(fd);
    unlink(cli_pipe_name);
    return 0;
}
