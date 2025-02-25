#include "othello.h"
#include <stdio.h>
#include <stdlib.h>


stone board[SIDE_LEN][SIDE_LEN];


int main(void) {
    int pnt_b, pnt_w;

    printf("*---------------------------------------*\n");
    printf("*              OTHELLO GAME             *\n");
    printf("*---------------------------------------*\n\n");
    printf("- Black stones are displayed as 'X'.\n");
    printf("- White stones are displayed as 'O'.\n");
    printf("- Please input a coordinate. (e.g.) '1a' or '1A'\n\n");
    printf("********* GAME START! *********\n");

    play();

    printf("\n");
    print_board();
    printf("\n");
    printf("********** GAME SET! **********\n\n");

    pnt_b = calc_point(BLACK);
    pnt_w = calc_point(WHITE);
    printf("- Result:\n\n");
    printf("    * BLACK(%c) : %d\n"  , BLACK, pnt_b);
    printf("    * WHITE(%c) : %d\n\n", WHITE, pnt_w);
    printf("- %s Win!!\n", (pnt_b > pnt_w) ? "BLACK" : "WHITE");

    return 0;
}