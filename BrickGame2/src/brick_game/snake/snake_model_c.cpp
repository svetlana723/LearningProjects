#include "snake_model_c.h"

#include "snake_model.h"

extern "C" {

static s21::GameSnake* game_instance = nullptr;

void snake_initialize() {
  if (game_instance == nullptr) {
    game_instance = new s21::GameSnake();
  }
}

void snake_user_input(UserAction_t action, bool hold) {
  (void)hold;
  if (game_instance) {
    s21::SnakeSignal signal;
    switch (action) {
      case Start:
        signal = s21::SnakeSignal::START;
        break;
      case Pause:
        signal = s21::SnakeSignal::PAUSE;
        break;
      case Terminate:
        signal = s21::SnakeSignal::EXIT;
        break;
      case Left:
        signal = s21::SnakeSignal::LEFT;
        break;
      case Right:
        signal = s21::SnakeSignal::RIGHT;
        break;
      case Up:
        signal = s21::SnakeSignal::UP;
        break;
      case Down:
        signal = s21::SnakeSignal::DOWN;
        break;
      case Action:
        signal = s21::SnakeSignal::AUTO_MOVING;
        break;
      default:
        signal = s21::SnakeSignal::NO_SIGNAL;
        break;
    }
    game_instance->HandleSignalState(signal);
  }
}

GameInfo_t snake_update_current_state() {
  if (game_instance) {
    return game_instance->GetGameInfo();
  }
  return create_empty_gameinfo();
}

GameInfo_t create_empty_gameinfo() {
  GameInfo_t info;
  info.field = nullptr;
  info.next = nullptr;
  info.score = 0;
  info.high_score = 0;
  info.level = 1;
  info.speed = 1;
  info.pause = 0;
  return info;
}

void snake_free_gameinfo(GameInfo_t* info) {
  if (info && info->field) {
    for (int i = 0; i < 20; i++) {
      free(info->field[i]);
    }
    free(info->field);
    info->field = nullptr;
  }
}

void snake_cleanup() {
  if (game_instance) {
    delete game_instance;
    game_instance = nullptr;
  }
}

int snake_get_score() { return game_instance ? game_instance->GetScore() : 0; }

int snake_get_level() { return game_instance ? game_instance->GetLevel() : 1; }

bool snake_is_paused() {
  return game_instance
             ? (game_instance->GetState() == s21::SnakeGameState::PAUSE)
             : true;
}

}  // extern "C"