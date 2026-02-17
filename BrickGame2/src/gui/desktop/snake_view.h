#ifndef SNAKE_VIEW_H
#define SNAKE_VIEW_H

#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <vector>

#include "../../brick_game/common.h"
#include "../../brick_game/snake/snake_constants.h"
#include "../../controller/base_controller.h"
#include "view_defines.h"

using namespace std;

namespace s21 {

class SnakeView : public QWidget {
 public:
  explicit SnakeView(QWidget* parent = nullptr);
  ~SnakeView();
  void SetController(BaseController* controller);
  void UpdateView(const GameInfo_t& game_info, const HighScore& highscore);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  BaseController* controller_;
  GameInfo_t game_info_copy;
  HighScore highscore_;

  void FreeGameInfo(GameInfo_t& info);
  void DrawGameBoard(QPainter& painter);
  void DrawGameStats(QPainter& painter);
  void DrawHighScore(QPainter& painter);
  void DrawInfo(QPainter& painter);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
};
}  // namespace s21
#endif  // SNAKE_VIEW_H