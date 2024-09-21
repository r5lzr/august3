#ifndef EVALUATION_H
#define EVALUATION_H


const int material_score[12];
const int pawn_score[64];
const int knight_score[64];
const int bishop_score[64];
const int rook_score[64];
const int king_score[64];
const int mirror_score[128];

int evaluate_pieces();


#endif
