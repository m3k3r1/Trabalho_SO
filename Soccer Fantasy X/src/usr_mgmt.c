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
