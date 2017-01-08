#include "usr_mgmt.h"


// LIST USERS LOGGED IN
int list_player(user_t * head){
    int counter = 0;
    printf("\n::Active players::\n" );

    if (head == NULL){
        printf("No active users\n" );
        return 0;
    }

    while(head){
        printf("%s\n", head->usr_name);
        counter++;
        head = head->next_usr;
    }

    return counter;
}

// ADD USER TO FILE
void sign_up(char* new_usr, char* new_pwd, char* file_name, bool custom_file){
    FILE *f_log;

    if(!custom_file){
         f_log= fopen(LOGIN_FILE, "a+");
    }else{
         f_log = fopen(file_name, "a+");
    }

    if(!f_log){
        perror("[ERROR]User database file not found");
        return;
    }

    fprintf(f_log, "%s %s\n", new_usr, new_pwd);

    printf("[NEW USER] - (%s) (%s)\n", new_usr, new_pwd );
    fclose(f_log);
}

// FREE USERS LINKED LIST
void free_mem(user_t * head) {
    user_t * tmp = NULL;

    while (head != NULL) {
        tmp = head->next_usr;
        free(head);
        head = tmp;
    }
}

// CHECKS IF USER IS ALREADY LOGGED ON
bool check_user(user_t * head ,char usr[20]){
  if (!head)
    return false;

  while(head){
    if(!strcmp(head->usr_name, usr) )
      return true;
    head = head->next_usr;
  }

  return false;
}

// SAVES USER TO LOGGED USERS LINKED LIST
bool save_user(user_t ** head, char usr[20], int pid){
  while(*head)
    head = &(*head)->next_usr;

  *head= add_user(usr, pid);

  if(* head == NULL)
    return 1;
  else
    return 0;
}

// ADD USER TO LOGGED USERS LINKED LIST
user_t * add_user(char usr[20], int pid){
    user_t * newUser;

    if (!(newUser = malloc(sizeof(user_t))))
    {
        perror("[MEMORY_ERROR]Can't alocate new node");
        return NULL;
    }

    strcpy(newUser->usr_name ,usr);
    newUser->pid = pid;
    newUser->next_usr = NULL;

    return newUser;
}

// AUTHENTICATES USER IN SERVER
bool usr_auth(char usr[20], char pass[20], char * file_name, bool custom_file){
  char tmp_usr[20];
  char tmp_pass[20];
  FILE * f_log;

  if(!custom_file){
    f_log= fopen(LOGIN_FILE, "r");
  }else{
    f_log = fopen(file_name, "r");
  }

  if(!f_log){
    perror("[ERROR]User database file not found");
    return false;
  }

  while (fscanf(f_log, "%s %s", tmp_usr, tmp_pass) == 2) {
    if (!strcmp(tmp_usr, usr) && !strcmp(tmp_pass, pass)){
      fclose(f_log);
      return true;
    }
  }

  fclose(f_log);
  return false;
}

// LOGS OFF USER FROM SERVER
void logout_user(user_t ** head, siginfo_t * cli, void * context)
{
  user_t * curr, * aux, *tmp = NULL;
  curr = aux = * head;



  if((*head)->next_usr == NULL)
  {
    free(*head);
    return;
  }

  while(curr->next_usr)
  {
    if(cli->si_pid == curr->pid)
    {
      tmp = curr->next_usr->next_usr;
      free(curr->next_usr);
      curr->next_usr = tmp;
      break;
    }

    curr = curr->next_usr;
  }

  *head = aux;
}

// SENDS SIGNAL TO CLIENT EXIT
void exit_warning(user_t * list){
  while (list != NULL) {
    kill(list->pid, 10);
    list = list->next_usr;
  }
}

// SENDS GAME_T TO ALL CLI_FIFO
void write_game_cli(user_t * list, game_t * game)
{
  player_t * curr = game->p_list;
  cli_game_t game_tmp;
  cli_player_t p_tmp;
  char cli_pipe_name[20];
  int cli_fd;

  // ASSIGN DATA TO CLIENT GAME STRUCT
  game_tmp.seconds = game->seconds;
  game_tmp.numPlayers = game->numPlayers;
  for(int i = 0; i < 2; i++)
    game_tmp.res[i] = game->res[i];

  // SEND CLIENT PLAYER STRUCT
  while(list)
  {
    sprintf(cli_pipe_name, CLI_FIFO, list->pid);
    cli_fd = open(cli_pipe_name, O_WRONLY|O_CREAT, 0600);

    // SEND CLIENT GAME STRUCT
    write(cli_fd, &game_tmp, sizeof(game_tmp));

    // ASSIGN POSITION
    while(curr)
    {
      p_tmp.role = curr->role;
      p_tmp.posX = curr->posX;
      p_tmp.posY = curr->posY;
      p_tmp.id = curr->id;

      write(cli_fd, &p_tmp, sizeof(p_tmp));
      curr = curr->next;
    }

    // NEXT USER/CLIENT
    list = list->next_usr;
  }
}
// SENDS SHIT TO USR
void write_to_clients(user_t * list, bool gamestart)
{
  char shit_name[20];
  int cli_fd;

  while(list)
  {
    sprintf(shit_name, "client_%d_FIFO", list->pid);
    cli_fd = open(shit_name, O_WRONLY|O_CREAT, 0600);
    write(cli_fd, &gamestart, sizeof(gamestart));
    close(cli_fd);
    list = list->next_usr;
  }
}


// CLIENT FUNCTIONS
// READS CLIENT CREDS
void set_cred(char usr[20], char pass[20]){
  printf ("[USERNAME] : ");
  scanf("%s", usr);
  printf ("[PASSWORD] : ");
  scanf("%s", pass);
}
