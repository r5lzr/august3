#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "evaluation.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "util.h"

const int material_score[12] = {
  100, // white pawn
  300, // white knight
  350, // white bishop
  500, // white rook
  1000, // white queen
  10000, // white king
  -100, // black pawn
  -300, // black knight
  -350, // black bishop
  -500, // black rook
  -1000, // black queen
  -10000, // black king
};

const int pawn_score[64] =
{
  90,  90,  90,  90,  90,  90,  90,  90,
  30,  30,  30,  40,  40,  30,  30,  30,
  20,  20,  20,  30,  30,  30,  20,  20,
  10,  10,  10,  20,  20,  10,  10,  10,
   5,   5,  10,  20,  20,   5,   5,   5,
   0,   0,   0,   5,   5,   0,   0,   0,
   0,   0,   0, -10, -10,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0
};

const int knight_score[64] =
{
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5,   0,   0,  10,  10,   0,   0,  -5,
  -5,   5,  20,  20,  20,  20,   5,  -5,
  -5,  10,  20,  30,  30,  20,  10,  -5,
  -5,  10,  20,  30,  30,  20,  10,  -5,
  -5,   5,  20,  10,  10,  20,   5,  -5,
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5, -10,   0,   0,   0,   0, -10,  -5
};

const int bishop_score[64] =
{
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,  20,   0,  10,  10,   0,  20,   0,
  0,   0,  10,  20,  20,  10,   0,   0,
  0,   0,  10,  20,  20,  10,   0,   0,
  0,  10,   0,   0,   0,   0,  10,   0,
  0,  30,   0,   0,   0,   0,  30,   0,
  0,   0, -10,   0,   0, -10,   0,   0
};

const int rook_score[64] =
{
  50,  50,  50,  50,  50,  50,  50,  50,
  50,  50,  50,  50,  50,  50,  50,  50,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,   0,  20,  20,   0,   0,   0
};

const int king_score[64] =
{
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   5,   5,   5,   5,   0,   0,
  0,   5,   5,  10,  10,   5,   5,   0,
  0,   5,  10,  20,  20,  10,   5,   0,
  0,   5,  10,  20,  20,  10,   5,   0,
  0,   0,   5,  10,  10,   5,   0,   0,
  0,   5,   5,  -5,  -5,   0,   5,   0,
  0,   0,   5,   0, -15,   0,  10,   0
};

const int mirror_score[128] =
{
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

ui64 file_mask_table[64];
ui64 rank_mask_table[64];
ui64 isolated_mask_table[64];
ui64 white_passed_mask_table[64];
ui64 black_passed_mask_table[64];

const int passed_pawn_rank[64] =
{
  7, 7, 7, 7, 7, 7, 7, 7,
  6, 6, 6, 6, 6, 6, 6, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  4, 4, 4, 4, 4, 4, 4, 4,
  3, 3, 3, 3, 3, 3, 3, 3,
  2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0
};

const int doubled_pawn_negative = -10;
const int isolated_pawn_negative = -10;
const int passed_pawn_positive[8] = {0, 10, 30, 50, 75, 100, 150, 200};

const int semi_file_score = 10;
const int open_file_score = 15;

void init_evaluation_masks()
{
  for (int rank = 0; rank < 8; rank++)
  {
    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      file_mask_table[square] |= file_rank_mask(file, -1);
//      printf("%s\n", square_to_coordinates[square]);
//      show_bitboard(file_mask_table[square]);
    }
  }

  for (int rank = 0; rank < 8; rank++)
  {
    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      rank_mask_table[square] |= file_rank_mask(-1, rank);
//      printf("%s\n", square_to_coordinates[square]);
//      show_bitboard(rank_mask_table[square]);
    }
  }

  for (int rank = 0; rank < 8; rank++)
  {
    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      isolated_mask_table[square] |= file_rank_mask(file - 1, -1);
      isolated_mask_table[square] |= file_rank_mask(file + 1, -1);
//      printf("%s\n", square_to_coordinates[square]);
//      show_bitboard(isolated_mask_table[square]);
    }
  }

  for (int rank = 0; rank < 8; rank++)
  {
    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      white_passed_mask_table[square] |= file_rank_mask(file - 1, -1);
      white_passed_mask_table[square] |= file_rank_mask(file, -1);
      white_passed_mask_table[square] |= file_rank_mask(file + 1, -1);

      for (int i = 0; i < (8 - rank); i++)
      {
        white_passed_mask_table[square] &= ~rank_mask_table[(7 - i) * 8 + file];
      }

//      printf("%s\n", square_to_coordinates[square]);
//      show_bitboard(white_passed_mask_table[square]);
    }
  }

  for (int rank = 0; rank < 8; rank++)
  {
    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      black_passed_mask_table[square] |= file_rank_mask(file - 1, -1);
      black_passed_mask_table[square] |= file_rank_mask(file, -1);
      black_passed_mask_table[square] |= file_rank_mask(file + 1, -1);

      for (int i = 0; i < (rank + 1); i++)
      {
        black_passed_mask_table[square] &= ~rank_mask_table[i * 8 + file];
      }

//      printf("%s\n", square_to_coordinates[square]);
//      show_bitboard(black_passed_mask_table[square]);
    }
  }
}

ui64 file_rank_mask(int file_number, int rank_number)
{
  ui64 bitmask = 0ULL;

  for (int rank = 0; rank < 8; rank++)
  {
    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      if (file_number != -1)
      {
        if (file == file_number)
        {
          bitmask |= set_bit(bitmask, square);
        }
      }

      else if (rank_number != -1)
      {
        if (rank == rank_number)
        {
          bitmask |= set_bit(bitmask, square);
        }
      }
    }
  }

  return bitmask;
}

int evaluate_pieces()
{
  int score = 0;

  ui64 bitboard;

  int square, relative_score;

  int doubled_pawns;

  for (int piece = P; piece <= k; piece++)
  {
    bitboard = piece_bitboards[piece];

    while (bitboard)
    {
      square = __builtin_ctzll(bitboard);

      score += material_score[piece];

      switch (piece)
      {
        case P:
          score += pawn_score[square];

          doubled_pawns = count_bits(piece_bitboards[P] & file_mask_table[square]);

          if (doubled_pawns > 1)
          {
            score += doubled_pawns * doubled_pawn_negative;
          }

          if ((piece_bitboards[P] & isolated_mask_table[square]) == 0)
          {
            score += isolated_pawn_negative;
          }

          if ((white_passed_mask_table[square] & piece_bitboards[p]) == 0)
          {
            score += passed_pawn_positive[passed_pawn_rank[square]];
          }

          break;
        case N:
          score += knight_score[square];
          break;
        case B:
          score += bishop_score[square];
          break;
        case R:
          score += rook_score[square];

          if ((piece_bitboards[P] & file_mask_table[square]) == 0)
          {
            score += semi_file_score;
          }

          if (((piece_bitboards[P] | piece_bitboards[p]) & file_mask_table[square]) == 0)
          {
            score += open_file_score;
          }

          break;
//        case Q:
//          score += queen_score[square];
//          break;
        case K:
          score += king_score[square];

          if ((piece_bitboards[P] & file_mask_table[square]) == 0)
          {
            score -= semi_file_score;
          }

          if (((piece_bitboards[P] | piece_bitboards[p]) & file_mask_table[square]) == 0)
          {
            score -= open_file_score;
          }

          break;

        case p:
          score -= pawn_score[mirror_score[square]];

          doubled_pawns = count_bits(piece_bitboards[p] & file_mask_table[square]);

          if (doubled_pawns > 1)
          {
            score -= doubled_pawns * doubled_pawn_negative;
          }

          if ((piece_bitboards[p] & isolated_mask_table[square]) == 0)
          {
            score -= isolated_pawn_negative;
          }

          if ((black_passed_mask_table[square] & piece_bitboards[P]) == 0)
          {
            score -= passed_pawn_positive[passed_pawn_rank[mirror_score[square]]];
          }

          break;
        case n:
          score -= knight_score[mirror_score[square]];
          break;
        case b:
          score -= bishop_score[mirror_score[square]];
          break;
        case r:
          score -= rook_score[mirror_score[square]];

          if ((piece_bitboards[p] & file_mask_table[square]) == 0)
          {
            score -= semi_file_score;
          }

          if (((piece_bitboards[P] | piece_bitboards[p]) & file_mask_table[square]) == 0)
          {
            score -= open_file_score;
          }

          break;
//        case q:
//          score -= queen_score[mirror_score[square]];
//          break;
        case k:
          score -= king_score[mirror_score[square]];

          if ((piece_bitboards[p] & file_mask_table[square]) == 0)
          {
            score += semi_file_score;
          }

          if (((piece_bitboards[P] | piece_bitboards[p]) & file_mask_table[square]) == 0)
          {
            score += open_file_score;
          }

          break;
      }

      pop_bit(bitboard, square);
    }
  }

  relative_score = (!board.side) ? score : -score;

  return relative_score;
}
















