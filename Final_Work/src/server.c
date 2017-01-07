#include "base.h"
#include "usr_mgmt.h"
bool srv_shutdown = false;

int main(int argc, char const *argv[])
{
  int srv_fd, cli_fd;                 // FIFO CONNECION
  char  cli_pipe_name[20],            // CLIENT NAME WITH PID
        cmd[20], arg1[20], arg2[20],  // COMMAND LINE STRS
        file_name[20];                // LOGIN CREDS FILE
  bool  custom_login_file = false;    // BOOL IF SERVER IS STARTED WITH LOG FILE
  login_t cli_log;                    // USER CREDS STRUCT
  pid_t cli_pid;                      // CLIENT PID
  user_t * user_list = NULL;          // USER LIST
  game_t game;

  //struct sigaction act;

  // SELECT FOR KEYBOAD
  fd_set conj;
  struct timeval tempo;
  int ret;

  // SIGNAL FOR SERVER SHUTDOWN
  signal(SIGUSR1, signal_handler_srv);

/*
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = logout_user;
  sigaction(SIGUSR2, &act, NULL);
*/

  // CHECK ARGS FOR LOGIN FILENAME
  if (argc > 1)
  {
      custom_login_file = true;
      strcpy(file_name,  argv[1]);
  }

  // CHECK IF SERVER FIFO ALREADY EXISTS
  if( !access(SRV_FIFO, F_OK) )
  {
      perror("[ERROR]Server already running\n");
      exit(1);
  }

  // MAKE SERVER FIFO
  mkfifo(SRV_FIFO , 0600);
  srv_fd = open(SRV_FIFO, O_RDWR);

  do {
    // INITIALIZE SELECT
    FD_ZERO(&conj);
    FD_SET(0, &conj);
    FD_SET(srv_fd, &conj);

    tempo.tv_sec = 1;
    tempo.tv_usec = 500000;

    ret = select(srv_fd + 1, &conj, NULL, NULL, &tempo);

    // CHECK IF THERE'S DATA
    if(ret > 0)
    {
      if(FD_ISSET(0, &conj)) // IN KEYBOARD
      {
        // GET SERVER CMD
        fgets(cmd, sizeof(cmd), stdin);
        strtok(cmd, "\n");
        // SPLIT CMD ARGS
        cmd_control(cmd, arg1, arg2);

        // SHOWS LOGGED IN USERS
        if(!strcmp(cmd, "users"))
          list_player(user_list);

        // SIGN UP USER
        if (!strcmp(cmd, "user"))
          sign_up(arg1, arg2,file_name, custom_login_file);

        if(!strcmp(cmd, "start") && strlen(arg1) > 0)
        {
          // RUN GAME FUNCTION
          puts("game start");
          game.seconds = atoi(arg1);
          write_game_cli(user_list, &game);
      }

      if (FD_ISSET(srv_fd, &conj)) // IN THE FIFO
      {
        read(srv_fd, &cli_pid, sizeof(cli_pid));  // READ CLIENT PID
        read(srv_fd, &cli_log, sizeof(cli_log));  // READ USER
        // BRIEF ADMIN OF USER LOGIN DEBUG
        printf("%s-%s", cli_log.usr, cli_log.pass);
        // CAT STR CLI FIFO AND PID
        sprintf(cli_pipe_name , CLI_FIFO, cli_pid);
        // OPEN CLI FIFO WRITE PERM
        cli_fd = open(cli_pipe_name, O_WRONLY|O_CREAT, 0600);

        // CHECKS USER CREDS
        if((cli_log.auth = usr_auth(cli_log.usr, cli_log.pass, file_name, custom_login_file)) == true)
        {
          // CHECKS IF USER ALREADY LOGGED ON
          if(check_user(user_list ,cli_log.usr)) // USER IS ALREADY LOGGED
          {
            fprintf(stderr, "Player %s already logged in server\n" , cli_log.usr);
            cli_log.auth = false;
            write(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
            // TODO CLIENT SHOULD ASK AGAIN FOR CREDENTIALS NOT EXIT
          }
          else // USER CREDS ARE VALID
          {
            printf("%s\n", cli_log.auth ? "[LOGIN SUCCESFULL]" : "[LOGIN FAILED]");
            // SAVE USER TO USER LINKED LIST
            if(save_user(&user_list ,cli_log.usr, cli_pid))
              cli_log.auth = false;
            else
              cli_log.auth = true;

            write(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
          }
        }
        else // USER CREDS ARE NOT VALID
        {
          write(cli_fd, &cli_log.auth, sizeof(cli_log.auth));
          printf("%s\n", cli_log.auth ? "[LOGIN SUCCESFULL]" : "[LOGIN FAILED]");
          // TODO CLIENT SHOULD ASK AGAIN FOR CREDENTIALS NOT EXIT
        }
      }
    }
  //} while(strcmp(cmd, "shutdown") && !srv_shutdown);
  } while(strcmp(cmd, "shutdown"));

  // SEND 0 BYTES TO CLIENT SO HE CAN GET THE FUCK OUT OF THE READ
  if(!strcmp(cmd, "shutdown"))
    write_game_cli(user_list, NULL);


  // WARN USERS SERVER GOING TO SHUTDOWN
  exit_warning(user_list);
  // FREE USER LINKED LIST
  free_mem(user_list);

  // CLOSE
  close(cli_fd);
  close(srv_fd);

  // REMOVES SERVER FIFO
  unlink(SRV_FIFO);

  exit(0);
}

void signal_handler_srv(int sig){
  if (sig == SIGUSR1) {
    printf("[SIGUSR1 RECEIVED] - Shuting down\n");
    srv_shutdown = true;
  }
}
