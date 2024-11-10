#ifndef SEARCH_H
#define SEARCH_H


#include "movegen.h"

#define max_ply 64
#define score_window 60

#define infinity 50000
#define mate_value 49000
#define mate_score 48000

extern int ply;
extern int mv_lv[12][12];
extern int killer_moves[2][max_ply];
extern int history_moves[12][64];
extern int pvar_length[max_ply];
extern int pvar_table[max_ply][max_ply];
extern int follow_pvar;
extern int score_pvar;
extern const int full_depth_moves;
extern const int lmr_reduction_limit;
extern const int null_reduction_limit;

extern ui64 repetition_table[1000];
extern int repetition_index;

int repetition_detection();
void pvar_scoring(moves *move_list);
void show_move_scores(moves *move_list);
void sort_move(moves *move_list);
int score_move(int move);
int quiescence(int alpha, int beta);
int negamax(int alpha, int beta, int depth);
void search_position(int depth);


#endif

