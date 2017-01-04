#include "usr_mgmt.h"
#include "sig_mgmt.h"
#include "thd_mgmt.h"
#include "ses_mgmt.h"
#include "usr_ui.h"

int EXIT = 1;

int main(int argc, char const *argv[]) {
    int srv_fd, cli_fd;
    char cli_pipe_name[20];
    login_t cli_log;
    cli_info_t cli_data;
    pthread_t curses;
    bool game_start = false;
    WIN win;
    game_stat_t game;
    game_control_t * head = NULL;

    signal(SIGUSR1, signal_handler_cli);
    signal(SIGALRM, signal_handler_cli);

    if( access(SRV_FIFO, F_OK) ){
        perror("[ERROR]Server not found \n");
        exit(1);
    }else
        srv_fd = open(SRV_FIFO, O_WRONLY|O_CREAT, 0600);

    cli_data.pid = getpid();
    sprintf(cli_pipe_name , CLI_FIFO, cli_data.pid );
    mkfifo(cli_pipe_name, 0600);
    cli_fd = open(cli_pipe_name,  O_RDWR);

    while(true && EXIT) {
        set_crd( cli_log.usr, cli_log.pss);

        write(srv_fd, &cli_data, sizeof(cli_data));
        write(srv_fd, &cli_log, sizeof(cli_log));

        read(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
        printf("%s\n", cli_log.auth ? "[LOGIN SUCCESFULL]" : "[LOGIN FAILED]");

        if ( !cli_log.auth) {
            fprintf(stderr, "Username or password not valid\n" );
            break;
        }else{
            read(cli_fd, &game_start, sizeof(game_start));

            if (game_start) {
              read(cli_fd, &game, sizeof(game));
              read(cli_fd, &head, sizeof(head));
                    alarm(game.seconds);
                    init_fld(&win);
                    init_players(&win, head);

                    while (EXIT) {
                        pthread_create(&curses,NULL, rdr_map(&win), NULL);
                        read(cli_fd, &game_start, sizeof(game_start));
                    }
                    EXIT = 0;
                    endwin();
                break;
            }
        }
    }

    close(cli_fd);
    close(srv_fd);
    unlink(cli_pipe_name);
    unlink(SRV_FIFO);
    exit(0);
}

void signal_handler_cli(int sig){
    if (sig == SIGUSR1){
        printf("[SERVER] - Shuting down\n");
        EXIT = 0;
    }
    if(sig == SIGALRM){
        printf("[SERVER] - Game ended\n");
        EXIT = 0;
    }
}
