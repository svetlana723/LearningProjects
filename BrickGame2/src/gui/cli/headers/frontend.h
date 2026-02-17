#ifndef TETRIS_FRONTEND_H
#define TETRIS_FRONTEND_H

#include <ncurses.h>
#include <string.h>

#include "../../../brick_game/tetris/headers/fsm.h"

enum window_type { PAUSE_WINDOW, GAMEOVER_WINDOW };

void print_overlay(void);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(GameInfo_t *game_info);
void print_high_score(GameInfo_t *game_info);
void print_board(int **field);
void print_next_figure(figure_type next_figure_type);
WINDOW *create_message_window(enum window_type type);
void destroy_message_window(WINDOW **message_window);
void create_input_window(char *input_name);

#endif