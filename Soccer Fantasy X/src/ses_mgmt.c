#include "ses_mgmt.h"

// STARTS RANDOM NUMBER GENERATION
void init_random_gen()
{
  srand((unsigned) time(NULL));
}

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

void move(game_control_t * head, int id)
{
  int tmpX, tmpY;
  game_control_t * curr;
  curr = head;

  // SELECT RIGHT PLAYER
  for(int i = 0; i <= id; i++)
    curr = curr->next;

  // GENERATES RANDOM POSITION
  do {
    tmpX = curr->posX;
    tmpY = curr->posY;

    tmpX = randNum(tmpX - 1, tmpX + 1);
    tmpY = randNum(tmpY - 1, tmpY + 1);
  } while(moveCheck(tmpX, tmpY, head));

  curr->posX = tmpX;
  curr->posY = tmpY;
}
