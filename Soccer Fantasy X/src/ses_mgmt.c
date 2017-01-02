#include "ses_mgmt.h"

void set_game(game_stat_t** head, int t){
    while ( *head )
        head = &(*head)->next_game;
    *head= new_game(t);
}

<<<<<<< HEAD
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
    if(x == curr->posX && y == curr->posY)
      return 1;
    curr = curr->next;
  }

  return 0;
}

// GENERATES NEW POS AND CHECKS IT
void * PlayerMovement(void * arg)
{
  game_control_t * player = (game_control_t *) arg;
  while(1)
    move(player);

  return NULL;
}

// MOVES AND CHECKS POSITION
void move(game_control_t * player)
{
  int tmpX, tmpY;

  // GENERATES RANDOM POSITION
  do {
    tmpX = player->posX;
    tmpY = player->posY;

    tmpX = randNum(tmpX - 1, tmpX + 1);
    tmpY = randNum(tmpY - 1, tmpY + 1);
  } while(moveCheck(tmpX, tmpY, player->head));

  player->posX = tmpX;
  player->posY = tmpY;
}

void startGame(game_stat_t * game, game_control_t ** head, int seconds)
{
  int numDef, numOff;
  char * ndef = NULL, * noff = NULL;
  game_control_t * curr;

  puts("set defenders");
  // GET NUMBER OF DEFENDERS
  ndef = getenv("NDEFESAS");
  if(ndef != NULL)
    numDef = atoi(ndef);
  else
    numDef = 2;

  puts("set attackers");
  // GET NUMBER OF ATTACKERS
  noff = getenv("NAVANCADOS");
  if(noff != NULL)
    numOff = atoi(noff);
  else
    numOff = 2;

  puts("set number of players");
  // ASSIGN NUMBER OF PLAYERS
  game->numPlayers = 1 + numDef + numOff;

  puts("set run game var");
  // ASSIGN RUN GAME VAR
  game->run = 1;

  puts("set goals to 0");
  // ASSIGN 0 TO GOALS
  game->game_result[0] = 0;
  game->game_result[1] = 0;

  puts("outside for to player malloc");
  for(int i = 0, j = 0; i < (game->numPlayers) * 2; i++)
  {
    // PLAYER MALLOC
    if(!i)
    {
      * head = malloc(sizeof(game_control_t));
      curr = * head;
      curr->next = NULL;
      curr->head = * head;
      puts("head alloc player");
    }
    else
    {
      curr->next = malloc(sizeof(game_control_t));
      curr = curr->next;
      curr->next = NULL;
      puts("new alloc player");
    }

    puts("pos assignment");
    // ASSIGN POSITION TODO
    curr->posX = 2;
    curr->posY = 2;

    // ASSIGN ROLE
    // GOALKEEPER
    if(!j)
      curr->role = 0;

    // DEFENSE
    if(j > 0)
      curr->role = 1;

    // OFFENSIVE
    if(j > numDef)
      curr->role = 2;

    // RESET COUNTER OR ADD
    if(j == game->numPlayers - 1)
      j = 0;
    else
      j++;

    puts("creating thread");
    pthread_create(&(curr->tid), NULL, PlayerMovement, curr);
    //pthread_create(&(curr->tid), NULL, show, &numDef);
    puts("thread created");
  }

  // THREADS FOR PLAYERS??? TODO
=======
game_stat_t* new_game(int t){
    game_stat_t* tmp;

    if (!(tmp = malloc(sizeof(game_stat_t)))) {
        perror("[MEMORY_ERROR]Can't alocate new node");
        return NULL;
    }

    tmp->game_result[0] = 0;
    tmp->game_result[1] = 0;
    tmp->dur = t;
    tmp->next_game = NULL;

    return tmp;
>>>>>>> master
}
