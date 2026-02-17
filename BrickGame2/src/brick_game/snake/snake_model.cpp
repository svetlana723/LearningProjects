#include "snake_model.h"

#include <time.h>
namespace s21 {

GameSnake* game_ptr = nullptr;

void initialize() {
  srand(time(NULL));

  if (game_ptr) {
    delete game_ptr;
  }
  game_ptr = new GameSnake();
  game_ptr->Init();
}

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  game_ptr->HandleSignalState(game_ptr->GetSignalByAction(action));
}

GameInfo_t updateCurrentState() {
  if (game_ptr) {
    GameInfo_t info = game_ptr->GetGameInfo();
    return info;
  }
  GameInfo_t empty_info;
  empty_info.score = 0;
  empty_info.high_score = 0;
  empty_info.level = 0;
  empty_info.speed = 0;
  empty_info.pause = 0;
  empty_info.over = 0;
  empty_info.field = nullptr;
  empty_info.next = nullptr;
  return empty_info;
}

void freeGameInfo(GameInfo_t& info) {
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

void cleanup() {
  if (game_ptr) {
    delete game_ptr;
    game_ptr = nullptr;
  }
}

GameSnake::GameSnake() : snake(), apple({0, 0}), highscore({0, ""}) { Init(); }

GameSnake::~GameSnake() {}

void GameSnake::Init() {
  current_state = SnakeGameState::MENU;
  current_moving = Direction::RIGHT;
  score = 0;
  level = 1;
  speed = 1;
  paused = false, gameOver = false, closeGame = false, win_game = false;
  new_highscore = false;
  ReadHighScore();
}

void GameSnake::InitBoard() {
  if (snake.size() > 0) {
    snake.clear();
  }

  int snake_pos_x = BOARD_WIDTH / 2;
  int snake_pos_y = BOARD_HEIGHT / 2;
  snake.push_back({snake_pos_x, snake_pos_y});
  for (int i = 1; i < INITIAL_SNAKE_LENGTH; i++)
    snake.push_back({snake_pos_x - i, snake_pos_y});
  CreateNewApple();
}

void GameSnake::CreateNewApple() {
  bool created = false;
  int x = 0, y = 0;
  while (!created) {
    x = rand() % BOARD_WIDTH;
    y = rand() % BOARD_HEIGHT;

    bool failed = false;
    for (size_t i = 0; i < snake.size() && !failed; ++i) {
      if (x == snake[i].x && y == snake[i].y) {
        failed = true;
      }
    }
    if (!failed) created = true;
  }
  apple.x = x;
  apple.y = y;
}

SnakeSignal GameSnake::GetSignalByAction(UserAction_t action) {
  SnakeSignal signal;
  switch (action) {
    case UserAction_t::Start:
      signal = SnakeSignal::START;
      break;
      ;
    case UserAction_t::Pause:
      signal = SnakeSignal::PAUSE;
      break;
      ;
    case UserAction_t::Terminate:
      signal = SnakeSignal::EXIT;
      break;
      ;
    case UserAction_t::Left:
      signal = SnakeSignal::LEFT;
      break;
      ;
    case UserAction_t::Right:
      signal = SnakeSignal::RIGHT;
      break;
      ;
    case UserAction_t::Up:
      signal = SnakeSignal::UP;
      break;
      ;
    case UserAction_t::Down:
      signal = SnakeSignal::DOWN;
      break;
      ;
    default:
      signal = SnakeSignal::NO_SIGNAL;
      break;
  }
  return signal;
}

void GameSnake::HandleSignalState(SnakeSignal signal) {
  switch (current_state) {
    case SnakeGameState::MENU:
      if (signal == SnakeSignal::START) {
        Init();
        InitBoard();
        current_state = SnakeGameState::PLAYING;
      } else if (signal == SnakeSignal::EXIT) {
        current_state = SnakeGameState::GAME_OVER;
        closeGame = true;
      }
      break;
    case SnakeGameState::PLAYING:
      switch (signal) {
        case SnakeSignal::PAUSE:
          current_state = SnakeGameState::PAUSE;
          break;
        case SnakeSignal::EXIT:
          current_state = SnakeGameState::GAME_OVER;
          closeGame = true;
          break;
        case SnakeSignal::UP:
        case SnakeSignal::DOWN:
        case SnakeSignal::RIGHT:
        case SnakeSignal::LEFT:
        case SnakeSignal::AUTO_MOVING:
          HandleMoving(signal);
        default:
          break;
      }
      break;
    case SnakeGameState::PAUSE:
      switch (signal) {
        case SnakeSignal::START:
        case SnakeSignal::PAUSE:
          current_state = SnakeGameState::PLAYING;
          break;
        case SnakeSignal::EXIT:
          current_state = SnakeGameState::GAME_OVER;
          closeGame = true;
          break;
        default:
          break;
      }
      break;
    case SnakeGameState::GAME_OVER:
      current_state = SnakeGameState::MENU;
      break;
    default:
      break;
  }
}

void GameSnake::HandleMoving(SnakeSignal signal) {
  // Prevent the snake from reversing direction
  if ((current_moving == Direction::UP && signal == SnakeSignal::DOWN) ||
      (current_moving == Direction::DOWN && signal == SnakeSignal::UP) ||
      (current_moving == Direction::LEFT && signal == SnakeSignal::RIGHT) ||
      (current_moving == Direction::RIGHT && signal == SnakeSignal::LEFT)) {
    return;
  }

  switch (signal) {
    case SnakeSignal::UP:
      current_moving = Direction::UP;
      break;
    case SnakeSignal::DOWN:
      current_moving = Direction::DOWN;
      break;
    case SnakeSignal::LEFT:
      current_moving = Direction::LEFT;
      break;
    case SnakeSignal::RIGHT:
      current_moving = Direction::RIGHT;
      break;
    default:
      break;
  }
  MoveSnake();
}

void GameSnake::MoveSnake() {
  // Move the snake one step forward
  Point head = snake.front();
  Point new_head;

  switch (current_moving) {
    case Direction::UP:
      new_head = {head.x, head.y - 1};
      break;
    case Direction::DOWN:
      new_head = {head.x, head.y + 1};
      break;
    case Direction::LEFT:
      new_head = {head.x - 1, head.y};
      break;
    case Direction::RIGHT:
      new_head = {head.x + 1, head.y};
      break;
    default:
      new_head = {head.x, head.y};
      break;
  }

  // Check for collisions
  if (new_head.x < 0 || new_head.x >= BOARD_WIDTH || new_head.y < 0 ||
      new_head.y >= BOARD_HEIGHT) {
    // wall -> game over
    current_state = SnakeGameState::GAME_OVER;
    return;
  }

  for (size_t i = 0; i < snake.size() - 1; ++i) {
    if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
      // self -> game over
      current_state = SnakeGameState::GAME_OVER;
      return;
    }
  }

  snake.push_front(new_head);

  // check if apple
  if (new_head.x == apple.x && new_head.y == apple.y) {
    score++;
    CheckHighScore();
    if (score >= level * LEVEL_INC) IncreaseLevel();

    CreateNewApple();
    if (snake.size() >= MAX_SNAKE_LENGTH) {
      win_game = true;
      current_state = SnakeGameState::GAME_OVER;
    }
  } else {
    // only moving -> remove tail
    snake.pop_back();
  }
}

GameInfo_t GameSnake::GetGameInfo() const {
  GameInfo_t info;
  info.score = score;
  info.high_score = highscore.score;
  info.level = level;
  info.speed = speed;
  info.pause = (current_state == SnakeGameState::PAUSE);
  info.over = (current_state == SnakeGameState::GAME_OVER);
  info.next = nullptr;

  info.field = new int*[BOARD_HEIGHT];
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    info.field[i] = new int[BOARD_WIDTH]();
  }

  if (snake.size() > 0) {
    info.field[apple.y][apple.x] = 1;
    for (size_t i = 0; i < snake.size(); i++)
      info.field[snake[i].y][snake[i].x] = 1;
  }

  return info;
}

vector<vector<BoardCell>> GameSnake::GenerateBoard() const {
  vector<vector<BoardCell>> board;

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    vector<BoardCell> board_row;
    for (int j = 0; j < BOARD_WIDTH; j++) {
      BoardCell cell;
      cell.x = j;
      cell.y = i;
      cell.type = CellType::Empty;

      board_row.push_back(cell);
    }
    board.push_back(board_row);
  }

  // if initialized
  if (snake.size() > 0) {
    board[apple.y][apple.x].type = CellType::Apple;

    for (size_t i = 0; i < snake.size(); i++)
      board[snake[i].y][snake[i].x].type = CellType::Snake;
  }

  return board;
}

HighScore GameSnake::GetHighScore() const { return highscore; }

SnakeGameState GameSnake::GetState() const { return current_state; }

void GameSnake::SetState(SnakeGameState new_state) {
  current_state = new_state;
}

void GameSnake::IncreaseLevel() {
  if (level < MAX_LEVEL) {
    level++;
    speed++;
  }
}

void GameSnake::CheckHighScore() {
  if (score > highscore.score) {
    new_highscore = true;
    highscore.score = score;
    highscore.name = "new highscore";
  }
}

void GameSnake::ReadHighScore() {
  ifstream file(SNAKE_HIGHSCORE_FILE, ios::binary | ios::in);

  if (file.is_open()) {
    HighScore hs;
    file.read(reinterpret_cast<char*>(&hs.score), sizeof(hs.score));
    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

    char* nameBuffer = new char[nameLength + 1];
    memset(nameBuffer, 0, nameLength + 1);
    file.read(nameBuffer, nameLength);
    nameBuffer[nameLength] = '\0';
    hs.name = nameBuffer;
    delete[] nameBuffer;

    highscore.score = hs.score;
    highscore.name = hs.name;

  } else {
    highscore.score = 0;
    highscore.name = "";
  }
}

void GameSnake::WriteHighScore() {
  ofstream file(SNAKE_HIGHSCORE_FILE, ios::out | ios::trunc | ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char*>(&highscore.score),
               sizeof(highscore.score));
    size_t nameLength = highscore.name.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(highscore.name.c_str(), nameLength);

    file.close();
  }
}

void GameSnake::SaveNewHighScore(string player_name) {
  highscore.name = player_name;
  WriteHighScore();
  new_highscore = false;
}

bool GameSnake::NewHighScore() { return new_highscore; }

bool GameSnake::WinGame() { return win_game; }

int GameSnake::GetLevel() { return level; }

int GameSnake::GetScore() { return score; }
}  // namespace s21