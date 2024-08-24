#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

UInt64 bishop_key_mask(int square);

int main()
{
  leaper_attacks_table();

  for (int square = 0; square < 64; square++)
  {
    show_bitboard(bishop_key_mask(square));
  }

  return 0;
}
