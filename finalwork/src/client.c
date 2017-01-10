#include "base.h"
#include "usr_mgmt.h"
#include "usr_ui.h"

bool cli_shutdown = false;
int EXIT = 0;

void signal_handler_cli(int sig);

int main(int argc, char const *argv[]) {
  int srv_fd, cli_fd;         // FIFO CONNECTION
  int color = 0;
  int tryHard = 0;            // TRY HARD VAR FOR CLIENT LOCKOUT
  int n_bytes;
  int ch, start_y = 0, start_x = 0, height = 21, width = 51;
  char cli_pipe_name[20];     // CLIENT NAME WITH PID
  login_t cli_log;            // USER CREDENTIALS
  pid_t cli_pid;              // CLIENT PID
  WINDOW * field;
  cli_game_t game;
  cli_player_t * player_list = NULL;
  // CLIENT AUTH BOOL
  cli_log.auth = false;

  // SIGNAL TO KILL SERVER
  signal(SIGUSR1, signal_handler_cli);

  if(access(SRV_FIFO, F_OK))
  {
    perror("[ERROR] Server not found\n");
    exit(1);
  }
  else
    srv_fd = open(SRV_FIFO, O_WRONLY|O_CREAT, 06000);

  // GET PID OF CLIENT
  cli_pid = getpid();
  // STR CAT CLI NAME FIFO WITH PID
  sprintf(cli_pipe_name, CLI_FIFO, cli_pid);

  // MAKE CLI FIFO
  mkfifo(cli_pipe_name, 0600);
  cli_fd = open(cli_pipe_name, O_RDWR);

  while(!cli_log.auth && !cli_shutdown)
  {
    // ASKS USERNAME AND PASS OF CLIENT
    set_cred(cli_log.usr, cli_log.pass);

    // SENDS CLIENT PID AND INSERTED CREDS
    write(srv_fd, &cli_pid, sizeof(cli_pid));
    write(srv_fd, &cli_log, sizeof(cli_log));

    // RECEIVES A BOOL FOR AUTH
    read(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
    printf("%s\n", cli_log.auth ? "[LOGIN SUCCESFULL]" : "[LOGIN FAILED]");

    // CLIENT CREDS ARE INVALID
    if((!cli_log.auth))
    {
      fprintf(stderr, "Username and/or password not valid\n");
      tryHard++;
    }

    // TRYHARD FOR NO MORE THAN 3 LOGIN ATTEMPTS
    if(tryHard > 3)
      break;
  }

  // AFTER LOGGED IN
  while (cli_log.auth && !cli_shutdown) {
    // READ CLIENT GAME STRUCT
    do {
      n_bytes = read(cli_fd, &game, sizeof(game));
  } while(n_bytes == 0 && !cli_shutdown);

    // CLI PLAYER STRUCT MALLOC
    player_list = malloc(sizeof(cli_player_t) * game.numPlayers*2);

    fd_set conj;
    struct timeval tempo;
    int ret;

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);

    start_y = (LINES - height) / 2;
    start_x = (COLS - width) / 2;

    attron(COLOR_PAIR(1));
    printw("Press q to exit");
    refresh();
    attroff(COLOR_PAIR(1));

    field = crt_win( height,  width,  start_x,  start_y);


    do {
        FD_ZERO(&conj);
        FD_SET(0, &conj);
        FD_SET(cli_fd,&conj);

        tempo.tv_sec = 1;
        tempo.tv_usec = 500000;
        ret = select( cli_fd + 1, &conj, NULL, NULL , &tempo);

        if (ret > 0){
            if (FD_ISSET(0, &conj))
                ch = getchar();
            if (FD_ISSET(cli_fd, &conj)){
              clean_pos(&field);
              wrefresh(field);

              for(int i = 0; i < game.numPlayers * 2; i++)
              {
                read(cli_fd, &player_list[i], sizeof(cli_player_t));

                if (i <= game.numPlayers)
                  color = 1;
                else
                  color = 2;

                render_players(field, player_list[i].posX, player_list[i].posY, color, player_list[i].role);

              }
            }
          }
    } while( ch != 'q' && !cli_shutdown);
  }


  delwin(field);
  endwin();


  //free
  free(player_list);

  // CLOSE FIFO CONNECTION
  close(cli_fd);
  close(srv_fd);

  // REMOVES FIFO FILE
  unlink(cli_pipe_name);

  return 0;
}

void signal_handler_cli(int sig){
    if (sig == SIGUSR1) {
        cli_shutdown = true;
        puts("Shutdown Signal from server");
        exit(0);
    }
}
