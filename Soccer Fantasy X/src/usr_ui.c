#include "usr_ui.h"

void init_win_params(WIN *p_win){
	p_win->height = 21;
	p_win->width = 55;
	p_win->starty = (LINES - p_win->height)/2;
	p_win->startx = (COLS - p_win->width)/2;

	//BORDERS
	p_win->border.left_s = '|';
	p_win->border.right_s = '|';
	p_win->border.top_s = '-';
	p_win->border.bottom_s = '-';
	//CORNERS
	p_win->border.top_l = '+';
	p_win->border.top_r = '+';
	p_win->border.bottom_l = '+';
	p_win->border.bottom_r = '+';
}
void create_box(WIN *p_win){
	int x, y, w, h, m_x, m_y;
	chtype middle_line = '|';

	x = p_win->startx;
	m_x = p_win->startx + (p_win->width / 2);
	y = p_win->starty;
	m_y = p_win->starty;
	w = p_win->width;
	h = p_win->height;

    mvaddch(y, x, p_win->border.top_l);
	mvaddch(y, x + w, p_win->border.top_r);
	mvaddch(y + h, x, p_win->border.bottom_l);
	mvaddch(y + h, x + w, p_win->border.bottom_r);
	mvvline(m_y + 1, m_x, middle_line, h-1);
	mvhline(y, x + 1, p_win->border.top_s, w - 1);
	mvhline(y + h, x + 1, p_win->border.bottom_s, w - 1);
	mvvline(y + 1, x, p_win->border.left_s, h-1);
	mvvline(y + 1, x + w, p_win->border.right_s, h - 1);

	refresh();
}
void init_players(WIN* p_win, client_data_t *p_pos,int num_p){
	int goal_keeper_x, goal_keeper_y, defense_x, defense_y, attack_x, attack_y;
	chtype goal_keeper = '0';
	chtype defense = '1';
	chtype attack = '2';

	int i = 0;


	while (i != num_p) {
		if (p_pos[i].role == 0) {
			goal_keeper_x = p_win->startx + p_pos[i].posX;
			goal_keeper_y = p_win->starty + p_pos[i].posY;
			mvaddch(goal_keeper_y, goal_keeper_x ,goal_keeper);
		}else if (p_pos[i].role == 1){
			defense_x =   p_win->startx + p_pos[i].posX;
			defense_y = p_win->starty + p_pos[i].posY;
			mvaddch(defense_y , defense_x, defense);
		}else if(p_pos[i].role == 2){
			attack_x = p_win->startx +  p_pos[i].posX;
			attack_y =  p_win->starty + p_pos[i].posY;
			mvaddch(attack_y, attack_x, attack);
		}
		i++;
	}
	refresh();
}
void init_fld(WIN* p_win){
    initscr();
    start_color();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_win_params(p_win);
    attron(COLOR_PAIR(1));
    printw("Press F1 to exit");
    refresh();
    attroff(COLOR_PAIR(1));
};
