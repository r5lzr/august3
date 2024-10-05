#ifndef ZOBRIST_H
#define ZOBRIST_H


extern ui64 piece_keys[12][64];
extern ui64 enpassant_keys[64];
extern ui64 castle_keys[16];
extern ui64 side_key;

void init_zobrist_hash();


#endif
