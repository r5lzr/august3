#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "ttable.h"
#include "zobrist.h"
#include "search.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "util.h"
#include "evaluation.h"

transposition_table ttable[ttable_size];

void restore_ttable()
{
  for (int index = 0; index < ttable_size; index++)
  {
    ttable[index].zobrist_key = 0;
    ttable[index].depth = 0;
    ttable[index].flag = 0;
    ttable[index].score = 0;
  }
}

int probe_ttable(int alpha, int beta, int depth)
{
  transposition_table *hash_entry = &ttable[zobrist_key % ttable_size];

  if (hash_entry->zobrist_key == zobrist_key)
  {
    if (hash_entry->depth >= depth)
    {
      int score = hash_entry->score;

      if (score < -mate_score)
      {
        score += ply;
      }

      if (score > mate_score)
      {
        score -= ply;
      }

      if (hash_entry->flag == hash_flag_exact)
      {
        return score;
      }

      if ((hash_entry->flag == hash_flag_alpha) && (score <= alpha))
      {
        return alpha;
      }

      if ((hash_entry->flag == hash_flag_beta) && (score >= beta))
      {
        return beta;
      }
    }
  }

  return no_hash_entry;
}

void record_ttable(int score, int depth, int flag)
{
  transposition_table *hash_entry = &ttable[zobrist_key % ttable_size];

  if (score < -mate_score)
  {
    score -= ply;
  }

  if (score > mate_score)
  {
    score += ply;
  }

  hash_entry->zobrist_key = zobrist_key;
  hash_entry->depth = depth;
  hash_entry->flag = flag;
  hash_entry->score = score;
}













