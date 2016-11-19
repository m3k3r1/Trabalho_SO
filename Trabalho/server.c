#include "header.h"

int main(int argc, char *argv[])
{
    int srv_fd, cli_fd;
    char cli_pipe_name[20];
    char tmp_usr[20];
    char tmp_pss[20];
    FILE *f_log = fopen(LOGS_FILE, "r");
    login_t cli_log;
    cli_info_t cli_data;

    mkfifo(SRV_FIFO , 0600);
    srv_fd = open(SRV_FIFO, O_RDWR);


    //do {
        read(srv_fd, &cli_data, sizeof(cli_data));
        read(srv_fd, &cli_log, sizeof(cli_log));

        printf("%s-%s\n", cli_log.usr, cli_log.pss );

        sprintf(cli_pipe_name , CLI_FIFO, cli_data.pid );
        cli_fd = open(cli_pipe_name, O_WRONLY|O_CREAT, 0600);

        while (fscanf(f_log, "%s %s", tmp_usr, tmp_pss) == 2) {
            if (!strcmp(tmp_usr, cli_log.usr) && !strcmp(tmp_pss, cli_log.pss))
                sprintf(cli_log.auth_buffer, "ACCESS GRANTED");
            else
            sprintf(cli_log.auth_buffer, "ACCESS DENIED");
        }

        printf("%s\n", cli_log.auth_buffer);
        write(cli_fd, &cli_log, sizeof(cli_log));
    //} while( strcmp( cli_log.auth_buffer, "ACCESS GRANTED") );

    close(cli_fd);
    close(srv_fd);
    unlink(cli_pipe_name);
    fclose(f_log);
    exit(0);
}
