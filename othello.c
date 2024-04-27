#include <stdio.h>
#define FRAME -1
#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define GRID 8


int board[GRID][GRID];


void init_board(void);
void print_board(void);


int main(void) {
    init_board();
    print_board();

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

    for (i = 0; i < GRID; i++) {
        for (j = 0; j < GRID; j++) {
            printf("%4d", board[i][j]);
        }
        printf("\n");
    }
}