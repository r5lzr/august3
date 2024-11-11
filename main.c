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
#include "zobrist.h"
#include "ttable.h"

int main()
{
  leaper_attacks_table();
  slider_attacks_table();
  init_zobrist_hash();
  restore_ttable();
  init_evaluation_masks();

//  uci_loop();

//  parse_fen("2r3k1/R7/8/1R6/8/8/P4KPP/8 w - - 0 40 ", &board);
//  parse_fen("1r3rk1/R2PR3/1nB2p2/5P2/8/PP5P/2K5/8 w - - 3 55 ", &board);
//  show_board();
//
//  search_position(10);
//  make_move(pvar_table[0][0], all_moves);
//  search_position(10);

  return 0;
}
