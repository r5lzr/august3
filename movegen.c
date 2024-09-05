#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "movegen.h"

int is_square_attacked(int square, int side)
{



  return 0;
}

void show_attacked_squares(int side)
{
  printf("\n");

  for (int rank = 0; rank < 8; rank++)
  {
    printf("  %d ", 8 - rank);

    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      printf(" %d", is_square_attacked(square, side) ? 1 : 0);
    }
    printf("\n");

  }
  printf("\n     a b c d e f g h\n\n");

}
