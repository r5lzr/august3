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

  uci_loop();

  return 0;
}
