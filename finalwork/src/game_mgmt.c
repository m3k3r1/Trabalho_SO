#include "game_mgmt.h"


//  SETUP GAME FUNCTIONS
// GET GAME PLAYERS NUM ENV VAR. IF NOT, USE STANDART
void getNumPlayers(int * numDef, int * numOff)
{
  char * ndef = NULL, * noff = NULL;

  // DEF ENV VAR
  ndef = getenv("NDEFESAS");
  if(ndef != NULL)
    * numDef = atoi(ndef);
  else
    * numDef = 2;

  // OFF ENV VAR
  noff = getenv("NAVANCADOS");
  if(noff != NULL)
    * numOff = atoi(noff);
  else
    * numOff = 2;
}

// INITIALIZE GAME PLAYERS DATA TODO FREE GAME PLAYERS DATA FUNC
void initPlayers(game_t * game, int numDef, int numOff)
{
  player_t * curr = NULL;

  // SETUP DATA
  for(int i = 0, j = 0, l = 1; i < game->numPlayers * 2; i++)
  {
    // CREATE MALLOC FOR PLAYER DATA
    if(!i)
    {
      curr = malloc(sizeof(player_t));
      curr->head = curr;
      curr->next = NULL;
      game->p_list = curr;
    }
    else
    {
      curr->next = malloc(sizeof(player_t));
      curr = curr->next;
      curr->next = NULL;
      curr->head = game->p_list;
    }

    //  FILL PLAYER DATA
    // ROLE ASSIGNMENT
    // GOALKEEPER
    if(!j)
    {
      curr->role = 0;
      if(i < game->numPlayers)
      {
        curr->posX = 1;
        curr->posY = 10;
      }
      else
      {
        curr->posX = 50;
        curr->posY = 10;
      }
    }

    // DEFENSE
    if(j > 0 && j <= numDef)
    {
      curr->role = 1;
      if(i < game->numPlayers)
      {
        curr->posX = 10;
        curr->posY = 9 + l++;
      }
      else
      {
        curr->posX = 40;
        curr->posY = 9 + l++;
      }
    }

    // OFFENSIVE
    if(j > numDef)
    {
      curr->role = 2;
      if(i < game->numPlayers)
      {
        curr->posX = 20;
        curr->posY = 9 + l++;
      }
      else
      {
        curr->posX = 30;
        curr->posY = 9 + l++;
      }
    }

    // RESET ROLE COUNTER OR INCREMENT IT
    if(j == game->numPlayers - 1)
      j = 0;
    else
      j++;

    // SET PLAYER ID
    curr->id = i;

    // GAME PLAYER CONTROL VAR FOR THREAD FUNC
    curr->run = 1;
  }
}

// SETUP REQUIRED DATA FOR GAME
void set_game(game_t * game, int sec)
{
  int numDef = 0, numOff = 0;

  // INITIALIZE NUM PLAYER
  getNumPlayers(&numDef, &numOff);
  game->numPlayers = 1 + numDef + numOff;

  // INITIALIZE SEC
  game->seconds = sec;
  // INITIALIZE GOALS
  for(int i = 0; i < 2; i++)
    game->res[i] = 0;

  // INITIALIZE GAME PLAYER POSITIONS
  initPlayers(game, numDef, numOff);
}

//  HANDLING GAME FUNCTIONS
// INITIALIZE RAND NUM SEQUENCE, OTHERWISE THE SEQUENCE IS ALWAYS THE SAME
void init_random_gen()
{
  srand((unsigned) time(NULL));
}

// GENERATES RANDOM NUM BETWEEN MIN AND MAX, INCLUDING
int randNum(int min, int max)
{
  return min + rand() % (max - min + 1);
}

// CHECK POS AVAILABILITY
int moveCheck(int x, int y, player_t * head)
{
  player_t * curr = head;

  while(curr != NULL)
  {
    if(x == curr->posX && y == curr->posY)
        return 1;

    curr = curr->next;
  }

  return 0;
}

// MOVES PLAYER
void movePlayer(player_t * player)
{
  int tmpX, tmpY;

  // GENERATES RANDOM POSITION
  do {
    do {
      tmpX = player->posX;
      tmpX = randNum(tmpX - 1, tmpX + 1);
    } while(tmpX < 0 || tmpX >= WIDTH);

    do {
      tmpY = player->posY;
      tmpY = randNum(tmpY - 1, tmpY + 1);
    } while(tmpY < 0 || tmpY >= HEIGHT);
  } while(moveCheck(tmpX, tmpY, player->head));

  player->posX = tmpX;
  player->posY = tmpY;
}

// PLAYER MOVEMENT THREAD FUNCTION
void * playerMovement(void * arg)
{
  // PTRS
  player_t * player = (player_t *) arg;
  //player_t * head = (player_t *) arg->player_list
  //user_t * curr_user = (user_t *) arg->user_list;

  while(player->run)
  {
    // NEW POSITION
    movePlayer(player);
      printf("ID: %d, X: %d, Y: %d\n", player->id, player->posX, player->posY);
    switch(player->role)
    {
      case 0:
        //sleep(0.3);
        sleep(1);
        break;

      case 1:
        //sleep(0.4);
        sleep(1);
        break;

      case 2:
        //sleep(0.3);
        sleep(1);
        break;
    }

    // SEND TO CLIENT

  }

  pthread_exit(0);
}


// THREAD FUNCTION FOR GAME CONTROL
void * runGame(void * arg)
{
  int i = 0;
  game_t * game = (game_t *) arg;
  player_t * curr = game->p_list, * tmp = NULL;

  // GAME RUNS WHILE COUNTER HASN'T REACH GAME SECONDS
  while(i < game->seconds)
  {
    sleep(1);
    puts("game is running");
    i++;
  }

  // TERMINATE ALL THREADS AND USERS
  while(curr != NULL)
  {
    curr->run = 0;
    pthread_join(curr->tid, &curr->retval);
    curr = curr->next;
  }

  // FREE PLAYER STRUCT
  curr = game->p_list;
  while(curr)
  {
    tmp = curr->next;
    free(curr);
    curr = tmp;
  }
  game->p_list = NULL;

  // SHOW GOALS
  for(int j = 0; j < 2; j++)
    printf("Team %d: goals %d\n", j + 1, game->res[j]);

  pthread_exit(0);
}
