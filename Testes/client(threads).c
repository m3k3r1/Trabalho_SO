#include "header.h"
#include <pthread.h>

int FIM;

void *receive_msg(void *dados){
    int i, cli_pipe, res;
    char cli_pipe_name[20];
    char str[80];

    sprintf(cli_pipe_name ,"PIPE_CLI%d", getpid() );
    mkfifo(cli_pipe_name, 0600);
    cli_pipe = open(cli_pipe_name,  O_RDWR);
    do {
        i = read(cli_pipe, &res, sizeof(res));
        printf("RESULTADO: %d \n", res);
    } while(!FIM);
    close(cli_pipe);
    unlink(cli_pipe_name);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int fd, i;
    PEDIDO p;
    //make an array for the final work
    pthread_t tarefa;

    if( access(PIPE_SERV, F_OK) ){
        printf("Não existe o pipe %s\n", PIPE_SERV );
        return 0;
    }

    FIM = 0;
    pthread_create(&tarefa, NULL, &receive_msg, NULL);

    p.pid = getpid();
    printf("%d\n", getpid() );
    fd = open(PIPE_SERV, O_WRONLY|O_CREAT, 0600);
    do {
        printf("\n");
        printf("NUM1? " );
        scanf("%d", &p.n1);
        printf("NUM2? " );
        scanf("%d", &p.n2);
        printf("OP? " );
        scanf(" %c", &p.op);
        printf("PEDIDO : %d%c%d \n", p.n1, p.op, p.n2 );
        i = write(fd, &p, sizeof(p));
        printf("Enviei um pedido ...(%d bytes)\n", sizeof(p) );
    } while (p.op != '.' && p.op != '!');

    FIM = 1;
    pthread_join(tarefa, NULL);

    close(fd);
    return 0;
}
