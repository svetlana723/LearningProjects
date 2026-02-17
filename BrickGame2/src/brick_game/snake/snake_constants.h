#ifndef SNAKE_CONSTANTS_H
#define SNAKE_CONSTANTS_H

#define INITIAL_SNAKE_LENGTH 4
#define MAX_SNAKE_LENGTH 200

#define MAX_LEVEL 10
#define LEVEL_INC 5

#define SNAKE_HIGHSCORE_FILE "./build/snake_high_scores"

#include <string>

namespace s21 {

struct HighScore {
  int score;
  std::string name;
};

enum class CellType { Empty, Snake, Apple };

struct BoardCell {
  int x;
  int y;
  CellType type;
};

enum class Direction { UP, DOWN, LEFT, RIGHT };

enum class SnakeGameState {
  MENU,
  PLAYING,
  PAUSE,
  GAME_OVER,
};

enum class SnakeSignal {
  NO_SIGNAL = 0,
  START,
  PAUSE,
  EXIT,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  AUTO_MOVING
};

}  // namespace s21

#endif