#ifndef OTHELLO_H
#define OTHELLO_H


#define EMPTY      '-'
#define PLACE_ABLE '*'
#define WHITE      'O'
#define BLACK      'X'
#define SIDE_LEN   8

typedef struct {
    int x;
    int y;
} coord;

typedef enum {
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT,
    BOTTOM,
    BOTTOM_LEFT,
    LEFT
} direction;

typedef char stone;

void play(void);
void init_board(void);
void print_board(void);
int get_coord(coord *);
int calc_point(stone );
void put_stone(coord *, stone);
void flip_stone(coord *, coord *);
int set_place_able(stone);
void reset_place_able(void);
int has_pair_stone(coord *, direction, stone, coord *);
stone get_opposite_stone(stone);
int get_distance(coord *, coord *);
direction get_direction(coord *, coord *);
int get_x_displacement(direction);
int get_y_displacement(direction);
int is_empty_square(coord *);
int is_place_able_square(coord *);
int is_in_board(coord *);
char to_upper(char);
void coord_copy(coord *, coord *);


#endif  // OTHELLO_H