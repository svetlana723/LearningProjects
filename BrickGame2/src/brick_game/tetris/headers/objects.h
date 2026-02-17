#ifndef OBJECTS_H
#define OBJECTS_H

#include "../../common.h"
#include "defines.h"

//    ####  #      #  ##   ##   #   ##
//          ###  ###  ##  ##   ###   ##
//    1     2    3    4   5    6    7
typedef enum {
  NO_FIGURE = 0,
  FIG_1,
  FIG_2,
  FIG_3,
  FIG_4,
  FIG_5,
  FIG_6,
  FIG_7
} figure_type;

typedef enum { ROTATE_0 = 0, ROTATE_1, ROTATE_2, ROTATE_3 } rotate_step;

typedef enum {
  GAME_START = 0,
  FIGURE_SPAWN,
  FIGURE_MOVING,
  FIGURE_DOWN,
  GAME_PAUSE,
  GAME_OVER
} states;

typedef enum {
  NO_SIGNAL = 0,
  MOVE_RIGHT,
  MOVE_LEFT,
  MOVE_DOWN,
  AUTO_MOVING,
  ROTATE,
  START,
  PAUSE,
  EXIT
} TetrisSignal;

typedef struct {
  int x;
  int y;
  figure_type type;
  rotate_step rotate;
  char shape[4][4];
  int centre_x;
  int centre_y;
} figure_t;

typedef struct {
  char cells[BOARD_HEIGHT][BOARD_WIDTH];
} board_t;

typedef struct {
  int score;
  int level;
} game_stats_t;

typedef struct game_state {
  game_stats_t stats;
  states state;
  board_t board_filled;
  figure_t figure;
  figure_type next_figure_type;
} game_state_t;

typedef struct high_score {
  int score;
  char name[MAX_NAME_LENGTH + 1];
} high_score_t;

#endif