#include "base.h"
#include "usr_mgmt.h"

bool cli_shutdown = false;

int main(int argc, char const *argv[]) {
  int srv_fd, cli_fd;         // FIFO CONNECTION
  int tryHard = 0;            // TRY HARD VAR FOR CLIENT LOCKOUT
  int n_bytes;
  char cli_pipe_name[20];     // CLIENT NAME WITH PID
  login_t cli_log;            // USER CREDENTIALS
  pid_t cli_pid;              // CLIENT PID
  //pthread_t curses; TODO
  //WIN win; TODO
  game_t game;
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

    if((!cli_log.auth))
    {
      fprintf(stderr, "Username and/or password not valid\n");
      tryHard++;
    }

    if(tryHard > 3)
      break;
  }

  while (cli_log.auth && !cli_shutdown) {
    do {
      n_bytes = read(cli_fd, &game, sizeof(game));
    } while(n_bytes == 0 && !cli_shutdown);

      puts("ITS ON");
    // LIST GAME
    // CONNECT TO GAME: TEAM NUM, PLAYER NUM
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
