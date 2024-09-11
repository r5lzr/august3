#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"

int main()
{
  leaper_attacks_table();
  slider_attacks_table();

  parse_fen("r3k2r/pPppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBqPPP/R3K2R w KQkq - 0 1 ", &board);
  show_board();

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

    printf("Initial make move from %s: board %d\n>>%s to move\n", board.side ? "white" : "black", move_count, !board.side ? "white" : "black");
    show_board();
//    show_bitboard(side_bitboards[black]);
    getchar();

    restore_board();
    printf("Restore board from: board %d\n>>%s to move\n", move_count, !board.side ? "white" : "black");
    show_board();
//    show_bitboard(side_bitboards[black]);
    getchar();
  }

  return 0;
}
