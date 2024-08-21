#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

U64 pawn_attacks_table(int side, int square);

int main()
{
  show_bitboard(pawn_attacks_table(white, e1));

  return 0;
}
