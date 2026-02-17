#ifndef DEFINES_H
#define DEFINES_H

#define KEY_ESCAPE 27

#define START_MESSAGE "Press 'S' to start!"
#define PAUSE_MESSAGE "Press 'P' to pause."
#define EXIT_MESSAGE "Press 'Q' to exit."

#define PAUSE_TIME 11
#define MAX_LEVEL 10

#define STAT_WIDTH 22
#define GAME_BOX_WIDTH 36
#define GAME_BOX_HEIGHT 22
#define MESSAGE_WINDOW_HEIGHT 6
#define MESSAGE_WINDOW_WIDTH 30

#define FIGURE_MAX_HEIGHT 4
#define FIGURE_MAX_WIDTH 4
#define FIGURE_START_X (BOARD_WIDTH / 2)
#define FIGURE_START_Y (1)

#define CHAR_EMPTY ' '
#define CHAR_FILLED '%'
#define CHAR_FIGURE '%'

#define SCORE_1_LINE 100
#define SCORE_2_LINE 300
#define SCORE_3_LINE 700
#define SCORE_4_LINE 1500

#define LEVEL_SCORE 600

#define TETRIS_HIGHSCORE_FILE "./build/tetris_high_scores"

#endif