#include "usr_mgmt.h"

void set_crd(char usr[20], char pss[20]){
    printf ("[USERNAME] : ");
    scanf("%s", usr);
    printf ("[PASSWORD] : ");
    scanf("%s", pss);
};
bool usr_auth(char usr[20], char pss[20], char * file_name, bool custom_file){
    char tmp_usr[20];
    char tmp_pss[20];
    FILE *f_log;

    if(!custom_file){
         f_log= fopen(LOGS_FILE, "r");
    }else{
         f_log = fopen(file_name, "r");
    }

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
        while ( *head )
            head = &(*head)->nxt_player;
        *head= add_player(usr, pid);
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
};
bool chk_player(player_t *head ,char usr[20]){
    if (!head)
        return false;
    while(head){
        if( !strcmp(head->usr_name, usr) )
            return true;
        head = head->nxt_player;
    }
    return false;
};
int list_player(player_t *head ){
    int counter = 0;
    printf("\n::Active players::\n" );

    if (head == NULL) {
        printf("No active users\n" );
        return 0;
    }

    while(head){
        printf("%s\n", head->usr_name);
        counter++;
        head = head->nxt_player;
    }
    return counter;
};
void sign_in(char* new_usr, char* new_pwd, char* file_name, bool custom_file){
    FILE *f_log;

    if(!custom_file){
         f_log= fopen(LOGS_FILE, "a+");
    }else{
         f_log = fopen(file_name, "a+");
    }

    if(!f_log){
        perror("[ERROR]User database file not found");
        return;
    }

    fprintf(f_log, "%s %s\n", new_usr, new_pwd);

    printf("[NEW USER] - (%s) (%s)\n", new_usr, new_pwd );
    fclose(f_log);
};
void free_mem(player_t* head) {
    player_t* tmp = NULL;

    while (head != NULL) {
        tmp = head->nxt_player;
        free(head);
        head = tmp;
    }
}
