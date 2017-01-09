#include "base.h"

#define WIDTH 51
#define HEIGHT 21

// USER STRUCT
typedef struct user_t user_t;
struct user_t {
  char usr_name[20];
  pid_t pid;
  user_t * next_usr;
};

// GAME PLAYER STRUCT
typedef struct player_t player_t;
struct player_t
{
  int role, id;             // PLAYER ROLE AND ID
  int posX, posY;           // PLAYER POS
  int run;                  // GAME SECONDS
  pthread_t tid;            // PLAYER MOVEMENT THREAD
  void * retval;
  player_t * next, * head;  // NEXT PLAYER PTR
};

// GAME STRUCT
typedef struct
{
  int seconds;        // GAME SECOND;
  int res[2];         // GAME RESULT
  int numPlayers;     // GAME PLAYER NUM
  player_t * p_list;  // GAME PLAYER LIST
  pthread_t tid;      // TID FOR GAME FUNCTION
  void * retval;
} game_t;

// POSITION DATA STRUCT FOR CLIENT
typedef struct
{
  player_t * player;
  user_t * user_list;
} data_cli_t;

// CLIENT'S GAME STRUCT
typedef struct
{
  int seconds;
  int numPlayers;
  int res[2];
} cli_game_t;

// CLIENTS'S PLAYER STRUCT
typedef struct
{
  int posX, posY, role, id;
} cli_player_t;

//SETUP GAME FUNCTIONS
void set_game(game_t * game, int sec); // SETUP REQUIRED DATA FOR GAME

// HANDLING GAME FUNCTIONS
void * playerMovement(void * arg);
void * runGame(void * arg); // THREAD FUNCTION FOR GAME CONTROL
