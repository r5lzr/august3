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

  parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq c6 0 1 ", &board);
  show_board(board);

  copy_board();

  parse_fen(empty_board, &board);
  show_board(board);

  restore_board();

  show_board(board);

  return 0;
}
