#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "uci.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "util.h"

int parse_move(char *move_string)
{
  moves move_list[1];

  generate_moves(&move_list);

  int source_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;

  int target_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;

  for (int move_count = 0; move_count < move_list->count; move_count++)
  {
    int move = move_list->moves[move_count];

    if (source_square == get_move_source(move) && target_square == get_move_target(move))
    {
      int promoted_piece = get_move_promoted(move);

      if (promoted_piece)
      {
        if ((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q')
        {
          return move;
        }

        else if ((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r')
        {
          return move;
        }

        else if ((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b')
        {
          return move;
        }

        else if ((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n')
        {
          return move;
        }

        continue;
      }

      return move;
    }
  }

  return 0;
}
