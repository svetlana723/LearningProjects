#include "./headers/figure.h"

figure_type get_next_figure_type() {
  int int_figure_type = rand() % 7 + 1;
  figure_type next_figure_type = (figure_type)int_figure_type;
  return next_figure_type;
}

void init_figure(figure_t* figure, figure_type type) {
  figure->x = FIGURE_START_X;
  figure->y = FIGURE_START_Y;
  figure->type = type;
  figure->rotate = ROTATE_0;

  set_figure_shape(figure);
}

rotate_step get_next_rotate(const figure_t* figure) {
  return ((rotate_step)(figure->rotate + 1) % 4);
}

void set_figure_shape(figure_t* figure) {
  for (int i = 0; i < FIGURE_MAX_WIDTH; i++)
    for (int j = 0; j < FIGURE_MAX_HEIGHT; j++) figure->shape[j][i] = 0;

  switch (figure->type) {
    case FIG_1:
      set_shape_figure_1(figure);
      break;
    case FIG_2:
      set_shape_figure_2(figure);
      break;
    case FIG_3:
      set_shape_figure_3(figure);
      break;
    case FIG_4:
      set_shape_figure_4(figure);
      break;
    case FIG_5:
      set_shape_figure_5(figure);
      break;
    case FIG_6:
      set_shape_figure_6(figure);
      break;
    case FIG_7:
      set_shape_figure_7(figure);
      break;

    default:
      break;
  }
}

int left_offset(const figure_t* figure) {
  int offset = 0;
  switch (figure->type) {
    case FIG_1:
      if (figure->rotate == ROTATE_0)
        offset = 1;
      else if (figure->rotate == ROTATE_2)
        offset = 2;
      break;
    case FIG_2:
    case FIG_3:
    case FIG_6:
      if (figure->rotate != ROTATE_1) offset = 1;
      break;
    case FIG_5:
    case FIG_7:
      if (figure->rotate != ROTATE_3) offset = 1;
      break;
    default:
      offset = 0;
      break;
  }
  return offset;
}

int right_offset(const figure_t* figure) {
  int offset = 0;
  switch (figure->type) {
    case FIG_1:
      if (figure->rotate == ROTATE_0)
        offset = 2;
      else if (figure->rotate == ROTATE_2)
        offset = 1;
      break;
    case FIG_2:
    case FIG_3:
    case FIG_6:
      if (figure->rotate != ROTATE_3) offset = 1;
      break;
    case FIG_4:
      offset = 1;
      break;
    case FIG_5:
    case FIG_7:
      if (figure->rotate != ROTATE_1) offset = 1;
      break;
    default:
      offset = 0;
      break;
  }
  return offset;
}

int down_offset(const figure_t* figure) {
  int offset = 0;
  switch (figure->type) {
    case FIG_1:
      if (figure->rotate == ROTATE_1)
        offset = 2;
      else if (figure->rotate == ROTATE_3)
        offset = 1;
      break;
    case FIG_2:
    case FIG_3:
    case FIG_6:
      if (figure->rotate != ROTATE_0) offset = 1;
      break;
    case FIG_4:
      offset = 1;
      break;
    case FIG_5:
    case FIG_7:
      if (figure->rotate != ROTATE_2) offset = 1;
      break;
    default:
      offset = 0;
      break;
  }
  return offset;
}

void set_shape_figure_1(figure_t* figure) {
  switch (figure->rotate) {
    case ROTATE_1:
      figure->shape[0][0] = 1;
      figure->shape[1][0] = 1;
      figure->shape[2][0] = 1;
      figure->shape[3][0] = 1;
      figure->centre_x = 0;
      figure->centre_y = 1;
      break;
    case ROTATE_2:
      figure->shape[0][0] = 1;
      figure->shape[0][1] = 1;
      figure->shape[0][2] = 1;
      figure->shape[0][3] = 1;
      figure->centre_x = 2;
      figure->centre_y = 0;
      break;
    case ROTATE_3:
      figure->shape[0][0] = 1;
      figure->shape[1][0] = 1;
      figure->shape[2][0] = 1;
      figure->shape[3][0] = 1;
      figure->centre_x = 0;
      figure->centre_y = 2;
      break;
    default:
      figure->shape[0][0] = 1;
      figure->shape[0][1] = 1;
      figure->shape[0][2] = 1;
      figure->shape[0][3] = 1;
      figure->centre_x = 1;
      figure->centre_y = 0;
      break;
  }
}

void set_shape_figure_2(figure_t* figure) {
  switch (figure->rotate) {
    case ROTATE_1:
      figure->shape[0][0] = 1;
      figure->shape[1][0] = 1;
      figure->shape[2][0] = 1;
      figure->shape[0][1] = 1;
      figure->centre_x = 0;
      figure->centre_y = 1;
      break;
    case ROTATE_2:
      figure->shape[0][0] = 1;
      figure->shape[0][1] = 1;
      figure->shape[0][2] = 1;
      figure->shape[1][2] = 1;
      figure->centre_x = 1;
      figure->centre_y = 0;
      break;
    case ROTATE_3:
      figure->shape[0][1] = 1;
      figure->shape[1][1] = 1;
      figure->shape[2][1] = 1;
      figure->shape[2][0] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
    default:
      figure->shape[0][0] = 1;
      figure->shape[1][0] = 1;
      figure->shape[1][1] = 1;
      figure->shape[1][2] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
  }
}

void set_shape_figure_3(figure_t* figure) {
  switch (figure->rotate) {
    case ROTATE_1:
      figure->shape[0][0] = 1;
      figure->shape[1][0] = 1;
      figure->shape[2][0] = 1;
      figure->shape[2][1] = 1;
      figure->centre_x = 0;
      figure->centre_y = 1;
      break;
    case ROTATE_2:
      figure->shape[0][0] = 1;
      figure->shape[0][1] = 1;
      figure->shape[0][2] = 1;
      figure->shape[1][0] = 1;
      figure->centre_x = 1;
      figure->centre_y = 0;
      break;
    case ROTATE_3:
      figure->shape[0][1] = 1;
      figure->shape[1][1] = 1;
      figure->shape[2][1] = 1;
      figure->shape[0][0] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
    default:
      figure->shape[1][0] = 1;
      figure->shape[1][1] = 1;
      figure->shape[1][2] = 1;
      figure->shape[0][2] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
  }
}

void set_shape_figure_4(figure_t* figure) {
  figure->shape[0][0] = 1;
  figure->shape[1][0] = 1;
  figure->shape[0][1] = 1;
  figure->shape[1][1] = 1;
  figure->centre_x = 0;
  figure->centre_y = 0;
}

void set_shape_figure_5(figure_t* figure) {
  switch (figure->rotate) {
    case ROTATE_1:
      figure->shape[0][0] = 1;
      figure->shape[1][0] = 1;
      figure->shape[1][1] = 1;
      figure->shape[2][1] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
    case ROTATE_2:
      figure->shape[0][1] = 1;
      figure->shape[1][0] = 1;
      figure->shape[1][1] = 1;
      figure->shape[0][2] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
    case ROTATE_3:
      figure->shape[0][0] = 1;
      figure->shape[1][0] = 1;
      figure->shape[1][1] = 1;
      figure->shape[2][1] = 1;
      figure->centre_x = 0;
      figure->centre_y = 1;
      break;
    default:
      figure->shape[0][1] = 1;
      figure->shape[1][0] = 1;
      figure->shape[1][1] = 1;
      figure->shape[0][2] = 1;
      figure->centre_x = 1;
      figure->centre_y = 0;
      break;
  }
}

void set_shape_figure_6(figure_t* figure) {
  switch (figure->rotate) {
    case ROTATE_1:
      figure->shape[0][0] = 1;
      figure->shape[1][0] = 1;
      figure->shape[2][0] = 1;
      figure->shape[1][1] = 1;
      figure->centre_x = 0;
      figure->centre_y = 1;
      break;
    case ROTATE_2:
      figure->shape[0][0] = 1;
      figure->shape[0][1] = 1;
      figure->shape[0][2] = 1;
      figure->shape[1][1] = 1;
      figure->centre_x = 1;
      figure->centre_y = 0;
      break;
    case ROTATE_3:
      figure->shape[0][1] = 1;
      figure->shape[1][1] = 1;
      figure->shape[2][1] = 1;
      figure->shape[1][0] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
    default:
      figure->shape[1][0] = 1;
      figure->shape[0][1] = 1;
      figure->shape[1][1] = 1;
      figure->shape[1][2] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
  }
}

void set_shape_figure_7(figure_t* figure) {
  switch (figure->rotate) {
    case ROTATE_1:
      figure->shape[1][0] = 1;
      figure->shape[1][1] = 1;
      figure->shape[0][1] = 1;
      figure->shape[2][0] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
    case ROTATE_2:
      figure->shape[0][0] = 1;
      figure->shape[0][1] = 1;
      figure->shape[1][1] = 1;
      figure->shape[1][2] = 1;
      figure->centre_x = 1;
      figure->centre_y = 1;
      break;
    case ROTATE_3:
      figure->shape[1][0] = 1;
      figure->shape[1][1] = 1;
      figure->shape[0][1] = 1;
      figure->shape[2][0] = 1;
      figure->centre_x = 0;
      figure->centre_y = 1;
      break;
    default:
      figure->shape[0][0] = 1;
      figure->shape[0][1] = 1;
      figure->shape[1][1] = 1;
      figure->shape[1][2] = 1;
      figure->centre_x = 1;
      figure->centre_y = 0;
      break;
  }
}
