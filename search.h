#ifndef SEARCH_H
#define SEARCH_H


#include "movegen.h"

extern int ply;
extern int best_move;
extern int mvv_lva[12][12];

void show_move_scores(moves *move_list);
int score_move(int move);
int quiescence(int alpha, int beta);
int negamax(int alpha, int beta, int depth);
void search_position(int depth);


#endif

