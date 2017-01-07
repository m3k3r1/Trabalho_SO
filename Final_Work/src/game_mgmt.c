#include "game_mgmt.h"

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

// THREAD FUNCTION FOR GAME CONTROL
void * runGame(void * arg)
{
  int i = 0;
  game_t * game = (game_t *) arg;

  // GAME RUNS WHILE COUNTER HASN'T REACH GAME SECONDS
  while(i < game->seconds)
  {
    sleep(1);
    i++;
  }

  // TERMINATE ALL THREADS AND USERS
  // SHOW GOALS

  pthread_exit(0);
}
