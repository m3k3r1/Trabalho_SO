#include "base.h"

typedef struct
{
  int seconds;
  pthread_t tid;
} game_t;

void * runGame(void * arg);
