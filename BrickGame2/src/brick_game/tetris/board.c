#include "./headers/board.h"

void init_board(board_t* board) {
  for (int i = 0; i < BOARD_HEIGHT; i++)
    for (int j = 0; j < BOARD_WIDTH; j++) board->cells[i][j] = 0;
}

void set_figure_on_board(board_t* board, const figure_t* figure,
                         char figure_char) {
  int x = figure->x;
  int y = figure->y;

  for (int i = 0; i < FIGURE_MAX_HEIGHT; i++)
    for (int j = 0; j < FIGURE_MAX_WIDTH; j++)
      if (figure->shape[j][i] == 1)
        board->cells[y - figure->centre_y + j][x - figure->centre_x + i] =
            figure_char;
}

int is_possible_position(const board_t* filled_board, const figure_t* figure) {
  int is_possible = 1;

  is_possible = ((figure->x - left_offset(figure) >= 0) &&
                 (figure->x + right_offset(figure) < BOARD_WIDTH) &&
                 (figure->y + down_offset(figure) < BOARD_HEIGHT));

  for (int i = 0; (i < FIGURE_MAX_WIDTH) && is_possible; i++)
    for (int j = 0; (j < FIGURE_MAX_HEIGHT) && is_possible; j++)
      if ((figure->shape[j][i] == 1) &&
          (filled_board->cells[figure->y - figure->centre_y + j]
                              [figure->x - figure->centre_x + i] == 1))
        is_possible = 0;

  return is_possible;
}

int get_possible_y(const board_t* filled_board, const figure_t* figure) {
  int new_y = figure->y;
  int run = 1;

  while (run) {
    int empty = 1;
    for (int i = 0; (i < FIGURE_MAX_WIDTH) && empty; i++)
      for (int j = 0; (j < FIGURE_MAX_HEIGHT) && empty; j++)
        if ((figure->shape[j][i] == 1) &&
            (filled_board->cells[new_y - figure->centre_y + j]
                                [figure->x - figure->centre_x + i] == 1))
          empty = 0;

    if (empty) {
      if (new_y < BOARD_HEIGHT - down_offset(figure) - 1)
        new_y++;
      else
        run = 0;
    } else {
      run = 0;
      new_y--;
    }
  }

  return new_y;
}

int get_score_adding(int line_count) {
  int result = 0;
  switch (line_count) {
    case 1:
      result = SCORE_1_LINE;
      break;
    case 2:
      result = SCORE_2_LINE;
      break;
    case 3:
      result = SCORE_3_LINE;
      break;
    case 4:
      result = SCORE_4_LINE;
      break;
    default:
      break;
  }
  return result;
}
void remove_lines(game_state_t* state) {
  int filled_line_index[4] = {0};
  int filled_line_count = 0;

  find_filled_lines(&state->board_filled, filled_line_index,
                    &filled_line_count);

  if (filled_line_count > 0) {
    int add_score = get_score_adding(filled_line_count);
    state->stats.score += add_score;

    if (state->stats.level < MAX_LEVEL &&
        state->stats.score >= (LEVEL_SCORE * state->stats.level)) {
      state->stats.level = state->stats.score / LEVEL_SCORE + 1;
    }

    remove_filled_lines(&state->board_filled, filled_line_index,
                        filled_line_count);
  }
}

void find_filled_lines(const board_t* board, int filled_line_index[4],
                       int* filled_line_count) {
  for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
    int filled_line = 1;

    for (int j = 0; j < BOARD_WIDTH; j++)
      if (board->cells[i][j] != 1) {
        filled_line = 0;
        break;
      }
    if (filled_line) {
      filled_line_index[*filled_line_count] = i;
      (*filled_line_count)++;
    }
  }
}

void remove_filled_lines(board_t* board, const int filled_line_index[4],
                         int filled_line_count) {
  int index = 0;
  int line_index = BOARD_HEIGHT - 1;
  char new_board[BOARD_HEIGHT][BOARD_WIDTH];

  for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
    if (index < filled_line_count && i == filled_line_index[index]) {
      index++;
    } else {
      for (int j = 0; j < BOARD_WIDTH; j++)
        new_board[line_index][j] = board->cells[i][j];
      line_index--;
    }
  }
  for (int i = line_index; i >= 0; i--) {
    for (int j = 0; j < BOARD_WIDTH; j++) new_board[i][j] = 0;
  }

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) board->cells[i][j] = new_board[i][j];
  }
}