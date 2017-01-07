#include "base.h"

#define WIDTH 51
#define HEIGHT 21

// GAME PLAYER STRUCT
typedef struct player_t player_t;
struct player_t
{
  int role, id;             // PLAYER ROLE AND ID
  int posX, posY;           // PLAYER POS
  pthread_t tid;            // PLAYER MOVEMENT THREAD
  player_t * next, * head;  // NEXT PLAYER PTR
};

// GAME STRUCT
typedef struct
{
  int seconds;      // GAME SECONDS
  int res[2];       // GAME RESULT
  player_t * list;  // GAME PLAYER LIST
  pthread_t tid;    // TID FOR GAME FUNCTION
} game_t;

// TODO PLAYER STRUCT NEEDED FOR CLIENT?! SO CLIENT DOESN'T HAVE ACCESS TO ALL INFO

// THREAD FUNCTION FOR GAME CONTROL
void * runGame(void * arg);
