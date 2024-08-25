#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

UInt64 rook_attack_mask(int square, UInt64 block);

int main()
{
  leaper_attacks_table();

  UInt64 block = 0ULL;
  set_bit(block, e4);
  set_bit(block, a4);
  set_bit(block, b4);
  set_bit(block, g3);

//  printf("bitcount: %d\n", count_bits(block));

//  for (int square = 0; square < 64; square++)
//  {
//    show_bitboard(rook_attack_mask(square, block));
//  }

//  UInt64 test = 0ULL;
//  set_bit(test, get_ls1b_index(block));

//  show_bitboard(test);
//  printf("%d\n", get_ls1b_index(block));


  printf("%d\n", __builtin_ctzll(block));

  return 0;
}
