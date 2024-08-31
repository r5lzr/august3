#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

int enpassant = no_sq;

void show_bitboard(UInt64 bitboard)
{
  printf("\n");

  for (int rank = 0; rank < 8; rank++)
  {
    printf("  %d ", 8 - rank);

    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      printf(" %d", get_bit(bitboard, square) ? 1 : 0);
    }

    printf("\n");
  }
  printf("\n     a b c d e f g h\n\n");

  printf("     Bitboard: %llud\n\n", bitboard);
}

void show_board()
{
  printf("\n");

  for (int rank = 0; rank < 8; rank++)
  {
    printf("  %d ", 8 - rank);

    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      int piece = -1;

      for (int piece_bitboard = P; piece_bitboard <= k; piece_bitboard++)
      {
        if (get_bit(bitboards[piece_bitboard], square))
          piece = piece_bitboard;
      }

      printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
    }

    printf("\n");
  }

  printf("\n     a b c d e f g h\n\n");

  printf("  side: %s\n", !side ? "white" : "black");

  printf("  enpass: %s\n", (enpassant != no_sq) ? square_to_coordinates[enpassant] : "none");

  printf("  castling: %c%c%c%c\n\n", (castle & wk) ? 'K' : '-', (castle & wq) ? 'Q' : '-', (castle & bk) ? 'k' : '-', (castle & bq) ? 'q' : '-' );
}

