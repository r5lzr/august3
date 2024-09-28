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

/*
                            (Target)
              Pawn Knight Bishop   Rook  Queen   King
  (Attacker)
        Pawn   105    205    305    405    505    605
      Knight   104    204    304    404    504    604
      Bishop   103    203    303    403    503    603
        Rook   102    202    302    402    502    602
       Queen   101    201    301    401    501    601
        King   100    200    300    400    500    600
*/

// most valuable (target), least valuable (attacker) [attacker][target]
int mv_lv[12][12] = {
  {105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605},
  {104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604},
  {103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603},
  {102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602},
  {101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601},
  {100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600},
  {105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605},
  {104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604},
  {103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603},
  {102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602},
  {101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601},
  {100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600}
};

// [id][max_ply]
int killer_moves[2][max_ply];

// [piece][square]
int history_moves[12][64];

// principle variation
int pvar_length[max_ply];

int pvar_table[max_ply][max_ply];

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
  int move_scores[move_list->count];

  for (int count = 0; count < move_list->count; count++)
  {
    move_scores[count] = score_move(move_list->moves[count]);
  }

  for (int current_move = 0; current_move < move_list->count; current_move++)
  {
    for (int next_move = current_move + 1; next_move < move_list->count; next_move++)
    {
      if (move_scores[current_move] < move_scores[next_move])
      {
        int temp_score = move_scores[current_move];
        move_scores[current_move] = move_scores[next_move];
        move_scores[next_move] = temp_score;

        int temp_move = move_list->moves[current_move];
        move_list->moves[current_move] = move_list->moves[next_move];
        move_list->moves[next_move] = temp_move;
      }
    }
  }
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

    return mv_lv[get_move_piece(move)][target_piece] + 10000;
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
  pvar_length[ply] = ply;

  if (depth == 0)
  {
    return quiescence(alpha, beta);
  }

  if (ply > max_ply - 1)
  {
    return evaluate_pieces();
  }

  nodes++;

  int in_check = is_square_attacked((!board.side) ? __builtin_ctzll(piece_bitboards[K]) : __builtin_ctzll(piece_bitboards[k]), board.side ^ 1);

  if (in_check)
  {
    depth++;
  }

  int legal_moves = 0;

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

      pvar_table[ply][ply] = move_list->moves[count];

      for (int next_ply = ply + 1; next_ply < pvar_length[ply + 1]; next_ply++)
      {
        pvar_table[ply][next_ply] = pvar_table[ply + 1][next_ply];
      }

      pvar_length[ply] = pvar_length[ply + 1];
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

  return alpha;
}

void search_position(int depth)
{
  memset(killer_moves, 0, sizeof(killer_moves));
  memset(history_moves, 0, sizeof(history_moves));
  memset(pvar_table, 0, sizeof(pvar_table));
  memset(pvar_length, 0, sizeof(pvar_length));

  for (int current_depth = 1; current_depth <= depth; current_depth++)
  {
    nodes = 0;

    int score = negamax(-50000, 50000, current_depth);

    printf("info score cp %d depth %d nodes %ld pv ", score, current_depth, nodes);

    for (int count = 0; count < pvar_length[0]; count++)
    {
      show_move(pvar_table[0][count]);
      printf(" ");
    }

    printf("\n");
  }

  printf("bestmove ");
  show_move(pvar_table[0][0]);
  printf("\n");
}











