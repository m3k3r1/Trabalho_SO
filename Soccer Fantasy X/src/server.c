#include "base.h"
#include "usr_mgmt.h"

int main(int argc, char *argv[]){
    int srv_fd, cli_fd;
    char cli_pipe_name[20], cmd[20], filename[20];
    bool game_allow = false;
    login_t cli_log;
    cli_info_t cli_data;
    player_t* player_list = NULL;

    fd_set conj;
    struct timeval tempo;
    int ret;

    if(argc > 0)
      strcpy(filename, argv[1]);

    if( !access(SRV_FIFO, F_OK) )
    {
      printf("Another server running");
      exit(1);
    }

    mkfifo(SRV_FIFO , 0600);
    srv_fd = open(SRV_FIFO, O_RDWR);

    do {

        FD_ZERO(&conj);
        FD_SET(0, &conj);
        FD_SET(srv_fd, &conj);
        tempo.tv_sec = 1;
        tempo.tv_usec = 500000;

        ret = select(srv_fd + 1, &conj, NULL, NULL , &tempo);

        if (!ret) {
            //printf(".");
            //fflush(stdout);
        }else if (ret > 0){
            if (FD_ISSET(0, &conj)) {
                scanf("%s" ,cmd);
                if (!strcmp(cmd, "users")) {
                    list_player(player_list);
                }
                if (!strcmp(cmd, "user")) {
                    //sign_in();
                }
                if (!strcmp(cmd, "start") && game_allow) {
                    write(cli_fd, &game_allow, sizeof(game_allow));
                }else if(!strcmp(cmd, "start") && !game_allow){
                    printf("[GAME]No players connected \n");
                }
            }
            if (FD_ISSET(srv_fd, &conj)){
                read(srv_fd, &cli_data, sizeof(cli_data));
                read(srv_fd, &cli_log, sizeof(cli_log));
                printf("%s-%s", cli_log.usr, cli_log.pss );

                sprintf(cli_pipe_name , CLI_FIFO, cli_data.pid );
                cli_fd = open(cli_pipe_name, O_WRONLY|O_CREAT, 0600);

                cli_log.auth = usr_auth(cli_log.usr, cli_log.pss, filename);

                if (cli_log.auth){
                        if (chk_player(player_list ,cli_log.usr)) {
                            fprintf(stderr, "Player %s already logged in server\n" , cli_log.usr);
                            cli_log.auth = false;
                            write(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
                        }else{
                            printf("%s\n", cli_log.auth ? "[LOGIN SUCCESFULL]" : "[LOGIN FAILED]");
                            save_player(&player_list ,cli_log.usr, cli_data.pid);
                            write(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
                            game_allow = true;
                        }
                }else{
                    write(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
                    printf("%s\n", cli_log.auth ? "[LOGIN SUCCESFULL]" : "[LOGIN FAILED]");
                }
            }
        }
    }while (strcmp(cmd, "shutdown"));

    close(cli_fd);
    close(srv_fd);
    unlink(cli_pipe_name);
    unlink(SRV_FIFO);
    exit(0);
}
