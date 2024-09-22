#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "search.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "perft.h"
#include "util.h"
#include "evaluation.h"

int ply;

int best_move;

int negamax(int alpha, int beta, int depth)
{
  if (depth == 0)
  {
    return evaluate_pieces();
  }

  nodes++;

  int best_move_currently;

  int old_alpha = alpha;

  moves move_list[1];

  generate_moves(move_list);

  for (int count = 0; count < move_list->count; count++)
  {
    copy_board();

    ply++;

    if (make_move(move_list->moves[count], all_moves) == 0)
    {
      ply--;

      continue;
    }

    int score = -negamax(-beta, -alpha, depth - 1);

    ply--;

    restore_board();

    if (score >= beta)
    {
      return beta;
    }

    if (score > alpha)
    {
      alpha = score;

      if (ply == 0)
      {
        best_move_currently = move_list->moves[count];
      }
    }
  }

  if (old_alpha != alpha)
  {
    best_move = best_move_currently;
  }

  return alpha;
}

void search_position(int depth)
{
  int score = negamax(-50000, 50000, depth);

  printf("bestmove ");
  show_move(best_move);
}











