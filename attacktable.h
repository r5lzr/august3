#ifndef ATTACK_TABLE_H
#define ATTACK_TABLE_H


extern const UInt64 overflow_A;
extern const UInt64 overflow_H;
extern const UInt64 overflow_AB;
extern const UInt64 overflow_HG;

extern UInt64 pawn_attacks_table[2][64];
extern UInt64 knight_attacks_table[64];
extern UInt64 king_attacks_table[64];

UInt64 pawn_attack_mask(int side, int square);
UInt64 knight_attack_mask(int square);
UInt64 king_attack_mask(int square);
void leaper_attacks_table();


#endif
