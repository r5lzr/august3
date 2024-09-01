#ifndef MAGIC_BITBOARD_H
#define MAGIC_BITBOARD_H


const enum {rook, bishop};

// shows relevant occupancy bits relative to board position, reduces bit count calls
extern const int bishop_relevant_bits[64];
extern const int rook_relevant_bits[64];

extern UInt64 key_mask_table[4096];
extern UInt64 sliding_attack_table[4096];
extern UInt64 used_attacks[4096];

extern UInt64 rook_magic_numbers[64];
extern UInt64 bishop_magic_numbers[64];

extern UInt64 bishop_occupancy_table[64];
extern UInt64 rook_occupancy_table[64];
extern UInt64 bishop_attacks_table[64][512];
extern UInt64 rook_attacks_table[64][4096];

UInt64 bishop_occupancy_mask(int square);
UInt64 rook_occupancy_mask(int square);
UInt64 bishop_attack_mask(int square, UInt64 block);
UInt64 rook_attack_mask(int square, UInt64 block);
int count_bits(UInt64 block);
UInt64 key_mask(int index, int relevant_bits, UInt64 occ_mask);
UInt64 random_UInt64();
UInt64 magic_number_candidate();
UInt64 find_magic_number(int square, int relevant_bits, int sliding_piece);
void init_magic_numbers();
void init_slider_attacks(int sliding_piece);
UInt64 get_bishop_attacks(int square, UInt64 occupancy);
UInt64 get_rook_attacks(int square, UInt64 occupancy);
void slider_attacks_table();

#endif
