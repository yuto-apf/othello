#include <stdio.h>
#include <stdlib.h>
#define EMPTY '-'
#define WHITE 'O'
#define BLACK 'X'
#define GRID   8


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


void init_board(void);
void print_board(void);
void coord_cpy(coord *, coord *);
int is_in_board(coord *);
int is_empty_square(coord *);
int calc_distance(coord *, coord *);
direction calc_direction(coord *, coord *);
stone get_opposite_stone(stone);
int get_x_incremental_by_direction(direction);
int get_y_incremental_by_direction(direction);
coord *get_pair_coord(coord *, coord *, direction, stone);
void put_stone(coord *, stone);
void flip_stone(coord *, coord *);
void test(void);


stone board[GRID][GRID];


int main(void) {
    init_board();

    test();

    return 0;
}

void init_board(void) {
    int i, j;

    for (i = 0; i < GRID; i++) {
        for (j = 0; j < GRID; j++) {
            board[i][j] = EMPTY;
        }
    }
    board[3][3] = WHITE;
    board[4][4] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
}

void print_board(void) {
    int i, j;

    printf("   ");
    for (i = 0; i < GRID; i++) {
        printf(" %c ", 'A' + i);
    }
    printf("\n");
    for (i = 0; i < GRID; i++) {
        printf(" %d ", i + 1);
        for (j = 0; j < GRID; j++) {
            printf(" %c ", board[i][j]);
        }
        printf("\n");
    }
}

void coord_cpy(coord *dst, coord *src) {
    dst->x = src->x;
    dst->y = src->y;
}

int is_empty_square(coord *pos) {
    return board[pos->y][pos->x] == EMPTY;
}

int is_in_board(coord *pos) {
    return (0 <= pos->x && pos->x < GRID) && (0 <= pos->y && pos->y < GRID);
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
    int x_inc, y_inc, cnt;

    coord_cpy(dst, pos);
    x_inc = get_x_incremental_by_direction(dir);
    y_inc = get_y_incremental_by_direction(dir);
    cnt = 0;

    while (1) {
        dst->x += x_inc;
        dst->y += y_inc;
        if (!is_in_board(dst) || is_empty_square(dst)) {
            dst->x = -1;
            dst->y = -1;
            return NULL;
        }
        if (board[dst->y][dst->x] == color) {
            if (cnt == 0) {
                dst->x = -1;
                dst->y = -1;
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

    coord_cpy(&current, from);
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

void test(void) {
    int i;
    direction dir;
    coord pos[] = {
        {3, 2},
        {2, 4},
        {5, 5},
        {3, 1},
        {0, 0}
    };
    coord pair;
    stone color = BLACK;

    print_board();
    for (i = 0; i < sizeof(pos) / sizeof(pos[0]); i++) {
        printf("\n");
        printf("*** put '%c' on %d%c\n\n", color, pos[i].y + 1, "ABCDEFGH"[pos[i].x]);

        for (dir = TOP_LEFT; dir <= LEFT; dir++) {
            if (get_pair_coord(&pair, &pos[i], dir, color) != NULL) {
                put_stone(&pos[i], color);
                flip_stone(&pos[i], &pair);
                printf("found a pair towards '%d'\n", dir);
            }
        }
        printf("\n");
        print_board();
        color = get_opposite_stone(color);
    }
}