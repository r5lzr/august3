#ifndef TTABLE_H
#define TTABLE_H


#define ttable_size 0x400000

#define hash_flag_exact 0
#define hash_flag_alpha 1
#define hash_flag_beta 2

typedef struct {
  ui64 zobrist_key;
  int depth;
  int flag;
  int score;
} transposition_table;
transposition_table ttable[ttable_size];

void restore_ttable();


#endif
