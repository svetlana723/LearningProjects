#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QStyle>

#include "../../brick_game/tetris/headers/fsm.h"
#include "../../controller/tetris_controller.h"
#include "snake_view.h"

using namespace s21;

int main(int argc, char *argv[]) {
  srand(time(NULL));

  QApplication app(argc, argv);
  TetrisController controller;
  SnakeView view;
  controller.SetView(&view);

  controller.StartGame();
  view.installEventFilter(&controller);
  view.show();

  int result = app.exec();
  cleanup();

  return result;
}