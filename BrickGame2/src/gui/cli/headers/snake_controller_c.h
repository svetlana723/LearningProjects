#ifndef SNAKE_CONTROLLER_C_H
#define SNAKE_CONTROLLER_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ncurses.h>
#include <stdbool.h>

#include "../../../brick_game/common.h"
#include "../../../brick_game/tetris/headers/defines.h"
#include "frontend.h"

// ===== Функции контроллера =====
void initialize_controller();
void handle_input(int key);
void cleanup_controller();

void game_loop();
int set_pause_on(WINDOW** message_window);
int set_pause_off(WINDOW* message_window, int pause_time);
int set_game_over_on(WINDOW** message_window);
int set_game_over_off(WINDOW* message_window);
void set_auto_moving_on(int pause_time);
void set_auto_moving_off();
void update_speed(GameInfo_t* info, int* pause_time);

// ===== Функции C-интерфейса =====
void snake_initialize();
void snake_user_input(UserAction_t action, bool hold);
GameInfo_t snake_update_current_state();
void snake_free_gameinfo(GameInfo_t* info);
void snake_cleanup();

#ifdef __cplusplus
}
#endif

#endif  // SNAKE_CONTROLLER_C_H