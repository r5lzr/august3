#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

UInt64 rook_attack_mask(int square, UInt64 block);

int main()
{
  leaper_attacks_table();

  UInt64 block = 0ULL;
  set_bit(block, e4);
  set_bit(block, a2);
  set_bit(block, h6);

  for (int square = 0; square < 64; square++)
  {
    show_bitboard(rook_attack_mask(square, block));
  }

  return 0;
}
