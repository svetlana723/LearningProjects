#ifndef TETRIS_CONTROLLER_H
#define TETRIS_CONTROLLER_H

#include <QCoreApplication>
#include <QKeyEvent>
#include <QObject>
#include <QTimer>

#include "base_controller.h"

extern "C" {
#include "../brick_game/tetris/headers/fsm.h"
}

#include "../gui/desktop/message_dialog.h"
#include "../gui/desktop/name_dialog.h"
#include "../gui/desktop/snake_view.h"

namespace s21 {

class SnakeView;

class TetrisController : public BaseController {
 public:
  TetrisController();
  ~TetrisController();

  void SetView(SnakeView* view);
  void HandleKeyPress(QKeyEvent* event);
  void StartGame();
  void SaveNewHighScore();
  void ShowWinMessage();
  void ShowGameOverMessage();

 public slots:
  void HandleAutoMove();

 private:
  SnakeView* view_;
  QTimer* game_timer_;
  bool game_active_;
  bool game_paused_;
  HighScore current_highscore_;

  void ProcessSignal(TetrisSignal signal);
  void UpdateSpeed(int speed);
  NameDialog* nameDialog;
  void CloseGame();

  states current_state;
  int current_level = 1;
  int pause_time = PAUSE_TIME;
  int run = 1, paused = 0, game_over = 0, new_highscore = 0;

  void UpdateView();
  vector<vector<BoardCell>> GenerateBoard() const;
};
}  // namespace s21
#endif  // TETRIS_CONTROLLER_H