#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitboard.h"
#include "perft.h"
#include "movegen.h"

ui64 nodes;

void perft_driver(int depth)
{
  if (depth == 0)
  {
    nodes++;
    return;
  }

  moves move_list[1];

  generate_moves(&move_list);

  for (int move_count = 0; move_count < move_list->count; move_count++)
  {
    int move = move_list->moves[move_count];

    copy_board();

    // if king is check, return 0 -> continue to next board
    if (!make_move(move, all_moves))
    {
      continue;
    }

    perft_driver(depth - 1);

    restore_board();
  }
}
