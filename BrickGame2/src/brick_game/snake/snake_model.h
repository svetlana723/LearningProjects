#ifndef SNAKE_MODEL_H
#define SNAKE_MODEL_H

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <vector>

#include "snake_constants.h"

using namespace std;

namespace s21 {

#include "../common.h"

class GameSnake;

extern GameSnake* game_ptr;

void initialize();
void userInput(UserAction_t action, bool hold = false);
GameInfo_t updateCurrentState();
extern "C" void freeGameInfo(GameInfo_t& info);
void cleanup();

class GameSnake {
 public:
  GameSnake();
  ~GameSnake();

  void Init();
  void InitBoard();

  // for testing
  const std::deque<Point>& GetSnake() const { return snake; }
  void SetApple(int x, int y) { apple = {x, y}; }

  void HandleSignalState(SnakeSignal signal);
  void HandleMoving(SnakeSignal signal);
  SnakeSignal GetSignalByAction(UserAction_t action);

  vector<vector<BoardCell>> GenerateBoard() const;

  SnakeGameState GetState() const;
  void SetState(SnakeGameState new_state);

  bool NewHighScore();
  bool WinGame();
  HighScore GetHighScore() const;
  int GetLevel();
  int GetScore();
  void SaveNewHighScore(string player_name);

  GameInfo_t GetGameInfo() const;

 private:
  deque<Point> snake;
  Point apple;
  HighScore highscore;
  int score = 0;
  int level = 1;
  int speed = 1;
  bool paused = false;
  bool gameOver = false;
  bool closeGame = false;
  Direction current_moving;
  SnakeGameState current_state;
  bool new_highscore = false;
  bool win_game = false;

  void MoveSnake();
  void CreateNewApple();

  void CheckHighScore();
  void ReadHighScore();
  void WriteHighScore();
  void IncreaseLevel();
};

}  // namespace s21

#endif