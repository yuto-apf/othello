#include "othello.h"
#include <stdio.h>
#include <stdlib.h>

stone board[SIDE_LEN][SIDE_LEN];

void play(void) {
    coord pos, pair;
    direction dir;
    stone s = BLACK;
    int passed = 0, move = 0, stone_cnt = 4;

    init_board();

    while (stone_cnt < SIDE_LEN * SIDE_LEN) {
        printf("\n*** Move %d:\n", ++move);
        printf(" - Your stone: %s ('%c')\n", (s == BLACK) ? "BLACK" : "WHITE", s);

        if (!set_place_able(s)) {
            if (passed) break;
            passed = 1;
            printf("\nThere is no square to put your stone.\n");
            printf("Passed.\n");
        } else {
            printf("\n - Current board:\n\n");
            print_board();
            printf("\n - Point:\n");
            printf("    * BLACK: %d\n",   calc_point(BLACK));
            printf("    * WHITE: %d\n\n", calc_point(WHITE));

            printf(" Q. Where do you place a stone?\n");
            while (1) {
                printf("  >> ");
                if (get_coord(&pos) && is_place_able_square(&pos)) break;
            }

            for (dir = TOP_LEFT; dir <= LEFT; dir++) {
                if (has_pair_stone(&pos, dir, s, &pair)) {
                    put_stone(&pos, s);
                    flip_stone(&pos, &pair);
                }
            }

            passed = 0;
            stone_cnt++;
        }

        reset_place_able();
        s = get_opposite_stone(s);
    }
}

void init_board(void) {
    int x, y;

    for (y = 0; y < SIDE_LEN; y++) {
        for (x = 0; x < SIDE_LEN; x++) {
            board[y][x] = EMPTY;
        }
    }
    board[3][3] = board[4][4] = WHITE;
    board[3][4] = board[4][3] = BLACK;
}

void print_board(void) {
    int x, y;

    printf("         ");
    for (x = 0; x < SIDE_LEN; x++) {
        printf(" %c ", 'A' + x);
    }
    printf("\n");
    for (y = 0; y < SIDE_LEN; y++) {
        printf("       %d ", y + 1);
        for (x = 0; x < SIDE_LEN; x++) {
            printf(" %c ", board[y][x]);
        }
        printf("\n");
    }
}

int get_coord(coord *dst) {
    char s[4];

    if (fgets(s, 4, stdin) == NULL) return 0;
    s[1] = to_upper(s[1]);

    if (('1' <= s[0] && s[0] <= '8') && ('A' <= s[1] && s[1] <= 'H')) {
        dst->x = s[1] - 'A';
        dst->y = (s[0] - '0') - 1;
        return 1;
    } else {
        return 0;
    }
}

int calc_point(stone s) {
    int x, y, point = 0;

    for (y = 0; y < SIDE_LEN; y++) {
        for (x = 0; x < SIDE_LEN; x++) {
            if (board[y][x] == s) {
                point++;
            }
        }
    }

    return point;
}

void put_stone(coord *pos, stone s) {
    board[pos->y][pos->x] = s;
}

void flip_stone(coord *from, coord *to) {
    int dis, _x, _y;
    direction dir;
    coord curr_pos;
    stone s;

    coord_copy(&curr_pos, from);
    dis = get_distance(from, to);
    dir = get_direction(from, to);
    _x = get_x_displacement(dir);
    _y = get_y_displacement(dir);
    s = board[from->y][from->x];

    while (dis--) {
        curr_pos.x += _x;
        curr_pos.y += _y;
        board[curr_pos.y][curr_pos.x] = s;
    }
}

int set_place_able(stone s) {
    int replaced, x, y;
    coord pos;
    direction dir;

    replaced = 0;
    for (y = 0; y < SIDE_LEN; y++) {
        for (x = 0; x < SIDE_LEN; x++) {
            pos.x = x; pos.y = y;
            if (!is_empty_square(&pos)) continue;
            for (dir = TOP_LEFT; dir <= LEFT; dir++) {
                if (has_pair_stone(&pos, dir, s, NULL)) {
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

    for (y = 0; y < SIDE_LEN; y++) {
        for (x = 0; x < SIDE_LEN; x++) {
            pos.x = x; pos.y = y;
            if (is_place_able_square(&pos)) {
                board[pos.y][pos.x] = EMPTY;
            }
        }
    }    
}

int has_pair_stone(coord *pos, direction dir, stone s, coord *pair_pos) {
    int _x, _y, cnt = 0;
    coord p = *pos;
    
    _x = get_x_displacement(dir);
    _y = get_y_displacement(dir);

    while (1) {
        p.x += _x; p.y += _y;
        if (!is_in_board(&p) || is_empty_square(&p)) {
            coord_copy(pair_pos, NULL);
            return 0;
        }
        if (board[p.y][p.x] == s) {
            coord_copy(pair_pos, (cnt > 0) ? &p : NULL);
            return cnt > 0;
        }
        cnt++;
    }
}

stone get_opposite_stone(stone color) {
    return (color == WHITE) ? BLACK : WHITE;
}

int get_distance(coord *from, coord *to) {
    int dis_x, dis_y;

    dis_x = abs(to->x - from->x) - 1;
    dis_y = abs(to->y - from->y) - 1;

    return (dis_x > dis_y) ? dis_x : dis_y;
}

direction get_direction(coord *from, coord *to) {
    int x, y;
    x = to->x - from->x;
    y = to->y - from->y;

    if      (x < 0  && y < 0)  return TOP_LEFT;
    else if (x == 0 && y < 0)  return TOP;
    else if (x > 0  && y < 0)  return TOP_RIGHT;
    else if (x > 0  && y == 0) return RIGHT;
    else if (x > 0  && y > 0)  return BOTTOM_RIGHT;
    else if (x == 0 && y > 0)  return BOTTOM;
    else if (x < 0  && y > 0)  return BOTTOM_LEFT;
    else                       return LEFT;
}

int get_x_displacement(direction dir) {
    if      (TOP_RIGHT <= dir && dir <= BOTTOM_RIGHT)              return 1;
    else if (dir == BOTTOM_LEFT || dir == LEFT || dir == TOP_LEFT) return -1;
    else                                                           return 0;
}

int get_y_displacement(direction dir) {
    if      (TOP_LEFT <= dir && dir <= TOP_RIGHT)       return -1;
    else if (BOTTOM_RIGHT <= dir && dir <= BOTTOM_LEFT) return 1;
    else                                                return 0;
}

int is_empty_square(coord *pos) {
    return board[pos->y][pos->x] == EMPTY || board[pos->y][pos->x] == PLACE_ABLE;
}

int is_place_able_square(coord *pos) {
    return board[pos->y][pos->x] == PLACE_ABLE;
}

int is_in_board(coord *pos) {
    return (0 <= pos->x && pos->x < SIDE_LEN) && (0 <= pos->y && pos->y < SIDE_LEN);
}

char to_upper(char c) {
    return ('a' <= c && c <= 'z') ? c - ('a' - 'A') : c;
}

void coord_copy(coord *dst, coord *src) {
    if (dst == NULL) return;
    if (src == NULL) {
        dst = NULL;
        return;
    }

    dst->x = src->x;
    dst->y = src->y;
}