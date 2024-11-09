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

//  uci_loop();

  parse_fen(initial_position, &board);
  show_board();

//  search_position(10);
//  make_move(pvar_table[0][0], all_moves);
//  search_position(10);

  return 0;
}
