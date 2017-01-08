#include "base.h"
#include "usr_mgmt.h"

bool cli_shutdown = false;
int EXIT = 0;
pthread_mutex_t lock;

struct pos{
    int x, y;
};

struct thread_data{
    WINDOW *field;
    struct pos p;
};

WINDOW* crt_win(int height, int width, int start_x, int start_y);
void move_pl(WINDOW* field, int y, int x);
int rand_p(int min ,int max);
void* thread_mgmt( void *p);
void clean_pos( WINDOW ** f);

int main(int argc, char const *argv[]) {
  int srv_fd, cli_fd;         // FIFO CONNECTION
  int tryHard = 0;            // TRY HARD VAR FOR CLIENT LOCKOUT
  int n_bytes;
  int ch, start_y = 0, start_x = 0, height = 21, width = 51;
  char cli_pipe_name[20];     // CLIENT NAME WITH PID
  login_t cli_log;            // USER CREDENTIALS
  pid_t cli_pid;              // CLIENT PID
  WINDOW * field;
  pthread_t p_pos[20];
  pthread_mutex_init(&lock, NULL);
  struct thread_data *thd_cli = NULL;
  cli_game_t game;
  cli_player_t * player_list = NULL;
  // CLIENT AUTH BOOL
  cli_log.auth = false;

  // SIGNAL TO KILL SERVER
  signal(SIGUSR1, signal_handler_cli);
  // SIGNAL TO WHAT MICHAEL???
  signal(SIGALRM, signal_handler_cli);

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
    thd_cli = malloc(sizeof(struct thread_data) * game.numPlayers*2);
    // READS FROM SRV ALL GAME PLAYER DATA
    for(int i = 0; i < game.numPlayers * 2; i++)
    {
      read(cli_fd, &player_list[i], sizeof(cli_player_t));
      thd_cli[i].p.x = player_list[i].posX;
      thd_cli[i].p.y = player_list[i].posY;

    // LIST GAME
    // CONNECT TO GAME: TEAM NUM, PLAYER NUM
    }

    fd_set conj;
    struct timeval tempo;
    int ret;

    initscr();			/* Start curses mode 		*/
    cbreak();			/* Line buffering disabled, Pass on  everty thing to me 		*/
    keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    start_y = (LINES - height) / 2;	/* Calculating for a center placement */
    start_x = (COLS - width) / 2;	/* of the window		*/

    attron(COLOR_PAIR(1));
       printw("Press q to exit");
        refresh();
    attroff(COLOR_PAIR(1));

    field = crt_win( height,  width,  start_x,  start_y);

    for (size_t i = 0; i < game.numPlayers * 2; i++) {
        thd_cli[i].field = field;
        pthread_create(&p_pos[i], NULL, thread_mgmt, &thd_cli[i]);
    }

    do {
        FD_ZERO(&conj);
        FD_SET(0, &conj);
        tempo.tv_sec = 1;
        tempo.tv_usec = 500000;
        ret = select( 1, &conj, NULL, NULL , &tempo);

        if (ret > 0){
            if (FD_ISSET(0, &conj))
                ch = getchar();
        }else{
            clean_pos(&field);
            wrefresh(field);
        }
    } while( ch != 'q');

    EXIT = 1;
    for (size_t i = 0; i < game.numPlayers*2; i++)
        pthread_join(p_pos[i], NULL);
  }

  // CLOSE FIFO CONNECTION
  close(cli_fd);
  close(srv_fd);

  // REMOVES FIFO FILE
  unlink(cli_pipe_name);

  return 0;
}

void signal_handler_cli(int sig){
  if(sig == SIGUSR1)
    cli_shutdown = true;
}

WINDOW* crt_win(int height, int width, int start_x, int start_y){
    WINDOW * game_field;

    game_field = newwin(height, width, start_y, start_x);
    box(game_field, '|' , '-');


    wrefresh(game_field);

    return game_field;
};
void move_pl(WINDOW* field, int y, int x){
    wattron(field,COLOR_PAIR(1));
    mvwprintw(field, y, x, "Y");
    wattroff(field,COLOR_PAIR(1));
};
int rand_p(int min ,int max){
    return min + rand() % (max - min + 1);
}
void* thread_mgmt( void *p){
    struct thread_data *data = (struct thread_data *) p;

    do {
        pthread_mutex_lock(&lock);
        move_pl(data->field, data->p.y, data->p.x);
        wrefresh(data->field);
        pthread_mutex_unlock(&lock);

        //SPEED
        sleep(1);
    } while(!EXIT);

    pthread_exit(0);
}

void clean_pos( WINDOW ** f){
    for (size_t y = 1; y < 20; y++) {
        for (size_t x = 1; x < 50; x++) {

        wattron(*f,COLOR_PAIR(2));
        pthread_mutex_lock(&lock);
        mvwprintw(*f, y, x, " ");
        pthread_mutex_unlock(&lock);
        wattroff(*f,COLOR_PAIR(2));

        }
    }
}
