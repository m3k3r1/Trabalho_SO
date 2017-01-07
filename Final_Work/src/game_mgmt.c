#include "game_mgmt.h"

void * runGame(void * arg)
{
  int i = 0;
  game_t * game = (game_t *) arg;

  while(i < game->seconds)
  {
    sleep(1);
    i++;
  }
  // TERMINATE ALL THREADS AND USERS
  // SHOW GOALS

  pthread_exit(0);
}
