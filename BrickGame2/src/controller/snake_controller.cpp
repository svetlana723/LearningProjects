#include "snake_controller.h"

#include <QKeyEvent>
#include <QTimer>

namespace s21 {

SnakeController::SnakeController()
    : view_(nullptr),
      game_timer_(nullptr),
      game_active_(false),
      game_paused_(false) {
  initialize();
  game_timer_ = new QTimer();
  QObject::connect(game_timer_, &QTimer::timeout,
                   [this]() { HandleAutoMove(); });
}

SnakeController::~SnakeController() {
  if (game_timer_) {
    game_timer_->stop();
    delete game_timer_;
  }
}

void SnakeController::FreeGameInfo(GameInfo_t &info) {
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

void SnakeController::SetView(SnakeView *view) {
  view_ = view;
  if (view_) {
    view_->SetController(this);
  }
  GameInfo_t info = updateCurrentState();
  HighScore highscore = game_ptr->GetHighScore();
  UpdateView(info, highscore);
  FreeGameInfo(info);
}

void SnakeController::HandleKeyPress(QKeyEvent *event) {
  if (!event) return;
  SnakeSignal signal;
  switch (event->key()) {
    case Qt::Key_Left:
      signal = SnakeSignal::LEFT;
      break;
    case Qt::Key_Right:
      signal = SnakeSignal::RIGHT;
      break;
    case Qt::Key_Up:
      signal = SnakeSignal::UP;
      break;
    case Qt::Key_Down:
      signal = SnakeSignal::DOWN;
      break;
    case Qt::Key_P:
      signal = SnakeSignal::PAUSE;
      break;
    case Qt::Key_Q:
      signal = SnakeSignal::EXIT;
      break;
    case Qt::Key_S:
      signal = SnakeSignal::START;
      break;
    default:
      signal = SnakeSignal::NO_SIGNAL;
      break;
  }
  ProcessSignal(signal);
}

void SnakeController::HandleAutoMove() {
  if (game_active_ && !game_paused_) {
    ProcessSignal(SnakeSignal::AUTO_MOVING);
  }
}

void SnakeController::ProcessSignal(SnakeSignal signal) {
  if (signal == SnakeSignal::EXIT) {
    CloseGame();
  }

  game_ptr->HandleSignalState(signal);
  GameInfo_t info = updateCurrentState();
  HighScore highscore = game_ptr->GetHighScore();

  if (info.over) {
    game_active_ = false;
    if (game_timer_) {
      game_timer_->stop();
    }
  } else if (info.pause) {
    game_paused_ = true;
    if (game_timer_) {
      game_timer_->stop();
    }
  } else {
    game_paused_ = false;
    game_active_ = true;
    if (game_timer_ && !game_timer_->isActive()) {
      game_timer_->start();
    }
  }

  UpdateView(info, highscore);
  FreeGameInfo(info);

  if (game_ptr->GetState() == SnakeGameState::GAME_OVER) {
    if (game_ptr->WinGame())
      ShowWinMessage();
    else if (game_ptr->NewHighScore())
      SaveNewHighScore();
    else if (signal != SnakeSignal::EXIT)
      ShowGameOverMessage();

    game_ptr->HandleSignalState(SnakeSignal::AUTO_MOVING);
  }
}

void SnakeController::UpdateView(const GameInfo_t &info,
                                 const HighScore &highscore) {
  if (!view_ || !game_ptr) return;

  view_->UpdateView(info, highscore);
  UpdateSpeed(info.speed);
}

void SnakeController::UpdateSpeed(int speed) {
  game_timer_->stop();
  game_timer_->start(PAUSE_INITIAL - PAUSE_STEP * speed);
}

void SnakeController::CloseGame() { QCoreApplication::quit(); }

void SnakeController::SaveNewHighScore() {
  NameDialog nameDialog;
  string player_name = "";
  if (nameDialog.exec() == QDialog::Accepted) {
    player_name = nameDialog.getName();
  } else {
    player_name = "";
  }
  game_ptr->SaveNewHighScore(player_name);
}

void SnakeController::ShowWinMessage() {
  MessageDialog::showMessage(WINDOW_SNAKE_TITLE, WIN_MESSAGE);
}

void SnakeController::ShowGameOverMessage() {
  MessageDialog::showMessage(WINDOW_SNAKE_TITLE, GAMEOVER_MESSAGE);
}

}  // namespace s21