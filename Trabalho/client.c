#include "header.h"

int main(int argc, char const *argv[]) {
    int srv_fd, cli_fd;
    char cli_pipe_name[20];
    login_t cli_log;
    cli_info_t cli_data;

    if( access(SRV_FIFO, F_OK) ){
        printf("Não existe o pipe %s\n", SRV_FIFO );
        exit(1);
    }else
        srv_fd = open(SRV_FIFO, O_WRONLY|O_CREAT, 0600);

    cli_data.pid = getpid();
    sprintf(cli_pipe_name , CLI_FIFO, cli_data.pid );
    mkfifo(SRV_FIFO, 0600);
    cli_fd = open(cli_pipe_name,  O_RDWR);

    printf ("[USERNAME] : ");
    fgets (cli_log.usr, sizeof(cli_log.usr), stdin);
    strtok (cli_log.usr, "\n");

    fflush (stdin);

    printf ("[PASSWORD] : ");
    fgets (cli_log.pss, sizeof(cli_log.pss), stdin);
    strtok (cli_log.pss, "\n");

    write(srv_fd, &cli_data, sizeof(cli_data));
    write(srv_fd, &cli_log, sizeof(cli_log));

    read(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
    printf("%s\n", cli_log.auth ? "[LOGIN SUCCESFULL]" : "[LOGIN FAILED]");

    close(cli_fd);
    close(srv_fd);
    unlink(cli_pipe_name);
    unlink(SRV_FIFO);
    exit(0);
}
