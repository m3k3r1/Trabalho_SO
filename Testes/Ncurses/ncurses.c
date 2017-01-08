#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int EXIT = 0;
pthread_mutex_t lock;

struct pos{
    int x, y, id;
};

WINDOW* crt_win(int height, int width, int start_x, int start_y);
void move_pl(WINDOW* field, int y, int x);
int rand_p(int min ,int max);
void* thread_mgmt( void *p);

void clean_pos( WINDOW ** f){
    for (size_t y = 1; y < 20; y++) {
        for (size_t x = 1; x < 50; x++) {

        wattron(*f,COLOR_PAIR(2));
        pthread_mutex_lock(&lock);
        mvwprintw(*f, y, x, " ");
        pthread_mutex_unlock(&lock);
        wattroff(*f,COLOR_PAIR(2));

        }
    }
}

int main(int argc, char const *argv[]) {
    int ch;
    int start_y = 0, start_x = 0;
    int height = 21, width = 51;
    WINDOW * field;
    pthread_t p_pos[5];
    pthread_mutex_init(&lock, NULL);

    fd_set conj;
    struct timeval tempo;
    int ret;

    initscr();			/* Start curses mode 		*/
	  cbreak();			/* Line buffering disabled, Pass on  everty thing to me 		*/
	  keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

	   start_y = (LINES - height) / 2;	/* Calculating for a center placement */
	    start_x = (COLS - width) / 2;	/* of the window		*/

    attron(COLOR_PAIR(1));
	   printw("Press q to exit");
	    refresh();
	attroff(COLOR_PAIR(1));

    field = crt_win( height,  width,  start_x,  start_y);

    for (size_t i = 0; i < 5; i++) {
        srand(time(NULL));
        pthread_create(&p_pos[i], NULL, thread_mgmt, (void *)field);
    }

    do {
        FD_ZERO(&conj);
        FD_SET(0, &conj);
        tempo.tv_sec = 1;
        tempo.tv_usec = 500000;
        ret = select( 1, &conj, NULL, NULL , &tempo);

        if (ret > 0){
            if (FD_ISSET(0, &conj))
                ch = getchar();
        }else{
            clean_pos(&field);
            wrefresh(field);
        }
    } while( ch != 'q');

    EXIT = 1;
    for (size_t i = 0; i < 5; i++) {
        pthread_join(p_pos[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    delwin(field);
    endwin();
    return 0;
}

WINDOW* crt_win(int height, int width, int start_x, int start_y){
    WINDOW * game_field;

    game_field = newwin(height, width, start_y, start_x);
    box(game_field, '|' , '-');


    wrefresh(game_field);

    return game_field;
};
void move_pl(WINDOW* field, int y, int x){
    wattron(field,COLOR_PAIR(1));
    mvwprintw(field, y, x, "Y");
    wattroff(field,COLOR_PAIR(1));
};
int rand_p(int min ,int max){
    return min + rand() % (max - min + 1);
}
void* thread_mgmt( void *p){
    struct pos players;
    WINDOW *field = (WINDOW *) p;

    do {
        players.x = rand_p(1, 49);
        players.y = rand_p(1, 19);
        pthread_mutex_lock(&lock);
        move_pl(field, players.y, players.x);
        wrefresh(field);
        pthread_mutex_unlock(&lock);

        //SPEED
        sleep(1);
    } while(!EXIT);

    pthread_exit(0);
}
