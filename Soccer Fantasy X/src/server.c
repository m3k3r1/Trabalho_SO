#include "base.h"
#include "usr_mgmt.h"
#include "ses_mgmt.h"

int EXIT = 1;

int main(int argc, char *argv[]){
    int srv_fd, cli_fd;
    char cli_pipe_name[20], cmd[20], arg1[20], arg2[20], file_name[20];
    bool game_allow = false, custom_login_file = false;
    login_t cli_log;
    cli_info_t cli_data;
    player_t * player_list = NULL;
    game_stat_t game;
    game_control_t * head = NULL; 

    fd_set conj;
    struct timeval tempo;
    int ret;

    signal(SIGUSR1, signal_handler_srv);

    if (argc > 1){
        custom_login_file = true;
        strcpy(file_name,  argv[1]);
    }

    if( !access(SRV_FIFO, F_OK) ){
        perror("[ERROR]Server already running\n");
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

        if (ret > 0){
            if (FD_ISSET(0, &conj)) {
                fgets(cmd, sizeof(cmd), stdin);
                strtok (cmd, "\n");
                cmd_control(cmd, arg1, arg2);

                if (!strcmp(cmd, "users")) {
                    list_player(player_list);
                }
                if (!strcmp(cmd, "user")) {
                    sign_in(arg1, arg2,file_name, custom_login_file);
                }
                if (!strcmp(cmd, "start") && game_allow) {
                    // TODO START THE GAME
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

                cli_log.auth = usr_auth(cli_log.usr, cli_log.pss, file_name, custom_login_file);

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
    }while (strcmp(cmd, "shutdown") && EXIT);

    exit_warning(player_list);
    free_mem(player_list);
    close(cli_fd);
    close(srv_fd);
    unlink(cli_pipe_name);
    unlink(SRV_FIFO);
    exit(0);
}

void cmd_control(char *cmd, char* arg1, char* arg2){
    int  flag = 1, counter = 0;
    char tmp_cmd[20];

    if(strlen(arg1) !=0 || strlen(arg2) !=0){
        memset(&(*arg1), 0, strlen(arg1));
        memset(&(*arg2), 0, strlen(arg2));
    }

    for (size_t i = 0; cmd[i] != '\0'; i++) {
        if ( cmd[i] == ' ' ) {
            counter = 0;
            flag++;
            continue;
        }
        if (flag == 1) {
            tmp_cmd[i] = cmd[i];
        }
        if (flag == 2) {
            arg1[counter] = cmd[i];
            counter++;
        }
        if (flag == 3) {
            arg2[counter] = cmd[i];
            counter++;
        }
    }
    strcpy(cmd, tmp_cmd);
}
void signal_handler_srv(int sig){
    if (sig == SIGUSR1) {
        printf("[SIGUSR1 RECEIVED] - Shuting down\n");
        EXIT = 0;
    }

}
