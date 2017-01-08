#include "base.h"

void cmd_control(char *cmd, char* arg1, char* arg2){
    int  flag = 1, counter = 0;
    char tmp_cmd[20];

    if(strlen(arg1) !=0 || strlen(arg2) !=0){
        memset(&(*arg1), 0, strlen(arg1));
        memset(&(*arg2), 0, strlen(arg2));
    }

    for (size_t i = 0; cmd[i] != '\0'; i++) {
        if(cmd[i] == ' '){
            counter = 0;
            flag++;
            continue;
        }

        if(flag == 1){
            tmp_cmd[i] = cmd[i];
            tmp_cmd[i + 1] = '\0';
        }

        if(flag == 2){
            arg1[counter] = cmd[i];
            arg1[counter + 1] = '\0';
            counter++;
        }

        if(flag == 3){
            arg2[counter] = cmd[i];
            arg2[counter + 1] = '\0';
            counter++;
        }
    }
    strcpy(cmd, tmp_cmd);
}
