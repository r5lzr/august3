#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

U64 pawn_attacks_table(int side, int square);
U64 knight_attacks_table(int square);

int main()
{
  show_bitboard(knight_attacks_table(e4));

  return 0;
}
