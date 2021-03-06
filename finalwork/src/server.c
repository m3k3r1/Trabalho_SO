#include "base.h"
#include "usr_mgmt.h"

void signal_handler_srv(int sig);

bool srv_shutdown = false;

int main(int argc, char const *argv[])
{
  int srv_fd, cli_fd;                 // FIFO CONNECION
  char  cli_pipe_name[20],            // CLIENT NAME WITH PID
        cmd[20] = "",
        arg1[20] = "",
        arg2[20] = "",                // COMMAND LINE STRS
        file_name[20];                // LOGIN CREDS FILE
  bool  custom_login_file = false;    // BOOL IF SERVER IS STARTED WITH LOG FILE
  login_t cli_log;                    // USER CREDS STRUCT
  pid_t cli_pid;                      // CLIENT PID
  user_t * user_list = NULL;          // USER LIST
  game_t game;                        // GAME STRUCT


  // TMP VARS
  data_player_t * player_data = NULL, * curr_player_data = NULL;      // DATA STRUCT FOR CLIENT
  player_t * curr_player = NULL;

  //struct sigaction act;
  game.p_list = NULL;
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
        if (!strcmp(cmd, "user") &&
        strcmp(arg1, " ") && strlen(arg1) > 0 &&
        strcmp(arg2, " ") && strlen(arg2) > 0)
          sign_up(arg1, arg2,file_name, custom_login_file);
        else

        // START GAME
        if(!strcmp(cmd, "start") && strlen(arg1) > 0)
        {
          puts("game start"); // DEBUG

          // SET GAME DATA
          set_game(&game, atoi(arg1));

          // SEND CLIENT GAME STRUCT TO CLIENT
          write_game_cli(user_list, &game);

          // SET PLAYER STRUCT DATA
          curr_player = game.p_list;
          curr_player_data = malloc(sizeof(data_player_t));
          curr_player_data->player = curr_player;
          curr_player_data->user_list = user_list;
          curr_player_data->next = NULL;

          // PTR FOR PLAYER STRUCT DATA FOR THREADS
          player_data = curr_player_data;

          // CREATE LINK LIST FOR PLAYER DATA STRUCT FOR THREADS
          curr_player = curr_player->next;
          while(curr_player)
          {
            // DATA PLAYER STRUCT MALLOC
            curr_player_data->next = malloc(sizeof(data_player_t));
            curr_player_data = curr_player_data->next;

            // ASSIGN ALL PTRS NEEDED
            curr_player_data->player = curr_player;
            curr_player_data->user_list = user_list;
            curr_player_data->next = NULL;

            // GET NEXT PLAYER
            curr_player = curr_player->next;

          }

          // ASSIGN DATA PLAYER STRUCT PTR TO GAME
          game.data_player = player_data;

          // CREATE GAME THREAD
          pthread_create(&(game.tid), NULL, runGame, (void *) &game);

          // CREATE GAME PLAYER MOVEMENT THREADS
          curr_player_data = player_data;
          while(curr_player_data)
          {
            pthread_create(&(curr_player_data->player->tid),
                          NULL,
                          playerMovement,
                          (void *) curr_player_data);

            curr_player_data = curr_player_data->next;
          }

          pthread_join(game.tid, &(game.retval));

        }
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
