#include <stdio.h>
#include <stdlib.h>
#define EMPTY      '-'
#define PLACE_ABLE '*'
#define WHITE      'O'
#define BLACK      'X'
#define GRID        8


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
coord *input_coord(coord *);
int set_place_able(stone);
void reset_place_able(void);
void coord_copy(coord *, coord *);
char to_upper(char);
int is_in_board(coord *);
int is_empty_square(coord *);
int is_place_able_square(coord *);
int calc_point(stone);
int calc_distance(coord *, coord *);
direction calc_direction(coord *, coord *);
stone get_opposite_stone(stone);
int get_x_incremental_by_direction(direction);
int get_y_incremental_by_direction(direction);
coord *get_pair_coord(coord *, coord *, direction, stone);
void put_stone(coord *, stone);
void flip_stone(coord *, coord *);


stone board[GRID][GRID];
coord place_able_squares[GRID * GRID];


int main(void) {
    play();
    return 0;
}

void play(void) {
    coord pos, pair;
    direction dir;
    stone color = BLACK;
    int pnt_b, pnt_w, pass_flag = 0, stone_cnt = 4;

    init_board();

    while (stone_cnt < GRID * GRID) {
        if (!set_place_able(color)) {
            if (pass_flag) {
                break;
            }
            pass_flag = 1;
            printf("Pass.\n");
        } else {
            printf("\n");
            print_board();
            printf("\n");
            do {
                printf("Where do you place the stone '%c' ?\n", color);
                printf(" >> ");
            } while (input_coord(&pos) == NULL || !is_place_able_square(&pos));

            for (dir = TOP_LEFT; dir <= LEFT; dir++) {
                if (get_pair_coord(&pair, &pos, dir, color) != NULL) {
                    put_stone(&pos, color);
                    flip_stone(&pos, &pair);
                }
            }
            pass_flag = 0;
            stone_cnt++;
        }
        reset_place_able();
        color = get_opposite_stone(color);
    }
    printf("\n");
    print_board();

    pnt_b = calc_point(BLACK);
    pnt_w = calc_point(WHITE);
    printf("BLACK(%c) : %d\n", BLACK, pnt_b);
    printf("WHITE(%c) : %d\n", WHITE, pnt_w);
}

void init_board(void) {
    int x, y;

    for (y = 0; y < GRID; y++) {
        for (x = 0; x < GRID; x++) {
            board[y][x] = EMPTY;
        }
    }
    board[3][3] = WHITE;
    board[4][4] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
}

void print_board(void) {
    int x, y;

    printf("   ");
    for (x = 0; x < GRID; x++) {
        printf(" %c ", 'A' + x);
    }
    printf("\n");
    for (y = 0; y < GRID; y++) {
        printf(" %d ", y + 1);
        for (x = 0; x < GRID; x++) {
            printf(" %c ", board[y][x]);
        }
        printf("\n");
    }
}

coord *input_coord(coord *dst) {
    char s[4];

    if (fgets(s, 4, stdin) == NULL) {
        return NULL;
    }
    s[1] = to_upper(s[1]);

    if (('1' <= s[0] && s[0] <= '8') && ('A' <= s[1] && s[1] <= 'H')) {
        dst->x = s[1] - 'A';
        dst->y = s[0] - '0' - 1;
        return dst;
    } else {
        return NULL;
    }
}

int set_place_able(stone color) {
    int replaced, x, y;
    coord pos, tmp;
    direction dir;

    replaced = 0;
    for (y = 0; y < GRID; y++) {
        for (x = 0; x < GRID; x++) {
            pos.x = x;
            pos.y = y;
            if (!is_empty_square(&pos)) {
                continue;
            }
            for (dir = TOP_LEFT; dir <= LEFT; dir++) {
                if (get_pair_coord(&tmp, &pos, dir, color) != NULL) {
                    board[pos.y][pos.x] = PLACE_ABLE;
                    replaced = 1;
                    break;
                }
            }
        }
    }

    return replaced;
}

void reset_place_able(void) {
    int x, y;
    coord pos;

    for (y = 0; y < GRID; y++) {
        for (x = 0; x < GRID; x++) {
            pos.x = x;
            pos.y = y;
            if (is_place_able_square(&pos)) {
                board[pos.y][pos.x] = EMPTY;
            }
        }
    }    
}

void coord_copy(coord *dst, coord *src) {
    dst->x = src->x;
    dst->y = src->y;
}

char to_upper(char c) {
    return ('a' <= c && c <= 'z') ? c - ('a' - 'A') : c;
}

int is_in_board(coord *pos) {
    return (0 <= pos->x && pos->x < GRID) && (0 <= pos->y && pos->y < GRID);
}

int is_empty_square(coord *pos) {
    return board[pos->y][pos->x] == EMPTY || board[pos->y][pos->x] == PLACE_ABLE;
}

int is_place_able_square(coord *pos) {
    return board[pos->y][pos->x] == PLACE_ABLE;
}

int calc_point(stone color) {
    int x, y, point = 0;

    for (y = 0; y < GRID; y++) {
        for (x = 0; x < GRID; x++) {
            if (board[y][x] == color) {
                point++;
            }
        }
    }

    return point;
}

int calc_distance(coord *from, coord *to) {
    int dis_x, dis_y;

    dis_x = abs(to->x - from->x) - 1;
    dis_y = abs(to->y - from->y) - 1;

    return (dis_x > dis_y) ? dis_x : dis_y;
}

direction calc_direction(coord *from, coord *to) {
    int x, y;
    x = to->x - from->x;
    y = to->y - from->y;

    if (x < 0 && y < 0) {
        return TOP_LEFT;
    } else if (x == 0 && y < 0) {
        return TOP;
    } else if (x > 0 && y < 0) {
        return TOP_RIGHT;
    } else if (x > 0 && y == 0) {
        return RIGHT;
    } else if (x > 0 && y > 0) {
        return BOTTOM_RIGHT;
    } else if (x == 0 && y > 0) {
        return BOTTOM;
    } else if (x < 0 && y > 0) {
        return BOTTOM_LEFT;
    } else {
        return LEFT;
    }
}

stone get_opposite_stone(stone color) {
    return (color == WHITE) ? BLACK : WHITE;
}

int get_x_incremental_by_direction(direction dir) {
    if (2 <= dir && dir <= 4) {
        return 1;
    } else if (dir == 6 || dir == 7 || dir == 0) {
        return -1;
    } else {
        return 0;
    }
}

int get_y_incremental_by_direction(direction dir) {
    if (0 <= dir && dir <= 2) {
        return -1;
    } else if (4 <= dir && dir <= 6) {
        return 1;
    } else {
        return 0;
    }
}

coord *get_pair_coord(coord *dst, coord *pos, direction dir, stone color) {
    int x_inc, y_inc, cnt = 0;

    coord_copy(dst, pos);
    x_inc = get_x_incremental_by_direction(dir);
    y_inc = get_y_incremental_by_direction(dir);

    while (1) {
        dst->x += x_inc;
        dst->y += y_inc;
        if (!is_in_board(dst) || is_empty_square(dst)) {
            return NULL;
        }
        if (board[dst->y][dst->x] == color) {
            if (cnt == 0) {
                return NULL;
            } else {
                return dst;
            }
        }
        cnt++;
    }
}

void put_stone(coord *pos, stone color) {
    board[pos->y][pos->x] = color;
}

void flip_stone(coord *from, coord *to) {
    int dis, x_inc, y_inc;
    direction dir;
    coord current;
    stone color;

    coord_copy(&current, from);
    dis = calc_distance(from, to);
    dir = calc_direction(from, to);
    x_inc = get_x_incremental_by_direction(dir);
    y_inc = get_y_incremental_by_direction(dir);
    color = board[from->y][from->x];

    while (dis--) {
        current.x += x_inc;
        current.y += y_inc;
        board[current.y][current.x] = color;
    }
}