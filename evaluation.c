#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "evaluation.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "util.h"

int material_score[12] = {
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

      pop_bit(bitboard, square);
    }
  }

  relative_score = (!board.side) ? score : -score;

  return relative_score;
}
















