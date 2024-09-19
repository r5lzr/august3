#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "perft.h"
#include "util.h"
#include "uci.h"

int main()
{
  leaper_attacks_table();
  slider_attacks_table();

  parse_fen(initial_position, &board);
  show_board();

  parse_position("position initpos moves e2e4 e7e6");
  show_board();


  return 0;
}
