#ifndef _USR_UI_H_
#define _USR_UI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

typedef struct _win_border_struct {
	chtype 	left_s, right_s, top_s, bottom_s,
	 	top_l, top_r, bottom_l, bottom_r;
}WIN_BORDER;

typedef struct _WIN_struct {
	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;

void init_win_params(WIN *p_win);
void create_box(WIN *win);

#endif
