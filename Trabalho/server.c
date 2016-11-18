#include "header.h"

int main(int argc, char *argv[])
{
    int srv_fd, cli_fd;
    char cli_pipe_name[20];
    bool log_auth = false;
    login_t cli_log;
    cli_info_t cli_data;

    mkfifo(SRV_FIFO , 0600);
    srv_fd = open(SRV_FIFO, O_RDWR);

    read(srv_fd, &cli_data, sizeof(cli_data));
    read(srv_fd, &cli_log, sizeof(cli_log));

    printf("%s-%s\n", cli_log.usr, cli_log.pss );

    sprintf(cli_pipe_name , CLI_FIFO, cli_data.pid );
    cli_fd = open(cli_pipe_name, O_WRONLY|O_CREAT, 0600);

    write(cli_fd, &log_auth, sizeof(log_auth));

    close(cli_fd);
    close(srv_fd);
    unlink(SRV_FIFO);
    exit(0);
}
