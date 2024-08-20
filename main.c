#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

int main()
{
  U64 bitboard = 0ULL;

  set_bit(bitboard, e4);
  show_bitboard(bitboard);

  return 0;
}
