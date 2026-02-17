#ifndef SNAKE_CONTROLLER_H
#define SNAKE_CONTROLLER_H

#include "../brick_game/common.h"
#include "../brick_game/snake/snake_constants.h"
#include "../brick_game/snake/snake_model.h"
#include "../gui/desktop/message_dialog.h"
#include "../gui/desktop/name_dialog.h"
#include "../gui/desktop/snake_view.h"
#include "base_controller.h"

class QTimer;
class QKeyEvent;

namespace s21 {

class SnakeController : public BaseController {
 public:
  SnakeController();
  ~SnakeController();

  void SetView(SnakeView *view);
  void StartGame();
  void HandleKeyPress(QKeyEvent *event) override;
  void HandleAutoMove();

 private:
  SnakeView *view_;
  QTimer *game_timer_;
  bool game_active_;
  bool game_paused_;

  void ProcessSignal(SnakeSignal signal);
  void UpdateView(const GameInfo_t &info, const HighScore &highscore);
  void UpdateSpeed(int speed);
  void CloseGame();
  void SaveNewHighScore();
  void ShowWinMessage();
  void ShowGameOverMessage();
  void FreeGameInfo(GameInfo_t &info);
};

}  // namespace s21

#endif  // SNAKE_CONTROLLER_H