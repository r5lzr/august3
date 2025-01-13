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

  uci_loop();

  return 0;
}
