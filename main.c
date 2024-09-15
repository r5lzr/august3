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

  parse_fen("r3k2r/pPppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ", &board);
  show_board();

  int start = get_time_ms();

  perft_test(5);

  return 0;
}
