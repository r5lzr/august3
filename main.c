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


//  for (int square = 0; square < 64; square++)
//  {
//    show_bitboard(rook_attack_mask(square, block));
//  }
  show_bitboard(block);
  printf("bitcount: %d\n", count_bits(block));

  return 0;
}
