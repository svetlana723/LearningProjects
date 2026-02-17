#include "headers/frontend.h"

void print_overlay() {
  if (has_colors()) {
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(1));
  }
  print_rectangle(0, BOARD_HEIGHT + 1, 0, BOARD_WIDTH + 1);
  print_rectangle(0, BOARD_HEIGHT + 1, BOARD_WIDTH + 2,
                  BOARD_WIDTH + STAT_WIDTH + 3);

  print_rectangle(1, 3, BOARD_WIDTH + 3, BOARD_WIDTH + STAT_WIDTH + 2);
  print_rectangle(4, 6, BOARD_WIDTH + 3, BOARD_WIDTH + STAT_WIDTH + 2);
  print_rectangle(7, 13, BOARD_WIDTH + 3, BOARD_WIDTH + STAT_WIDTH + 2);
  print_rectangle(15, 20, BOARD_WIDTH + 3, BOARD_WIDTH + STAT_WIDTH + 2);

  if (has_colors()) attron(COLOR_PAIR(3));
  mvprintw(2, BOARD_WIDTH + 5, "LEVEL");
  mvprintw(5, BOARD_WIDTH + 5, "SCORE");
  mvprintw(8, BOARD_WIDTH + 5, "NEXT");
  mvprintw(16, BOARD_WIDTH + 5, "HIGH SCORE");
  mvprintw(BOARD_HEIGHT + 2, 2, START_MESSAGE);
  mvprintw(BOARD_HEIGHT + 3, 2, PAUSE_MESSAGE);
  mvprintw(BOARD_HEIGHT + 4, 2, EXIT_MESSAGE);
  if (has_colors()) {
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
  }
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  for (int i = left_x + 1; i < right_x; i++) {
    mvaddch(top_y, i, ACS_HLINE);
    mvaddch(bottom_y, i, ACS_HLINE);
  }
  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }
  mvaddch(top_y, left_x, ACS_ULCORNER);
  mvaddch(top_y, right_x, ACS_URCORNER);
  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  mvaddch(bottom_y, right_x, ACS_LRCORNER);
}

void print_stats(GameInfo_t *game_info) {
  mvprintw(2, BOARD_WIDTH + 11, "%d", game_info->level);
  mvprintw(5, BOARD_WIDTH + 11, "%d", game_info->score);
}

void print_high_score(GameInfo_t *game_info) {
  if (game_info->high_score > 0) {
    mvprintw(18, BOARD_WIDTH + 5, "%s", "                 ");
    mvprintw(18, BOARD_WIDTH + 5, "%s", "Player");
    mvprintw(18, BOARD_WIDTH + 19, "%d", game_info->high_score);
  } else {
    mvprintw(18, BOARD_WIDTH + 5, "%s", "No highscores yet");
  }
}

void print_board(int **field) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++)
      if (field[i][j] == 1)
        mvaddch(1 + i, 1 + j, CHAR_FIGURE);
      else
        mvaddch(1 + i, 1 + j, CHAR_EMPTY);
  }
}

WINDOW *create_message_window(enum window_type type) {
  int window_y = (GAME_BOX_HEIGHT - MESSAGE_WINDOW_HEIGHT) / 2 - 3;
  int window_x = (GAME_BOX_WIDTH - MESSAGE_WINDOW_WIDTH) / 2;

  WINDOW *message_win =
      newwin(MESSAGE_WINDOW_HEIGHT, MESSAGE_WINDOW_WIDTH, window_y, window_x);
  box(message_win, ACS_VLINE, ACS_HLINE);

  if (type == PAUSE_WINDOW) {
    mvwprintw(message_win, 1, MESSAGE_WINDOW_WIDTH / 2 - 3, "PAUSED");
    mvwprintw(message_win, 3, 3, "Press 'P' to start");
  } else if (type == GAMEOVER_WINDOW) {
    mvwprintw(message_win, 1, MESSAGE_WINDOW_WIDTH / 2 - 5, "GAME OVER!");
    mvwprintw(message_win, 3, 3, "Press 'S' to start");
    mvwprintw(message_win, 4, 3, "Press 'Q' to exit");
  }

  wrefresh(message_win);
  return message_win;
}

void create_input_window(char *input_name) {
  int window_y = (GAME_BOX_HEIGHT - MESSAGE_WINDOW_HEIGHT) / 2 - 3;
  int window_x = (GAME_BOX_WIDTH - MESSAGE_WINDOW_WIDTH) / 2;

  WINDOW *input_win =
      newwin(MESSAGE_WINDOW_HEIGHT, MESSAGE_WINDOW_WIDTH, window_y, window_x);
  box(input_win, ACS_VLINE, ACS_HLINE);

  mvwprintw(input_win, 1, MESSAGE_WINDOW_WIDTH / 2 - 10,
            "You have a highscore!");
  mvwprintw(input_win, 2, 1, "Enter your name (< 10 chars)");
  wrefresh(input_win);

  echo();
  wmove(input_win, 4, MESSAGE_WINDOW_WIDTH / 2 - 5);
  wgetnstr(input_win, input_name, MAX_NAME_LENGTH);
  noecho();

  destroy_message_window(&input_win);
}

void destroy_message_window(WINDOW **message_win) {
  if (message_win != NULL && *message_win != NULL) {
    werase(*message_win);
    wrefresh(*message_win);
    delwin(*message_win);
    *message_win = NULL;
  }
}