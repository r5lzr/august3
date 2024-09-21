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

int evaluate_pieces()
{
  int score = 0;

  ui64 bitboard;

  int square, relative_score;

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
          break;
        case N:
          score += knight_score[square];
          break;
        case B:
          score += bishop_score[square];
          break;
        case R:
          score += rook_score[square];
          break;
//        case Q:
//          score += queen_score[square];
//          break;
        case K:
          score += king_score[square];
          break;

        case p:
          score -= pawn_score[mirror_score[square]];
          break;
        case n:
          score -= knight_score[mirror_score[square]];
          break;
        case b:
          score -= bishop_score[mirror_score[square]];
          break;
        case r:
          score -= rook_score[mirror_score[square]];
          break;
//        case q:
//          score -= queen_score[mirror_score[square]];
//          break;
        case k:
          score -= king_score[mirror_score[square]];
          break;
      }

      pop_bit(bitboard, square);
    }
  }

  relative_score = (!board.side) ? score : -score;

  return relative_score;
}
















