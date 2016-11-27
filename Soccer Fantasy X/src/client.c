#include "base.h"
#include "usr_mgmt.h"

int main(int argc, char const *argv[]) {
    int srv_fd, cli_fd;
    char cli_pipe_name[20];
    login_t cli_log;
    cli_info_t cli_data;

    if( access(SRV_FIFO, F_OK) ){
        perror("[ERROR]Server not found \n");
        exit(1);
    }else
        srv_fd = open(SRV_FIFO, O_WRONLY|O_CREAT, 0600);

    cli_data.pid = getpid();
    sprintf(cli_pipe_name , CLI_FIFO, cli_data.pid );
    mkfifo(cli_pipe_name, 0600);
    cli_fd = open(cli_pipe_name,  O_RDWR);

    while(true) {
        set_crd( cli_log.usr, cli_log.pss);

        write(srv_fd, &cli_data, sizeof(cli_data));
        write(srv_fd, &cli_log, sizeof(cli_log));

        read(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
        printf("%s\n", cli_log.auth ? "[LOGIN SUCCESFULL]" : "[LOGIN FAILED]");

        if ( !cli_log.auth) {
            printf("Try again later");
            break;
        }
    }

    close(cli_fd);
    close(srv_fd);
    unlink(cli_pipe_name);
    unlink(SRV_FIFO);
    exit(0);
}
