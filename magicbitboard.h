#ifndef MAGIC_BITBOARD_H
#define MAGIC_BITBOARD_H


enum {rook, bishop};

// shows relevant occupancy bits relative to board position, reduces bit count calls
extern const int bishop_relevant_bits[64];
extern const int rook_relevant_bits[64];

extern ui64 key_mask_table[4096];
extern ui64 sliding_attack_table[4096];
extern ui64 used_attacks[4096];

extern ui64 rook_magic_numbers[64];
extern ui64 bishop_magic_numbers[64];

extern ui64 bishop_occupancy_table[64];
extern ui64 rook_occupancy_table[64];
extern ui64 bishop_attacks_table[64][512];
extern ui64 rook_attacks_table[64][4096];

ui64 bishop_occupancy_mask(int square);
ui64 rook_occupancy_mask(int square);
ui64 bishop_attack_mask(int square, ui64 block);
ui64 rook_attack_mask(int square, ui64 block);
int count_bits(ui64 block);
ui64 key_mask(int index, int relevant_bits, ui64 occ_mask);
ui64 random_ui64();
ui64 magic_number_candidate();
ui64 find_magic_number(int square, int relevant_bits, int sliding_piece);
void init_magic_numbers();
void init_slider_attacks(int sliding_piece);
ui64 get_bishop_attacks(int square, ui64 occupancy);
ui64 get_rook_attacks(int square, ui64 occupancy);
ui64 get_queen_attacks(int square, ui64 occupancy);
void slider_attacks_table();


#endif
