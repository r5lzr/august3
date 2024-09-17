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

  parse_fen(kiwipete_position, &board);
  show_board();

  int start = get_time_ms();

  perft_test(5);

  return 0;
}
