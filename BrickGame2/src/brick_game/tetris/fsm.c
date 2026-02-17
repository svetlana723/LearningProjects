#include "./headers/fsm.h"

static game_state_t* game = NULL;

void initialize() {
  if (game) {
    free(game);
  }
  game = (game_state_t*)malloc(sizeof(game_state_t));
  if (game) {
    init_state(game);
  }
}

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  process_state(game, get_signal_by_action(action));
}

GameInfo_t updateCurrentState() {
  if (game) {
    return get_game_info(game);
  }

  GameInfo_t empty_info = {0};
  empty_info.field = NULL;
  empty_info.next = NULL;
  return empty_info;
}

void freeGameInfo(GameInfo_t* info) {
  if (info->field) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
      if (info->field[i]) {
        free(info->field[i]);
        info->field[i] = NULL;
      }
    }
    free(info->field);
    info->field = NULL;
  }
}

void cleanup() {
  if (game) {
    free(game);
    game = NULL;
  }
}

game_state_t* get_game_state() { return game; }

TetrisSignal get_signal_by_action(UserAction_t action) {
  TetrisSignal signal = NO_SIGNAL;
  switch (action) {
    case Start:
      signal = START;
      break;
    case Pause:
      signal = PAUSE;
      break;
    case Terminate:
      signal = EXIT;
      break;
    case Left:
      signal = MOVE_LEFT;
      break;
    case Right:
      signal = MOVE_RIGHT;
      break;
    case Up:
      signal = ROTATE;
      break;
    case Action:
    case Down:
      signal = MOVE_DOWN;
      break;
    default:
      signal = NO_SIGNAL;
      break;
  }
  return signal;
}

void init_state(game_state_t* state) {
  state->stats.level = 1;
  state->stats.score = 0;
  state->state = GAME_START;

  state->figure.type = NO_FIGURE;
  state->figure.rotate = ROTATE_0;
  state->figure.x = 0;
  state->figure.y = 0;
  state->figure.centre_x = 0;
  state->figure.centre_y = 0;

  state->next_figure_type = get_next_figure_type();

  init_board(&state->board_filled);
}

void process_figure_moving(game_state_t* game_state, TetrisSignal signal,
                           states* new_state) {
  int x_old = game_state->figure.x;
  int y_old = game_state->figure.y;
  rotate_step rotate_old = game_state->figure.rotate;

  if (signal == MOVE_LEFT) {
    game_state->figure.x--;
  } else if (signal == MOVE_RIGHT) {
    game_state->figure.x++;
  } else if (signal == ROTATE) {
    game_state->figure.rotate = get_next_rotate(&game_state->figure);
    set_figure_shape(&game_state->figure);
  } else if (signal == AUTO_MOVING) {
    game_state->figure.y++;
  } else if (signal == MOVE_DOWN) {
    game_state->figure.y =
        get_possible_y(&game_state->board_filled, &game_state->figure);
  }

  if (!is_possible_position(&game_state->board_filled, &game_state->figure)) {
    game_state->figure.x = x_old;
    game_state->figure.y = y_old;

    if (rotate_old != game_state->figure.rotate) {
      game_state->figure.rotate = rotate_old;
      set_figure_shape(&game_state->figure);
    }
  }

  if (game_state->figure.y ==
      get_possible_y(&game_state->board_filled, &game_state->figure))
    *new_state = FIGURE_DOWN;
  else
    *new_state = FIGURE_MOVING;
}

void process_state(game_state_t* game_state, TetrisSignal signal) {
  states current_state = game_state->state;
  states new_state = current_state;

  switch (current_state) {
    case GAME_START:
      if (signal == START) new_state = FIGURE_SPAWN;
      break;

    case FIGURE_SPAWN:
      init_figure(&game_state->figure, game_state->next_figure_type);
      game_state->next_figure_type = get_next_figure_type();
      if (is_possible_position(&game_state->board_filled,
                               &game_state->figure)) {
        new_state = FIGURE_MOVING;
      } else {
        new_state = GAME_OVER;
      }
      break;

    case FIGURE_MOVING:
      if (signal == PAUSE) {
        new_state = GAME_PAUSE;
      } else {
        process_figure_moving(game_state, signal, &new_state);
      }
      break;

    case FIGURE_DOWN:
      set_figure_on_board(&game_state->board_filled, &game_state->figure, 1);
      remove_lines(game_state);
      game_state->figure.type = NO_FIGURE;
      new_state = FIGURE_SPAWN;
      break;

    case GAME_PAUSE:
      if (signal == START || signal == PAUSE) new_state = FIGURE_MOVING;
      break;

    case GAME_OVER:
      if (signal == START) {
        init_state(game_state);
        new_state = FIGURE_SPAWN;
      }
      break;
  }
  game_state->state = new_state;
}

board_t get_board_to_print(const game_state_t* game_state) {
  board_t board_print;

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++)
      if (game_state->board_filled.cells[i][j] == 1)
        board_print.cells[i][j] = 1;
      else
        board_print.cells[i][j] = 0;
  }

  if (game_state->figure.type != NO_FIGURE)
    set_figure_on_board(&board_print, &game_state->figure, 1);

  return board_print;
}

GameInfo_t get_game_info(const game_state_t* game_state) {
  GameInfo_t game_info;
  game_info.score = game_state->stats.score;
  game_info.high_score = game_state->stats.score;
  game_info.level = game_state->stats.level;
  game_info.speed = game_state->stats.level;
  game_info.pause = (game_state->state == GAME_PAUSE);
  game_info.over = (game_state->state == GAME_OVER);
  game_info.next = NULL;

  board_t board_print = get_board_to_print(game_state);

  game_info.field = malloc(BOARD_HEIGHT * sizeof(int*));
  if (game_info.field) {
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
      game_info.field[i] = malloc(BOARD_WIDTH * sizeof(int));
      if (game_info.field[i]) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
          game_info.field[i][j] = board_print.cells[i][j];
        }
      }
    }
  }

  return game_info;
}
