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
      game->p_list = malloc(sizeof(player_t));
      curr = game->p_list;
      curr->head = game->p_list;
      curr->next = NULL;
    }
    else
    {
      curr->next = malloc(sizeof(player_t));
      curr->next->next = NULL;
      curr = curr->next;
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

    // GAME PLAYER CONTROL VAR FOR THREAD FUNC
    curr->run = 1;

    // GAME PLAYER MOVEMENT THREAD
    pthread_create(&(curr->tid), NULL, playerMovement, curr);
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
}


// PLAYER MOVEMENT THREAD FUNCTION
void * playerMovement(void * arg)
{
  player_t * player = (player_t *) arg;

  while(player->run)
  {
    // NEW POSITION
    movePlayer(player);
    switch(player->role)
    {
      case 0:
        sleep(1);
        break;

      case 1:
        sleep(2);
        break;

      case 2:
        sleep(1);
        break;
    }
  }

  pthread_exit(0);
}

void showMap(player_t * p_list)
{
  player_t * curr = p_list;
  system("clear");
  for(int i = 0, l = 0; i < WIDTH; i++)
  {
    for(int j = 0; j < HEIGHT; j++)
    {
      while(curr != NULL)
      {
        if(curr->posX == i && curr->posY == j)
        {
          printf("%c", curr->role);
          l = 1;
        }

        curr = curr->next;
      }
      if(l)
        l = 0;
      else
        putchar(' ');
    }
    putchar('\n');
  }
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
    //showMap(game->p_list);
    i++;
  }

  // TERMINATE ALL THREADS AND USERS
  while(curr != NULL)
  {
    curr->run = 0;
    curr = curr->next;
  }

  // FREE PLAYER LIST
  curr = game->p_list;
  while(curr != NULL)
  {
    tmp = curr->next;
    free(curr);
    curr = tmp;
  }

  // SHOW GOALS
  for(int j = 0; j < 2; j++)
    printf("Team %d: goals %d\n", j + 1, game->res[j]);

  pthread_exit(0);
}
