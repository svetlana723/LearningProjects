#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "figure.h"
#include "objects.h"

void init_board(board_t* board);
int is_possible_position(const board_t* filled_board, const figure_t* figure);
int get_possible_y(const board_t* filled_board, const figure_t* figure);
void set_figure_on_board(board_t* board, const figure_t* figure,
                         char figure_char);
void remove_lines(game_state_t* state);
void find_filled_lines(const board_t* board, int filled_line_index[4],
                       int* filled_line_count);
void remove_filled_lines(board_t* board, const int filled_line_index[4],
                         int filled_line_count);

#endif