#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../common.h"
#include "board.h"
#include "defines.h"
#include "figure.h"
#include "objects.h"

#ifdef __cplusplus
extern "C" {
#endif

void initialize();
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
void freeGameInfo(GameInfo_t* info);
void cleanup();

#ifdef __cplusplus
}
#endif

void init_state(game_state_t* state);
void process_state(game_state_t* game_state, TetrisSignal signal);
void process_figure_moving(game_state_t* game_state, TetrisSignal signal,
                           states* new_state);
char* get_state_name(states state);
char* get_signal_name(TetrisSignal signal);
TetrisSignal get_signal_by_action(UserAction_t action);
board_t get_board_to_print(const game_state_t* game_state);
GameInfo_t get_game_info(const game_state_t* game_state);
game_state_t* get_game_state();
#endif