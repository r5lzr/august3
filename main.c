#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "perft.h"
#include "util.h"

int main()
{
  leaper_attacks_table();
  slider_attacks_table();

  parse_fen(initial_position, &board);
  show_board();

  int start = get_time_ms();

  perft_driver(6);

  printf("time taken to execute: %d ms\n", get_time_ms() - start);
  printf("nodes: %ld\n", nodes);

  return 0;
}
