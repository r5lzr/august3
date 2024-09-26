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

/*
    (Victims) Pawn Knight Bishop   Rook  Queen   King
  (Attackers)
        Pawn   105    205    305    405    505    605
      Knight   104    204    304    404    504    604
      Bishop   103    203    303    403    503    603
        Rook   102    202    302    402    502    602
       Queen   101    201    301    401    501    601
        King   100    200    300    400    500    600
*/

// most viable victim, less valuable attacker [attacker][victim]
int mvv_lva[12][12] = {
  105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
  104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
  103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
  102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
  101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
  100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

  105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
  104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
  103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
  102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
  101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
  100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600
};

int killer_moves[2][64];

int history_moves[12][64];

void show_move_scores(moves *move_list)
{
  printf("\n");

  for (int count = 0; count < move_list->count; count++)
  {
    printf("move: ");
    show_move(move_list->moves[count]);
    printf("score: %d\n", score_move(move_list->moves[count]));

  }
}

int compare_scores(const void *x_void, const void *y_void)
{
  int score_x = score_move(*(int*)x_void);
  int score_y = score_move(*(int*)y_void);

  return score_y - score_x;
}

void sort_move(moves *move_list)
{
  qsort(move_list->moves, move_list->count, sizeof(int), compare_scores);
}

int score_move(int move)
{
  if (get_move_capture(move))
  {
    int target_piece = P;

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
      if (get_bit(piece_bitboards[piece], get_move_target(move)))
      {
        target_piece = piece;
        break;
      }
    }

    return mvv_lva[get_move_piece(move)][target_piece] + 10000;
  }

  else
  {
    if (killer_moves[0][ply] == move)
    {
      return 9000;
    }

    else if (killer_moves[1][ply] == move)
    {
      return 8000;
    }

    else
    {
      return history_moves[get_move_piece(move)][get_move_target(move)];
    }
  }

  return 0;
}

int quiescence(int alpha, int beta)
{
  nodes++;

  int evaluation = evaluate_pieces();

  if (evaluation >= beta)
  {
    return beta;
  }

  if (evaluation > alpha)
  {
    alpha = evaluation;
  }

  moves move_list[1];

  generate_moves(move_list);

  sort_move(move_list);

  for (int count = 0; count < move_list->count; count++)
  {
    copy_board();

    ply++;

    if (make_move(move_list->moves[count], only_captures) == 0)
    {
      ply--;

      continue;
    }

    int score = -quiescence(-beta, -alpha);

    ply--;

    restore_board();

    if (score >= beta)
    {
      return beta;
    }

    if (score > alpha)
    {
      alpha = score;
    }
  }

  return alpha;
}

int negamax(int alpha, int beta, int depth)
{
  if (depth == 0)
  {
    return quiescence(alpha, beta);
  }

  nodes++;

  int in_check = is_square_attacked((!board.side) ? __builtin_ctzll(piece_bitboards[K]) : __builtin_ctzll(piece_bitboards[k]), board.side ^ 1);

  if (in_check)
  {
    depth++;
  }

  int legal_moves = 0;

  int best_move_currently;

  int old_alpha = alpha;

  moves move_list[1];

  generate_moves(move_list);

  sort_move(move_list);

  for (int count = 0; count < move_list->count; count++)
  {
    copy_board();

    ply++;

    if (make_move(move_list->moves[count], all_moves) == 0)
    {
      ply--;

      continue;
    }

    legal_moves++;

    int score = -negamax(-beta, -alpha, depth - 1);

    ply--;

    restore_board();

    if (score >= beta)
    {
      if (get_move_capture(move_list->moves[count]) == 0)
      {
        killer_moves[1][ply] = killer_moves[0][ply];
        killer_moves[0][ply] = move_list->moves[count];
      }

      return beta;
    }

    if (score > alpha)
    {
      if (get_move_capture(move_list->moves[count]) == 0)
      {
        history_moves[get_move_piece(move_list->moves[count])][get_move_target(move_list->moves[count])] += depth;
      }

      alpha = score;

      if (ply == 0)
      {
        best_move_currently = move_list->moves[count];
      }
    }
  }

  if (legal_moves == 0)
  {
    if (in_check)
    {
      return -49000 + ply;
    }

    else
    {
      return 0;
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

  printf("info score cp %d depth %d nodes %ld\n", score, depth, nodes);
  printf("bestmove ");
  show_move(best_move);
}











