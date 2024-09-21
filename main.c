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

int main()
{
  leaper_attacks_table();
  slider_attacks_table();

//  uci_loop();

  parse_fen("rnbqkbnr/pppppppp/8/8/4PP2/8/PPPPQ1PP/RNB1KBNR w KQkq - 0 1 ", &board);
  show_board();
  printf("score: %d\n", evaluate_pieces());

  return 0;
}
