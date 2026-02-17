#include "headers/snake_controller_c.h"

#include <ncurses.h>
#include <stdio.h>

#include "../../brick_game/snake/snake_model_c.h"

void initialize_controller() { snake_initialize(); }

void handle_input(int key) {
  UserAction_t action;

  switch (key) {
    case KEY_UP:
      action = Up;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case 's':
    case 'S':
      action = Start;
      break;
    case 'p':
    case 'P':
      action = Pause;
      break;
    case ' ':
      action = Action;
      break;
    case 'q':
    case 'Q':
      action = Terminate;
      break;
    default:
      return;
  }

  snake_user_input(action, false);
}

void cleanup_controller() { snake_cleanup(); }

void game_loop() {
  int run = 1, paused = 0, game_over = 0;
  int pause_time = PAUSE_TIME;
  WINDOW* message_window = NULL;

  halfdelay(pause_time);

  while (run) {
    int key = getch();

    if (key == 'q' || key == 'Q') {
      run = 0;
    } else if (key == 'p' || key == 'P') {
      paused = !paused;
      if (paused) {
        set_pause_on(&message_window);
      } else {
        set_pause_off(message_window, pause_time);
      }
    } else if (!paused && key != ERR) {
      if (game_over && (key == 's' || key == 'S')) {
        game_over = set_game_over_off(message_window);
        handle_input(key);
      }
      handle_input(key);
    }

    if (!paused && !game_over) {
      if (key == ERR) {
        handle_input(' ');
      }

      GameInfo_t info = snake_update_current_state();

      if (info.over) {
        game_over = set_game_over_on(&message_window);
      } else {
        print_stats(&info);
        print_board(info.field);
        update_speed(&info, &pause_time);
      }
      snake_free_gameinfo(&info);
    }
  }

  nocbreak();
}

void update_speed(GameInfo_t* info, int* pause_time) {
  *pause_time = PAUSE_TIME - info->level - 1;
  set_auto_moving_on(*pause_time);
}

int set_pause_on(WINDOW** message_window) {
  if (message_window != NULL)
    *message_window = create_message_window(PAUSE_WINDOW);
  set_auto_moving_off();
  return TRUE;
}

int set_pause_off(WINDOW* message_window, int pause_time) {
  destroy_message_window(&message_window);
  print_overlay();
  set_auto_moving_on(pause_time);
  return FALSE;
}

int set_game_over_on(WINDOW** message_window) {
  if (message_window != NULL)
    *message_window = create_message_window(GAMEOVER_WINDOW);
  set_auto_moving_off();
  return TRUE;
}

int set_game_over_off(WINDOW* message_window) {
  destroy_message_window(&message_window);
  print_overlay();
  return FALSE;
}

void set_auto_moving_on(int pause_time) { halfdelay(pause_time); }

void set_auto_moving_off() { cbreak(); }
