#include "tetris_controller.h"

using namespace s21;

namespace s21 {

TetrisController::TetrisController() : view_(nullptr) {
  initialize();
  game_timer_ = new QTimer(this);
  connect(game_timer_, &QTimer::timeout, this,
          &TetrisController::HandleAutoMove);

  current_state = get_game_state()->state;
  current_level = get_game_state()->stats.level;
}

TetrisController::~TetrisController() {
  if (game_timer_) {
    game_timer_->stop();
    delete game_timer_;
  }
}

void TetrisController::SetView(SnakeView* view) {
  view_ = view;
  UpdateView();
  if (view_) {
    view_->SetController(this);
  }
}

void TetrisController::StartGame() {
  if (!game_active_) {
    game_active_ = true;
    game_paused_ = false;
    if (game_timer_) {
      game_timer_->start();
    }
    UpdateView();
  }
}

void TetrisController::HandleKeyPress(QKeyEvent* event) {
  TetrisSignal signal;
  switch (event->key()) {
    case Qt::Key_Left:
      signal = TetrisSignal::MOVE_LEFT;
      break;
    case Qt::Key_Right:
      signal = TetrisSignal::MOVE_RIGHT;
      break;
    case Qt::Key_Up:
      signal = TetrisSignal::ROTATE;
      break;
    case Qt::Key_Space:
    case Qt::Key_Down:
      signal = TetrisSignal::MOVE_DOWN;
      break;
    case Qt::Key_P:
      signal = TetrisSignal::PAUSE;
      break;
    case Qt::Key_Q:
      signal = TetrisSignal::EXIT;
      break;
    case Qt::Key_S:
      signal = TetrisSignal::START;
      break;
  }

  ProcessSignal(signal);
}

void TetrisController::HandleAutoMove() {
  if (current_state != states::GAME_START &&
      current_state != states::GAME_PAUSE &&
      current_state != states::GAME_OVER) {
    ProcessSignal(TetrisSignal::AUTO_MOVING);
  }
}

void TetrisController::ProcessSignal(TetrisSignal signal) {
  if (signal == TetrisSignal::EXIT) {
    CloseGame();
  }

  process_state(get_game_state(), signal);
  current_state = get_game_state()->state;
  UpdateView();

  if (current_state == states::GAME_OVER) {
    ShowGameOverMessage();
  }
}

void TetrisController::UpdateView() {
  GameInfo_t game_info = updateCurrentState();
  HighScore hs;
  hs.name = "";
  hs.score = game_info.high_score;
  view_->UpdateView(game_info, hs);
  UpdateSpeed(game_info.speed);
  freeGameInfo(&game_info);
}

void TetrisController::UpdateSpeed(int speed) {
  game_timer_->stop();
  game_timer_->start(PAUSE_INITIAL - PAUSE_STEP * speed);
}

vector<vector<BoardCell>> TetrisController::GenerateBoard() const {
  vector<vector<BoardCell>> board;
  board_t board_print = get_board_to_print(get_game_state());

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    vector<BoardCell> board_row;
    for (int j = 0; j < BOARD_WIDTH; j++) {
      BoardCell cell;
      cell.x = j;
      cell.y = i;

      if (board_print.cells[i][j] == 1)
        cell.type = CellType::Apple;
      else
        cell.type = CellType::Empty;

      board_row.push_back(cell);
    }
    board.push_back(board_row);
  }
  return board;
}

void TetrisController::CloseGame() { QCoreApplication::quit(); }

void TetrisController::ShowGameOverMessage() {
  MessageDialog::showMessage(WINDOW_TETRIS_TITLE, GAMEOVER_MESSAGE);
}

}  // namespace s21