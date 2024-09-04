#ifndef ATTACK_TABLE_H
#define ATTACK_TABLE_H


extern const ui64 overflow_A;
extern const ui64 overflow_H;
extern const ui64 overflow_AB;
extern const ui64 overflow_HG;

extern ui64 pawn_attacks_table[2][64];
extern ui64 knight_attacks_table[64];
extern ui64 king_attacks_table[64];

ui64 pawn_attack_mask(int side, int square);
ui64 knight_attack_mask(int square);
ui64 king_attack_mask(int square);
void leaper_attacks_table();


#endif
