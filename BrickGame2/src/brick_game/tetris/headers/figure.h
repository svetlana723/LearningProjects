#ifndef FIGURE_H
#define FIGURE_H

#include <stdlib.h>

#include "defines.h"
#include "objects.h"

void init_figure(figure_t* figure, figure_type type);
figure_type get_next_figure_type();
void set_figure_shape(figure_t* figure);
void set_shape_figure_1(figure_t* figure);
void set_shape_figure_2(figure_t* figure);
void set_shape_figure_3(figure_t* figure);
void set_shape_figure_4(figure_t* figure);
void set_shape_figure_5(figure_t* figure);
void set_shape_figure_6(figure_t* figure);
void set_shape_figure_7(figure_t* figure);
int right_offset(const figure_t* figure);
int left_offset(const figure_t* figure);
int down_offset(const figure_t* figure);
rotate_step get_next_rotate(const figure_t* figure);

#endif