#ifndef SEARCH_H
#define SEARCH_H


extern int ply;
extern int best_move;

int quiescence(int alpha, int beta);
int negamax(int alpha, int beta, int depth);
void search_position(int depth);

#endif

