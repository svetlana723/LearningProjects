#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../brick_game/tetris/headers/fsm.h"
#include "frontend.h"

void init();
int main();
void game_loop();
signals get_signal_by_key(int key);
void update_highscore(int current_score, int* new_highscore, high_score_t* hs);
void update_level(int* current_level, int new_level, int* pause_time);
void save_highscore(int* new_highscore, high_score_t* hs);
void set_auto_moving(states state, int pause_time);
void set_auto_moving_on(int level);
void set_auto_moving_off();
int set_pause_on(WINDOW** message_window);
int set_pause_off(WINDOW* message_window, int pause_time);
int set_game_over_on(WINDOW** message_window);
int set_game_over_off(WINDOW* message_window);
high_score_t read_highscore();
void update_highscore(int current_score, int* new_highscore, high_score_t* hs);
void write_highscore(const high_score_t* hs);
void save_new_highscore(high_score_t* high_score);

#endif