#include "base.h"
#include "usr_mgmt.h"

int main(int argc, char *argv[])
{
    int srv_fd, cli_fd;
    char cli_pipe_name[20];
    login_t cli_log;
    cli_info_t cli_data;
    player_t* player_list;

    mkfifo(SRV_FIFO , 0600);
    srv_fd = open(SRV_FIFO, O_RDWR);

    while( true ) {
        read(srv_fd, &cli_data, sizeof(cli_data));
        read(srv_fd, &cli_log, sizeof(cli_log));
        printf("%s-%s", cli_log.usr, cli_log.pss );

        sprintf(cli_pipe_name , CLI_FIFO, cli_data.pid );
        cli_fd = open(cli_pipe_name, O_WRONLY|O_CREAT, 0600);

        cli_log.auth = usr_auth(cli_log.usr, cli_log.pss);

        if (cli_log.auth){
            if (chk_player(player_list ,cli_log.usr)) {
                fprintf(stderr, "Player %s already logged in server\n" , cli_log.usr);
                break;
            }else{
                printf("%s\n", cli_log.auth ? "[LOGIN SUCCESFULL]" : "[LOGIN FAILED]");
                save_player(&player_list ,cli_log.usr, cli_data.pid);
                write(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
            }
        }
    }

    list_player(player_list);

    close(cli_fd);
    close(srv_fd);
    unlink(cli_pipe_name);
    exit(0);
}
