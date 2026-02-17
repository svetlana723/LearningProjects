#include "snake_view.h"

namespace s21 {

SnakeView::SnakeView(QWidget* parent) : QWidget(parent), controller_(nullptr) {
  setFocusPolicy(Qt::StrongFocus);
  setFocus();
  int width = LEFT_OFFSET * 2 + (BOARD_WIDTH + 1) * SQUARE_SIZE + STATS_WIDTH +
              BETWEEN_OFFSET;
  int height = TOP_OFFSET * 2 + (BOARD_HEIGHT + 1) * SQUARE_SIZE;
  setFixedSize(width, height);

  game_info_copy.score = 0;
  game_info_copy.high_score = 0;
  game_info_copy.level = 0;
  game_info_copy.speed = 0;
  game_info_copy.pause = 0;
  game_info_copy.over = 0;
  game_info_copy.field = nullptr;
  game_info_copy.next = nullptr;

  highscore_.score = 0;
  highscore_.name = "";
}

SnakeView::~SnakeView() { FreeGameInfo(game_info_copy); }

void SnakeView::SetController(BaseController* controller) {
  controller_ = controller;
}

void SnakeView::FreeGameInfo(GameInfo_t& info) {
  if (info.field) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
      if (info.field[i]) {
        delete[] info.field[i];
        info.field[i] = nullptr;
      }
    }
    delete[] info.field;
    info.field = nullptr;
  }
}

void SnakeView::UpdateView(const GameInfo_t& game_info,
                           const HighScore& highscore) {
  game_info_copy.score = game_info.score;
  game_info_copy.high_score = game_info.high_score;
  game_info_copy.level = game_info.level;
  game_info_copy.speed = game_info.speed;
  game_info_copy.pause = game_info.pause;
  game_info_copy.over = game_info.over;

  if (game_info.field) {
    if (game_info_copy.field == nullptr) {
      game_info_copy.field = new int*[BOARD_HEIGHT];
      for (int i = 0; i < BOARD_HEIGHT; i++) {
        game_info_copy.field[i] = new int[BOARD_WIDTH];
      }
    }
    for (int i = 0; i < BOARD_HEIGHT; i++)
      for (int j = 0; j < BOARD_WIDTH; j++)
        game_info_copy.field[i][j] = game_info.field[i][j];

  } else
    game_info_copy.field = nullptr;
  highscore_ = highscore;
  update();
}

void SnakeView::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  DrawGameBoard(painter);
  DrawGameStats(painter);
  DrawHighScore(painter);
  DrawInfo(painter);
}

void SnakeView::DrawGameBoard(QPainter& painter) {
  const int squareSize = SQUARE_SIZE;

  painter.setBrush(QBrush(COLOR_FRAME));
  painter.setPen(QPen(Qt::black, 1));
  painter.drawRect(LEFT_OFFSET, TOP_OFFSET, squareSize * (BOARD_WIDTH + 1),
                   squareSize * (BOARD_HEIGHT + 1));

  for (size_t i = 0; i < BOARD_HEIGHT; i++) {
    for (size_t j = 0; j < BOARD_WIDTH; j++) {
      QColor cell_color;
      if (game_info_copy.field[i][j] == 1) {
        cell_color = COLOR_SNAKE;
      } else
        cell_color = COLOR_EMPTY;

      painter.setBrush(QBrush(cell_color));
      painter.setPen(QPen(Qt::black, 1));
      painter.drawRect(LEFT_OFFSET + squareSize * (0.5 + j),
                       TOP_OFFSET + squareSize * (0.5 + i), squareSize,
                       squareSize);
    }
  }
}

void SnakeView::DrawGameStats(QPainter& painter) {
  int left_offset =
      LEFT_OFFSET + BETWEEN_OFFSET + SQUARE_SIZE * (BOARD_WIDTH + 1.5);
  int top_offset = TOP_OFFSET + SQUARE_SIZE * 0.5;
  painter.setBrush(QBrush(COLOR_FRAME));
  painter.setPen(QPen(Qt::black, 1));
  painter.drawRect(left_offset - SQUARE_SIZE * 0.5, TOP_OFFSET,
                   STATS_WIDTH + SQUARE_SIZE, STATS_HEIGHT + SQUARE_SIZE);

  painter.setBrush(QBrush(COLOR_EMPTY));
  painter.setPen(QPen(Qt::black, 1));
  painter.drawRect(left_offset, top_offset, STATS_WIDTH, STATS_HEIGHT);

  painter.setPen(Qt::black);
  painter.drawText(left_offset + TEXT_LEFT_OFFSET, top_offset + TEXT_TOP_OFFSET,
                   QString("Score: %1").arg(game_info_copy.score));
  painter.drawText(left_offset + TEXT_LEFT_OFFSET,
                   top_offset + TEXT_TOP_OFFSET + TEXT_ROW_HEIGHT,
                   QString("Level: %1").arg(game_info_copy.level));
  painter.drawText(left_offset + TEXT_LEFT_OFFSET,
                   top_offset + TEXT_TOP_OFFSET + TEXT_ROW_HEIGHT * 2,
                   QString("Speed: %1").arg(game_info_copy.speed));
}

void SnakeView::DrawHighScore(QPainter& painter) {
  int left_offset =
      LEFT_OFFSET + BETWEEN_OFFSET + SQUARE_SIZE * (BOARD_WIDTH + 1.5);
  int top_offset =
      TOP_OFFSET + BETWEEN_OFFSET + STATS_HEIGHT + SQUARE_SIZE * 1.5;
  painter.setBrush(QBrush(COLOR_FRAME));
  painter.setPen(QPen(Qt::black, 1));
  painter.drawRect(left_offset - SQUARE_SIZE * 0.5,
                   top_offset - SQUARE_SIZE * 0.5, STATS_WIDTH + SQUARE_SIZE,
                   STATS_HEIGHT + SQUARE_SIZE);

  painter.setBrush(QBrush(COLOR_EMPTY));
  painter.setPen(QPen(Qt::black, 1));
  painter.drawRect(left_offset, top_offset, STATS_WIDTH, STATS_HEIGHT);

  painter.setPen(Qt::black);
  painter.drawText(left_offset + TEXT_LEFT_OFFSET, top_offset + TEXT_TOP_OFFSET,
                   QString("HighScore:"));
  painter.drawText(
      left_offset + TEXT_LEFT_OFFSET,
      top_offset + TEXT_TOP_OFFSET + TEXT_ROW_HEIGHT,
      QString("Name: %1").arg(QString::fromStdString(highscore_.name)));
  painter.drawText(left_offset + TEXT_LEFT_OFFSET,
                   top_offset + TEXT_TOP_OFFSET + TEXT_ROW_HEIGHT * 2,
                   QString("Score: %1").arg(highscore_.score));
}

void SnakeView::DrawInfo(QPainter& painter) {
  int left_offset =
      LEFT_OFFSET + BETWEEN_OFFSET + SQUARE_SIZE * (BOARD_WIDTH + 1.5);
  int top_offset =
      TOP_OFFSET + BETWEEN_OFFSET * 2 + STATS_HEIGHT * 2 + SQUARE_SIZE * 2.5;
  painter.setBrush(QBrush(COLOR_FRAME));
  painter.setPen(QPen(Qt::black, 1));
  painter.drawRect(left_offset - SQUARE_SIZE * 0.5,
                   top_offset - SQUARE_SIZE * 0.5, STATS_WIDTH + SQUARE_SIZE,
                   STATS_HEIGHT + SQUARE_SIZE);

  painter.setBrush(QBrush(COLOR_EMPTY));
  painter.setPen(QPen(Qt::black, 1));
  painter.drawRect(left_offset, top_offset, STATS_WIDTH, STATS_HEIGHT);

  painter.setPen(Qt::black);
  painter.drawText(left_offset + TEXT_LEFT_OFFSET, top_offset + TEXT_TOP_OFFSET,
                   QString("Press 'S' to start"));
  painter.drawText(left_offset + TEXT_LEFT_OFFSET,
                   top_offset + TEXT_TOP_OFFSET + TEXT_ROW_HEIGHT,
                   QString("Press 'P' to pause"));
  painter.drawText(left_offset + TEXT_LEFT_OFFSET,
                   top_offset + TEXT_TOP_OFFSET + TEXT_ROW_HEIGHT * 2,
                   QString("Press 'Q' to quit"));
}

void SnakeView::keyPressEvent(QKeyEvent* event) {
  if (controller_) {
    controller_->HandleKeyPress(event);
  } else {
    QWidget::keyPressEvent(event);
  }
}

}  // namespace s21