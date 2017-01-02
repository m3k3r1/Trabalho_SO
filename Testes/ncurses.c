#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
    char y, x;
} pos_t;

typedef struct{
    char fim, humano, num;
    int tempo;
} player_t;

int max_y, max_x;
pos_t ele[4]  = { {3, 10}, {15,15}, {12,40}, {19,45}};
pthread_mutex_t key;


void* move_jogador( void *dados){
    player_t *j;
    int ch;
    pos_t d;

    j = (player_t*) dados;

    do {
        if (!j->humano) {
            ch = rand() % 4;
            d.y = 0;
            d.x = 0;

            switch (ch) {
                case 1:
                    d.y--;
                    break;
                case 2:
                    d.x++;
                    break;
                case 3:
                    d.x--;
                    break;
                case 4:
                    d.x++;
                    break;
            }

            if (j-> num >= 0 && j->num < 4) {
                pthread_mutex_lock(&key);
                mvaddch(ele[j->num].y, ele[j->num].x , ' ');
                ele[j->num].y += d.y;
                ele[j->num].x += d.x;
                mvaddch(ele[j->num].y, ele[j->num].x, '0'+ j->num);
                refresh();
                }
                pthread_mutex_unlock(&key);
        }
        usleep( j->tempo);
    } while( !j->fim);

    pthread_exit(0);
}


int main(int argc, char const *argv[]) {
    int ch,num;
    pos_t d;
    player_t j[4] = { {0,0,0,100000}, {0,0,1,300000}, {0,0,2,500000}, {0,0,3,700000} };
    pthread_t tarefa[4];

    initscr();
    noecho();
    cbreak();
    keypad (stdscr, TRUE);
    curs_set(0);
    getmaxyx(stdscr, max_y, max_x);
    pthread_mutex_init(&key, NULL);

    clear();
    for (size_t i = 0; i < 4; i++)
        mvaddch(ele[i].y, ele[i].x, '0'+i);
    refresh();

    for (size_t i = 0; i < 4; i++)
        pthread_create( &tarefa[i], NULL, &move_jogador, (void*) &j[i]);

    while ( (ch = getch())  != 'q'  ) {
        d.y = 0;
        d.x = 0;

        switch (ch) {
            case '0' :
                num = 0;
                j[0].humano = 1;
                break;
            case '1'  :
                num = 1;
                j[1].humano = 1;
                break;
            case '2'  :
                num = 2;
                j[2].humano = 1;
                break;
            case  '3' :
                num = 3;
                j[3].humano = 1;
                break;
            case 'd' :
                num = -1;
                for (size_t i = 0; i < 4; i++) {
                    j[i].humano = 0;
                }
            case KEY_UP:
                d.y--;
                break;
            case KEY_DOWN:
                d.y++;
                break;
            case KEY_RIGHT:
                d.x++;
                break;
            case KEY_LEFT:
                d.x--;
                break;
        }

        if (num >= 0 && num < 4) {
            pthread_mutex_lock(&key);

            //para o trabalho verificar se o jogadores se sobrepoem ou se sai de campo
            mvaddch(ele[num].y, ele[num].x , ' ');
            ele[num].y += d.y;
            ele[num].x += d.x;
        //}
        //clear();
        //for (size_t i = 0; i < 4; i++)
            mvaddch(ele[num].y, ele[num].x, '0'+ num);
        refresh();
        pthread_mutex_unlock(&key);
        }
    }

    for (size_t i = 0; i < 4; i++) {
        j[i]. fim = 1;
        pthread_join(tarefa[i], NULL);
    }
    pthread_mutex_destroy(&key);
    endwin();
    return 0;
}
