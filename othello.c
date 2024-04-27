#include <stdio.h>
#define EMPTY '-'
#define WHITE 'O'
#define BLACK 'X'
#define GRID   8


char board[GRID][GRID];


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