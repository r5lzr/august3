#ifndef EVALUATION_H
#define EVALUATION_H


extern const int material_score[12];
extern const int pawn_score[64];
extern const int knight_score[64];
extern const int bishop_score[64];
extern const int rook_score[64];
extern const int king_score[64];
extern const int mirror_score[128];

int evaluate_pieces();


#endif
