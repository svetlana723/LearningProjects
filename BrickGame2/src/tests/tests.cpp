#include <gtest/gtest.h>

#include <cstdio>
#include <deque>
#include <string>
#include <vector>

#include "../brick_game/snake/snake_model.h"

using namespace s21;
using namespace std;

class GameSnakeTest : public ::testing::Test {
 protected:
  void SetUp() override {
    game = new GameSnake();
    game->Init();
  }

  void TearDown() override { delete game; }

  void ForceGameState(SnakeGameState state) { game->SetState(state); }

  GameSnake* game;
};

TEST_F(GameSnakeTest, InitialState) {
  EXPECT_EQ(game->GetState(), SnakeGameState::MENU);
  EXPECT_FALSE(game->NewHighScore());
  s21::GameInfo_t game_info = game->GetGameInfo();
  EXPECT_EQ(game_info.score, 0);
  EXPECT_EQ(game_info.level, 1);
  EXPECT_EQ(game_info.speed, 1);
  EXPECT_EQ(game_info.pause, 0);
  EXPECT_EQ(game_info.over, 0);
  freeGameInfo(game_info);
}

TEST_F(GameSnakeTest, Initialize) {
  initialize();
  EXPECT_NE(game_ptr, nullptr);
  s21::GameInfo_t game_info = game_ptr->GetGameInfo();
  EXPECT_EQ(game_info.score, 0);
  EXPECT_EQ(game_info.level, 1);
  EXPECT_EQ(game_info.speed, 1);
  EXPECT_EQ(game_info.pause, 0);
  EXPECT_EQ(game_info.over, 0);
  freeGameInfo(game_info);
}

TEST_F(GameSnakeTest, StateTransitions) {
  // MENU -> PLAYING
  game->HandleSignalState(SnakeSignal::START);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  // PLAYING -> PLAYING
  game->HandleSignalState(SnakeSignal::DOWN);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  // PLAYING -> PLAYING
  game->HandleSignalState(SnakeSignal::RIGHT);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  // PLAYING -> PAUSE
  game->HandleSignalState(SnakeSignal::PAUSE);
  EXPECT_EQ(game->GetState(), SnakeGameState::PAUSE);

  // PAUSE -> PAUSE
  game->HandleSignalState(SnakeSignal::DOWN);
  EXPECT_EQ(game->GetState(), SnakeGameState::PAUSE);

  // PAUSE -> PLAYING
  game->HandleSignalState(SnakeSignal::PAUSE);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  // PLAYING -> GAME_OVER
  game->HandleSignalState(SnakeSignal::EXIT);
  EXPECT_EQ(game->GetState(), SnakeGameState::GAME_OVER);

  game->SetState(SnakeGameState::GAME_OVER);
  game->HandleSignalState(SnakeSignal::AUTO_MOVING);
  EXPECT_EQ(game->GetState(), SnakeGameState::MENU);
}

TEST_F(GameSnakeTest, SignalHandlingInMenu) {
  game->HandleSignalState(SnakeSignal::START);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  game->SetState(SnakeGameState::MENU);

  game->HandleSignalState(SnakeSignal::EXIT);
  EXPECT_EQ(game->GetState(), SnakeGameState::GAME_OVER);
}

TEST_F(GameSnakeTest, SignalHandlingInPlaying) {
  game->HandleSignalState(SnakeSignal::START);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  game->HandleSignalState(SnakeSignal::RIGHT);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  game->HandleSignalState(SnakeSignal::DOWN);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  game->HandleSignalState(SnakeSignal::PAUSE);
  EXPECT_EQ(game->GetState(), SnakeGameState::PAUSE);
}

TEST_F(GameSnakeTest, SignalHandlingInPause) {
  game->SetState(SnakeGameState::PAUSE);

  game->HandleSignalState(SnakeSignal::DOWN);
  EXPECT_EQ(game->GetState(), SnakeGameState::PAUSE);

  game->HandleSignalState(SnakeSignal::START);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  game->SetState(SnakeGameState::PAUSE);
  game->HandleSignalState(SnakeSignal::PAUSE);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);
}

TEST_F(GameSnakeTest, MovementDirections) {
  game->HandleSignalState(SnakeSignal::START);

  game->HandleSignalState(SnakeSignal::RIGHT);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  game->HandleSignalState(SnakeSignal::DOWN);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  game->HandleSignalState(SnakeSignal::LEFT);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);

  game->HandleSignalState(SnakeSignal::UP);
  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);
}

TEST_F(GameSnakeTest, PreventReverseMovement) {
  game->HandleSignalState(SnakeSignal::START);

  game->HandleSignalState(SnakeSignal::RIGHT);
  Point head = game->GetSnake()[0];
  game->HandleSignalState(SnakeSignal::LEFT);
  Point new_head = game->GetSnake()[0];
  EXPECT_EQ(head.x, new_head.x);
  EXPECT_EQ(head.y, new_head.y);
}

TEST_F(GameSnakeTest, WallCollision) {
  game->HandleSignalState(SnakeSignal::START);

  for (int i = 0; i < BOARD_WIDTH; i++) {
    if (game->GetState() == SnakeGameState::GAME_OVER) break;
    game->HandleSignalState(SnakeSignal::RIGHT);
  }

  EXPECT_EQ(game->GetState(), SnakeGameState::GAME_OVER);
}

TEST_F(GameSnakeTest, SelfCollision) {
  game->HandleSignalState(SnakeSignal::START);

  Point head = game->GetSnake()[0];

  game->SetApple(head.x, head.y + 1);
  game->HandleSignalState(SnakeSignal::DOWN);

  GameInfo_t game_info = game->GetGameInfo();
  EXPECT_EQ(game_info.score, 1);
  freeGameInfo(game_info);

  game->HandleSignalState(SnakeSignal::LEFT);
  game->HandleSignalState(SnakeSignal::UP);

  EXPECT_EQ(game->GetState(), SnakeGameState::GAME_OVER);
}

TEST_F(GameSnakeTest, UserInput) {
  userInput(s21::UserAction_t::Start);
  EXPECT_NE(game_ptr, nullptr);
  s21::GameInfo_t game_info = game_ptr->GetGameInfo();
  EXPECT_EQ(game_info.score, 0);
  EXPECT_EQ(game_info.level, 1);
  EXPECT_EQ(game_info.speed, 1);
  EXPECT_EQ(game_info.pause, 0);
  EXPECT_EQ(game_info.over, 0);

  userInput(s21::UserAction_t::Left, false);
  userInput(s21::UserAction_t::Up, false);
  userInput(s21::UserAction_t::Down, false);
  userInput(s21::UserAction_t::Right, false);
  userInput(s21::UserAction_t::Pause, false);
  userInput(s21::UserAction_t::Terminate, false);

  freeGameInfo(game_info);
}

TEST_F(GameSnakeTest, UpdateCurrentState) {
  s21::GameInfo_t game_info = updateCurrentState();
  EXPECT_EQ(game_info.score, 0);
  EXPECT_EQ(game_info.level, 1);
  EXPECT_EQ(game_info.speed, 1);
  EXPECT_EQ(game_info.pause, 0);
  freeGameInfo(game_info);
}

TEST_F(GameSnakeTest, GameInfoStructure) {
  s21::GameInfo_t info = game->GetGameInfo();

  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.speed, 1);
  EXPECT_FALSE(info.pause);
  EXPECT_FALSE(info.over);
  EXPECT_NE(info.field, nullptr);

  EXPECT_NE(info.field[0], nullptr);

  freeGameInfo(info);
}

TEST_F(GameSnakeTest, LevelProgression) {
  game->HandleSignalState(SnakeSignal::START);

  s21::GameInfo_t game_info = game->GetGameInfo();
  EXPECT_EQ(game_info.level, 1);
  EXPECT_EQ(game_info.score, 0);
  freeGameInfo(game_info);

  Point head = game->GetSnake()[0];

  for (int i = 0; i < 5; i++) {
    game->SetApple(head.x, head.y + i + 1);
    game->HandleSignalState(SnakeSignal::DOWN);
  }

  EXPECT_EQ(game->GetState(), SnakeGameState::PLAYING);
  game_info = game->GetGameInfo();
  EXPECT_EQ(game_info.score, 5);
  EXPECT_EQ(game_info.level, 2);
  freeGameInfo(game_info);
}

TEST_F(GameSnakeTest, HighScoreWriting) {
  game->HandleSignalState(SnakeSignal::START);

  s21::GameInfo_t info = game->GetGameInfo();
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.score, 0);

  Point head = game->GetSnake()[0];

  for (int i = 0; i < 5; i++) {
    game->SetApple(head.x, head.y + i + 1);
    game->HandleSignalState(SnakeSignal::DOWN);
  }

  game->SaveNewHighScore("test");

  EXPECT_FALSE(game->NewHighScore());

  HighScore hs = game->GetHighScore();
  EXPECT_EQ(hs.score, 5);
  EXPECT_EQ(hs.name, "test");

  freeGameInfo(info);
}

TEST_F(GameSnakeTest, WinGame) {
  game->HandleSignalState(SnakeSignal::START);

  EXPECT_FALSE(game->WinGame());
}

TEST_F(GameSnakeTest, GetScore) {
  game->HandleSignalState(SnakeSignal::START);

  EXPECT_EQ(game->GetScore(), 0);
}

TEST_F(GameSnakeTest, GetLevel) {
  game->HandleSignalState(SnakeSignal::START);

  EXPECT_EQ(game->GetLevel(), 1);
}

TEST_F(GameSnakeTest, GenerateBoard) {
  game->HandleSignalState(SnakeSignal::START);

  vector<vector<BoardCell>> board = game->GenerateBoard();

  EXPECT_EQ(board.size(), BOARD_HEIGHT);
  EXPECT_EQ(board[0].size(), BOARD_WIDTH);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  cleanup();
  return result;
}