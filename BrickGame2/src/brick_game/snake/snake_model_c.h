#ifndef C_INTERFACE_H
#define C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../common.h"

// C
void snake_initialize();
void snake_user_input(UserAction_t action, bool hold);
GameInfo_t snake_update_current_state();
GameInfo_t create_empty_gameinfo();
void snake_free_gameinfo(GameInfo_t* info);
void snake_cleanup();

int snake_get_score();
int snake_get_level();
bool snake_is_paused();

#ifdef __cplusplus
}
#endif

#endif  // C_INTERFACE_H