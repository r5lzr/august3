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

  parse_fen(kiwipete_position, &board);
  board.enpassant = c6;
  show_board();
//  search_position(3);

  moves move_list[1];

  generate_moves(move_list);

  show_move_scores(move_list);

  return 0;
}
