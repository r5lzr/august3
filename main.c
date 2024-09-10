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

  parse_fen("r3k2r/pPppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ", &board);
  show_board(board);

  moves move_list[1];

  generate_moves(board, move_list);

  for (int move_count = 0; move_count < move_list->count; move_count++)
  {
    int move = move_list->moves[move_count];

    copy_board();

    make_move(move, all_moves);
    printf("Initial make move: %d\n", move_count);
    show_board(board);
    getchar();

    restore_board();
    printf("Restore board from: %d\n", move_count);
    show_board(board);
    getchar();
  }

  return 0;
}
