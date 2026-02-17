#include <QApplication>

#include "../../brick_game/snake/snake_model.h"
#include "../../controller/snake_controller.h"
#include "snake_view.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  s21::SnakeView view;
  s21::SnakeController controller;
  controller.SetView(&view);
  view.show();

  int result = app.exec();
  s21::cleanup();

  return result;
}