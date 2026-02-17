#ifndef COMMON_H
#define COMMON_H

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define MAX_NAME_LENGTH 20

#define PAUSE_INITIAL 560
#define PAUSE_STEP 50

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  int over;
} GameInfo_t;

struct Point {
  int x;
  int y;
};

#endif