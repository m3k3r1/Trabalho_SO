#include "usr_mgmt.h"

void set_crd(char usr[20], char pss[20]){
    printf ("[USERNAME] : ");
    scanf("%s", usr);
    printf ("[PASSWORD] : ");
    scanf("%s", pss);
};

bool usr_auth(char usr[20], char pss[20]){
    FILE *f_log = fopen(LOGS_FILE, "r");
    char tmp_usr[20];
    char tmp_pss[20];

    if(!f_log){
        perror("[ERROR]User database file not found");
        return false;
    }

    while (fscanf(f_log, "%s %s", tmp_usr, tmp_pss) == 2) {
        if (!strcmp(tmp_usr, usr) && !strcmp(tmp_pss, pss)){
            fclose(f_log);
            return true;
        }
    }
    fclose(f_log);
    return false;
};

void save_player(player_t** head, char usr[20], int pid){
    if (head) {
        if (!(*head = malloc(sizeof(player_t)))) {
            perror("[MEMORY_ERROR]Can't alocate new node");
            return;
        }
    }else{
        while (*head)
            head = &(*head)->nxt_player;
        *head = add_player(usr, pid);
    }
}

player_t* add_player( char usr[20], int pid){
    player_t *tmp;

    if (!(tmp = malloc(sizeof(player_t)))) {
        perror("[MEMORY_ERROR]Can't alocate new node");
        return NULL;
    }

    strcpy(tmp->usr_name ,usr);
    tmp->pid = pid;
    tmp->nxt_player = NULL;

    return tmp;
}
