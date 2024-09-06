#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "movegen.h"
#include "attacktable.h"
#include "magicbitboard.h"

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

int is_square_attacked(int square, int side)
{
  // leaper
  if ((!side) && (pawn_attacks_table[black][square] & piece_bitboards[P]))
    return 1;

  if ((side) && (pawn_attacks_table[white][square] & piece_bitboards[p]))
    return 1;

  if (knight_attacks_table[square] & ((!side) ? piece_bitboards[N] : piece_bitboards[n]))
    return 1;

  if (king_attacks_table[square] & ((!side) ? piece_bitboards[K] : piece_bitboards[k]))
    return 1;

  // slider
  if (get_bishop_attacks(square, side_bitboards[both]) & ((!side) ? piece_bitboards[B] : piece_bitboards[b]))
    return 1;

  if (get_rook_attacks(square, side_bitboards[both]) & ((!side) ? piece_bitboards[R] : piece_bitboards[r]))
    return 1;

  if (get_queen_attacks(square, side_bitboards[both]) & ((!side) ? piece_bitboards[Q] : piece_bitboards[q]))
    return 1;

  return 0;
}

void generate_moves(int side)
{
  int source_square, target_square;

  ui64 bitboard, attacks;

  for (int piece = P; piece <= k; piece++)
  {
    bitboard = piece_bitboards[piece];

    if (!side)
    {
      if (piece == P)
      {
        while (bitboard)
        {
          source_square = __builtin_ctzll(bitboard);

          target_square = source_square - 8;

          // prevents off board movement and if target square blocked
          if (!(target_square < 0) && !(get_bit(side_bitboards[both], target_square)))
          {
            // pawn promo
            if (source_square >= a7 && source_square <= h7)
            {
              printf("pawn promo: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promo: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promo: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promo: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
            }

            else
            {
              printf("pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);

              if ((source_square >= a2 && source_square <= h2) && !(get_bit(side_bitboards[both], target_square - 8)))
                printf("2x pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square - 8]);

            }
          }

          pop_bit(bitboard, source_square);
        }
      }
    }

    else
    {

    }



  }
}


















