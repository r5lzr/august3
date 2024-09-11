#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "movegen.h"
#include "attacktable.h"
#include "magicbitboard.h"

/* UCI protocol needs to return lower case
regardless of side for encoding moves */
char promoted_pieces[] = {
  [Q] = 'q',
  [R] = 'r',
  [B] = 'b',
  [N] = 'n',
  [q] = 'q',
  [r] = 'r',
  [b] = 'b',
  [n] = 'n',
};

/*
                           castling   move     in      in
                              right update     binary  decimal

 king & rooks didn't move:     1111 & 1111  =  1111    15

         white king moved:     1111 & 0011  =  0011    3
  white king's rook moved:     1111 & 0111  =  0111    7
 white queen's rook moved:     1111 & 1011  =  1011    11

         black king moved:     1111 & 1100  =  1100    12
  black king's rook moved:     1111 & 1101  =  1101    13
 black queen's rook moved:     1111 & 1110  =  1110    14

*/

const int castling_rights[64] = {
  14, 15, 15, 15, 12, 15, 15, 13,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  11, 15, 15, 15, 3, 15, 15, 7
};

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

void generate_moves(moves *move_list)
{
  move_list->count = 0;

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
            // pawn promotion (promoted char will be lower case)
            add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
            add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
            add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
            add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
          }

          else
          {
            // pawn push
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

            if (((source_square >= a2 && source_square <= h2 && !board.side) ||
                 (source_square >= a7 && source_square <= h7 && board.side)) &&
                !(get_bit(side_bitboards[both], target_square + pawn_direction)))
              {
                // double pawn push
                add_move(move_list, encode_move(source_square, (target_square + pawn_direction), piece, 0, 0, 1, 0, 0));
              }
          }
        }

        attacks = pawn_attacks_table[!board.side ? white : black][source_square] & side_bitboards[board.side ? white : black];

        while (attacks)
        {
          target_square = __builtin_ctzll(attacks);

          if ((source_square >= a7 && source_square <= h7 && !board.side) || (source_square >= a2 && source_square <= h2 && board.side))
          {
            // pawn capture promotion
            add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
            add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
            add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
            add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
          }

          else
          {
            // pawn capture
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
          }

          pop_bit(attacks, target_square);
        }

        if (board.enpassant != no_sq)
        {
          ui64 enpassant_attack = pawn_attacks_table[!board.side ? white : black][source_square] & (1ULL << board.enpassant);

          if (enpassant_attack)
          {
            int target_enpassant = __builtin_ctzll(enpassant_attack);

            // pawn capture enpassant
            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
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
            // king move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          }

          else
          {
            // king capture
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
            {
              // castle white king side
              add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
            }
          }
        }

        if ((board.castle & wq) && !board.side)
        {
          if (!get_bit(side_bitboards[both], d1) && !get_bit(side_bitboards[both], c1) && !get_bit(side_bitboards[both], b1))
          {
            if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
            {
              // castle white queen side
              add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
            }
          }
        }

        if ((board.castle & bk) && board.side)
        {
          if (!get_bit(side_bitboards[both], f8) && !get_bit(side_bitboards[both], g8))
          {
            if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
            {
              // castle black king side
              add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
            }
          }
        }

        if ((board.castle & bq) && board.side)
        {
          if (!get_bit(side_bitboards[both], d8) && !get_bit(side_bitboards[both], c8) && !get_bit(side_bitboards[both], b8))
          {
            if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
            {
              // castle black queen side
              add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
            }
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
            // knight move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          }

          else
          {
            // knight capture
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
            // bishop move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          }

          else
          {
            // bishop capture
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
            // rook move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          }

          else
          {
            // rook capture
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
            // queen move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          }

          else
          {
            //  queen capture
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
          }

          pop_bit(attacks, target_square);
        }

        pop_bit(bitboard, source_square);
      }
    }
  }
}

void show_move(int move)
{
  printf("%s%s%c\n", square_to_coordinates[get_move_source(move)],
          square_to_coordinates[get_move_target(move)],
          promoted_pieces[get_move_promoted(move)]);
}

void show_move_list(moves *move_list)
{
  if (!move_list->count)
  {
    printf("\n*No moves in move list*\n");

    return;
  }

  for (int move_count = 0; move_count < move_list->count; move_count++)
  {
    int move = move_list->moves[move_count];

    printf("\nmove: %s%s%c   piece: %c   capture: %d   double: %d   enpassant: %d   castling: %d\n",
            square_to_coordinates[get_move_source(move)],
            square_to_coordinates[get_move_target(move)],
            promoted_pieces[get_move_promoted(move)],
            ascii_pieces[get_move_piece(move)],
            get_move_capture(move),
            get_move_double(move),
            get_move_enpassant(move),
            get_move_castling(move)
          );
  }
  printf("\nTotal number of moves %d\n", move_list->count);

}

void add_move(moves *move_list, int move)
{
  move_list->moves[move_list->count] = move;

  move_list->count++;
}

int make_move(int move, int move_flag)
{
  if (move_flag == all_moves || get_move_capture(move))
  {
    copy_board();

    int source_square = get_move_source(move);
    int target_square = get_move_target(move);
    int piece = get_move_piece(move);
    int promoted_piece = get_move_promoted(move);
    int capture = get_move_capture(move);
    int double_push = get_move_double(move);
    int enpassant = get_move_enpassant(move);
    int castling = get_move_castling(move);

    pop_bit(piece_bitboards[piece], source_square);
    set_bit(piece_bitboards[piece], target_square);

    if (capture)
    {
      int start_piece, end_piece;

      if (!board.side)
      {
        start_piece = p;
        end_piece = k;
      }

      else
      {
        start_piece = P;
        end_piece = K;
      }

      for (int piece = start_piece; piece <= end_piece; piece++)
      {
        if (get_bit(piece_bitboards[piece], target_square))
        {
          pop_bit(piece_bitboards[piece], target_square);
          break;
        }
      }
    }

    if (promoted_piece)
    {
      pop_bit(piece_bitboards[!board.side ? P : p], target_square);

      set_bit(piece_bitboards[promoted_piece], target_square);
    }

    if (enpassant)
    {
      (!board.side) ? pop_bit(piece_bitboards[p], target_square + 8) : pop_bit(piece_bitboards[P], target_square - 8);
    }

    board.enpassant = no_sq;

    if (double_push)
    {
      (!board.side) ? (board.enpassant = target_square + 8) : (board.enpassant = target_square - 8);
    }

    if (castling)
    {
      switch (target_square)
      {
        // wk
        case (g1):
          pop_bit(piece_bitboards[R], h1);
          set_bit(piece_bitboards[R], f1);
          break;

        // wq
        case (c1):
          pop_bit(piece_bitboards[R], a1);
          set_bit(piece_bitboards[R], d1);
          break;

        // bk
        case (g8):
          pop_bit(piece_bitboards[r], h8);
          set_bit(piece_bitboards[r], f8);
          break;

        // bq
        case (c8):
          pop_bit(piece_bitboards[r], a8);
          set_bit(piece_bitboards[r], d8);
          break;
      }
    }

    board.castle &= castling_rights[source_square];
    board.castle &= castling_rights[target_square];

    // update side_bitboards
    memset(side_bitboards, 0ULL, sizeof(side_bitboards));

    for (int piece = P; piece <= K; piece++)
    {
      side_bitboards[white] |= piece_bitboards[piece];
    }

    for (int piece = p; piece <= k; piece++)
    {
      side_bitboards[black] |= piece_bitboards[piece];
    }

    side_bitboards[both] |= side_bitboards[white];
    side_bitboards[both] |= side_bitboards[black];

    // swap side after move
    board.side ^= 1;

    // if king is in check
    if (is_square_attacked((!board.side) ? __builtin_ctzll(piece_bitboards[k]) : __builtin_ctzll(piece_bitboards[K]), board.side))
    {
      restore_board();

      return 0;
    }

    else
    {
      return 1;
    }
  }

  else
  {
    return 0;
  }
}

















