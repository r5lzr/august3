#ifndef EVALUATION_H
#define EVALUATION_H


extern const int material_score[12];
extern const int pawn_score[64];
extern const int knight_score[64];
extern const int bishop_score[64];
extern const int rook_score[64];
extern const int king_score[64];
extern const int mirror_score[128];

extern ui64 file_mask_table[64];
extern ui64 rank_mask_table[64];
extern ui64 isolated_mask_table[64];
extern ui64 white_passed_mask_table[64];
extern ui64 black_passed_mask_table[64];

extern const int passed_pawn_rank[64];
extern const int doubled_pawn_negative;
extern const int isolated_pawn_negative;
extern const int passed_pawn_positive[8];

void init_evaluation_masks();
ui64 file_rank_mask(int file_number, int rank_number);
int evaluate_pieces();


#endif
