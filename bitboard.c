#include <stdio.h>
#include <stdlib.h>

#define U64 unsigned long long

#define get_bit(bitboard, square) (bitboard & (1ULL << square))


void show_bitboard(U64 bitboard)
{
  for (int rank = 0; rank < 8; rank++)
  {
    for (int file = 0; file < 8; file++)
    {

      int square = rank * 8 + file;

      printf(" %d ", get_bit(bitboard, square) ? 1 : 0);

    }

    printf("\n");
  }
}

