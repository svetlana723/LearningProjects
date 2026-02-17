#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "headers/snake_controller_c.h"

void init_ncurses();

int main() {
  init_ncurses();
  print_overlay();

  initialize_controller();
  game_loop();

  cleanup_controller();
  endwin();
  return 0;
}

void init_ncurses() {
  srand(time(NULL));
  initscr();
  keypad(stdscr, TRUE);
  start_color();
  curs_set(0);
  noecho();
}
