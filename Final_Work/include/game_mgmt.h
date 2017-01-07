#include "base.h"

#define WIDTH 51
#define HEIGHT 21

// GAME PLAYER STRUCT
typedef struct player_t player_t;
struct player_t
{
  int role, id;             // PLAYER ROLE AND ID
  int posX, posY;           // PLAYER POS
  int run;              // GAME SECONDS
  pthread_t tid;            // PLAYER MOVEMENT THREAD
  player_t * next, * head;  // NEXT PLAYER PTR
};

// GAME STRUCT
typedef struct
{
  int seconds;        // GAME SECONDS
  int res[2];         // GAME RESULT
  int numPlayers;     // GAME PLAYER NUM
  player_t * p_list;  // GAME PLAYER LIST
  pthread_t tid;      // TID FOR GAME FUNCTION
} game_t;

// TODO PLAYER STRUCT NEEDED FOR CLIENT?! SO CLIENT DOESN'T HAVE ACCESS TO ALL INFO


//SETUP GAME FUNCTIONS
//void getNumPlayers(int * numDef, int * numOff); // GET GAME PLAYER NUM
//void initPlayers(game_t * game, int numDef, int numOff); // INIT PLAYER DATA
void set_game(game_t * game, int sec); // SETUP REQUIRED DATA FOR GAME


// HANDLING GAME FUNCTIONS
void * playerMovement(void * arg);
void * runGame(void * arg); // THREAD FUNCTION FOR GAME CONTROL
