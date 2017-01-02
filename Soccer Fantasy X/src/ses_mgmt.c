#include "ses_mgmt.h"

void set_game(game_stat_t** head, int t){
    while ( *head )
        head = &(*head)->next_game;
    *head= new_game(t);
}

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
}
