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

void generate_moves(FenBoard board)
{
  int source_square, target_square;

  ui64 bitboard, attacks;

  for (int piece = P; piece <= k; piece++)
  {
    bitboard = piece_bitboards[piece];

    if ((piece == P && !board.side) || (piece == p && board.side))
    {
      // white = -8, black = 8
      int pawn_direction = (!board.side) ? -8 : 8;

      while (bitboard)
      {
        source_square = __builtin_ctzll(bitboard);

        target_square = source_square + pawn_direction;

        // prevents off board movement and if target square blocked
        if ((target_square >= 0 && target_square <= 63) && !(get_bit(side_bitboards[both], target_square)))
        {
          // pawn promo
          if ((source_square >= a7 && source_square <= h7 && !board.side) || (source_square >= a2 && source_square <= h2 && board.side))
          {
            printf("pawn promo: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
            printf("pawn promo: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
            printf("pawn promo: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
            printf("pawn promo: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          else
          {
            printf("pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);

            if (((source_square >= a2 && source_square <= h2 && !board.side) ||
                 (source_square >= a7 && source_square <= h7 && board.side)) &&
                !(get_bit(side_bitboards[both], target_square + pawn_direction)))
              {
                printf("2x pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square + pawn_direction]);
              }
          }
        }

        attacks = pawn_attacks_table[!board.side ? white : black][source_square] & side_bitboards[board.side ? white : black];

        while (attacks)
        {
          target_square = __builtin_ctzll(attacks);

          if ((source_square >= a7 && source_square <= h7 && !board.side) || (source_square >= a2 && source_square <= h2 && board.side))
          {
            printf("pawn capture promo: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
            printf("pawn capture promo: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
            printf("pawn capture promo: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
            printf("pawn capture promo: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          else
          {
            printf("pawn capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          pop_bit(attacks, target_square);
        }

        if (board.enpassant != no_sq)
        {
          ui64 enpassant_attack = pawn_attacks_table[!board.side ? white : black][source_square] & (1ULL << board.enpassant);

          if (enpassant_attack)
          {
            int target_enpassant = __builtin_ctzll(enpassant_attack);
            printf("pawn capture enpassant: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_enpassant]);
          }
        }

        pop_bit(bitboard, source_square);
      }
    }

    if ((piece == K && !board.side) || (piece == k && board.side))
    {
      while (bitboard)
      {
        source_square = __builtin_ctzll(bitboard);

        attacks = king_attacks_table[source_square] & ~side_bitboards[!board.side ? white : black];

        while (attacks)
        {
          target_square = __builtin_ctzll(attacks);

          if (!get_bit(side_bitboards[!board.side ? black : white], target_square))
          {
            printf("king move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          else
          {
            printf("king capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          pop_bit(attacks, target_square);
        }

        pop_bit(bitboard, source_square);
      }

      if (board.castle)
      {
        if ((board.castle & wk) && !board.side)
        {
          if (!get_bit(side_bitboards[both], f1) && !get_bit(side_bitboards[both], g1))
          {
            if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black))
              printf("castling move e1g1\n");
          }
        }

        if ((board.castle & wq) && !board.side)
        {
          if (!get_bit(side_bitboards[both], d1) && !get_bit(side_bitboards[both], c1) && !get_bit(side_bitboards[both], b1))
          {
            if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
              printf("castling move e1c1\n");
          }
        }

        if ((board.castle & bk) && board.side)
        {
          if (!get_bit(side_bitboards[both], f8) && !get_bit(side_bitboards[both], g8))
          {
            if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
              printf("castling move e8g8\n");
          }
        }

        if ((board.castle & bq) && board.side)
        {
          if (!get_bit(side_bitboards[both], d8) && !get_bit(side_bitboards[both], c8) && !get_bit(side_bitboards[both], b8))
          {
            if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
              printf("castling move e8c8\n");
          }
        }
      }
    }

    if ((piece == N && !board.side) || (piece == n && board.side))
    {
      while (bitboard)
      {
        source_square = __builtin_ctzll(bitboard);

        attacks = knight_attacks_table[source_square] & ~side_bitboards[!board.side ? white : black];

        while (attacks)
        {
          target_square = __builtin_ctzll(attacks);

          if (!get_bit(side_bitboards[!board.side ? black : white], target_square))
          {
            printf("knight move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          else
          {
            printf("knight capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          pop_bit(attacks, target_square);
        }

        pop_bit(bitboard, source_square);
      }
    }

    if ((piece == B && !board.side) || (piece == b && board.side))
    {
      while (bitboard)
      {
        source_square = __builtin_ctzll(bitboard);

        attacks = get_bishop_attacks(source_square, side_bitboards[both]) & ~side_bitboards[!board.side ? white : black];

        while (attacks)
        {
          target_square = __builtin_ctzll(attacks);

          if (!get_bit(side_bitboards[!board.side ? black : white], target_square))
          {
            printf("bishop move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          else
          {
            printf("bishop capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          pop_bit(attacks, target_square);
        }

        pop_bit(bitboard, source_square);
      }
    }

    if ((piece == R && !board.side) || (piece == r && board.side))
    {
      while (bitboard)
      {
        source_square = __builtin_ctzll(bitboard);

        attacks = get_rook_attacks(source_square, side_bitboards[both]) & ~side_bitboards[!board.side ? white : black];

        while (attacks)
        {
          target_square = __builtin_ctzll(attacks);

          if (!get_bit(side_bitboards[!board.side ? black : white], target_square))
          {
            printf("rook move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          else
          {
            printf("rook capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          pop_bit(attacks, target_square);
        }

        pop_bit(bitboard, source_square);
      }
    }

    if ((piece == Q && !board.side) || (piece == q && board.side))
    {
      while (bitboard)
      {
        source_square = __builtin_ctzll(bitboard);

        attacks = get_queen_attacks(source_square, side_bitboards[both]) & ~side_bitboards[!board.side ? white : black];

        while (attacks)
        {
          target_square = __builtin_ctzll(attacks);

          if (!get_bit(side_bitboards[!board.side ? black : white], target_square))
          {
            printf("queen move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          else
          {
            printf("queen capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          pop_bit(attacks, target_square);
        }

        pop_bit(bitboard, source_square);
      }
    }
  }
}


















