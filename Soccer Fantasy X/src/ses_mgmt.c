#include "ses_mgmt.h"

// GENERATES RANDOM NUM BETWEEN MIN AND MAX, INCLUSIVE
int randNum(int min, int max)
{
  return min + rand() % (max - min + 1);
}

// CHECK POS AVAILABILITY
int moveCheck(int x, int y, game_control_t * head)
{
  game_control_t * curr = head;

  while(curr != NULL)
  {
    if(x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
      if(x == curr->posX && y == curr->posY)
        return 1;
      else
        return 0;
    else
      return 0;
    curr = curr->next;
  }

  return 0;
}

// GENERATES NEW POS AND CHECKS IT
void * PlayerMovement(void * arg)
{
  game_control_t * player = (game_control_t *) arg;
  // TODO MIGHT NEED ANOTHER CONTROL VAR INSIDE WHILE
  while(1)
  {
    movePlayer(player);
    switch(player->role)
    {
      case 0:
        sleep(2);
        break;

      case 1:
        sleep(2);
        break;

      case 2:
        sleep(2);
    }
  }

  return NULL;
}

// MOVES AND CHECKS POSITION
void movePlayer(game_control_t * player)
{
  int tmpX, tmpY;

  // GENERATES RANDOM POSITION
  do {
    do {
      tmpX = player->posX;
      tmpX = randNum(tmpX - 1, tmpX + 1);
    } while(tmpX < 0 || tmpX > WIDTH);

    do {
      tmpY = player->posY;
      tmpY = randNum(tmpY - 1, tmpY + 1);
    } while(tmpY < 0 || tmpY > HEIGHT);
  } while(moveCheck(tmpX, tmpY, player->head));

  player->posX = tmpX;
  player->posY = tmpY;
}

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

void initPlayers(game_stat_t * game, game_control_t ** head,
                 int numDef, int numOff)
{
  game_control_t * curr = NULL;
  // int height = HEIGHT;

  for(int i = 0, j = 0, l = 1; i < game->numPlayers * 2; i++)
  {
    if(!i)
    {
      * head = malloc(sizeof(game_control_t));
      curr = * head;
      curr->next = NULL;
      curr->head = * head;
    }
    else
    {
      curr->next = malloc(sizeof(game_control_t));
      curr->next->next = NULL;
      curr = curr->next;
    }

    // TODO MIGHT NOT NEED
    curr->id = i;

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
        // curr->posY = (l++ / (numDef + 1)) * height;
        curr->posY = 9 + l++;
      }
      else
      {
        curr->posX = 40;
        //curr->posY = (l++ / (numDef + 1)) * height;
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
        //curr->posY = (l++ / (numOff + 1)) * height;
        curr->posY = 9 + l++;
      }
      else
      {
        curr->posX = 30;
        //curr->posY = (l++ / (numOff + 1)) * height;
        curr->posY =  9 + l++;
      }
    }
    // RESET COUNTER OR ADD
    if(j == game->numPlayers - 1)
      j = 0;
    else
      j++;

    // PRINT TEST
    //printf("player %d x: %d y: %d role: %d\n",
    //curr->id, curr->posX, curr->posY, curr->role);
    // PLAYER MOVEMENT THREAD
    pthread_create(&(curr->tid), NULL, PlayerMovement, curr);

  }
}

void set_game(game_stat_t * game, game_control_t ** head, int sec)
{
  int numDef = 0,numOff = 0;
  game->seconds = sec;

  //  INITIALIZE NUMBER OF PLAYERS
  getNumPlayers(&numDef, &numOff);
  game->numPlayers = 1 + numDef + numOff;

  // INITIALIZE GOALS VAR
  game->game_result[0] = 0;
  game->game_result[1] = 0;

  //Initialize the players postions !!!TODO MICHAEL! DO SOMETHING PLS
  initPlayers(game, head, numDef, numOff);

}

int send_to_cli(game_control_t * head, int cli_fd, int numPlayers){
  int i = 0;
  game_control_t * curr = head;
  client_data_t tmp[numPlayers];

  while (i != numPlayers) {

    tmp[i].role = curr->role;
    tmp[i].posX = curr->posX;
    tmp[i].posY = curr->posY;
    i++;
    curr = curr->next;
  }
  sleep(1);
  if(write(cli_fd, &tmp, sizeof(tmp)))
    return 1;
  else
    return 0;
};
