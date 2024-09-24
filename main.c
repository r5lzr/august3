#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "perft.h"
#include "util.h"
#include "uci.h"
#include "evaluation.h"
#include "search.h"

int main()
{
  leaper_attacks_table();
  slider_attacks_table();

//  uci_loop();

  parse_fen("r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ", &board);
  show_board();
//  search_position(3);

  printf("move score: %d\n", mvv_lva[k][P]);

  return 0;
}
