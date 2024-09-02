#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "attacktable.h"
#include "magicbitboard.h"

int main()
{
  leaper_attacks_table();

  slider_attacks_table();

//  UInt64 occupancy = 0ULL;
//  set_bit(occupancy, e6);
//  set_bit(occupancy, f4);
//  set_bit(occupancy, c4);
//
//  show_bitboard(occupancy);
//  show_bitboard(get_rook_attacks(e4, occupancy));

  parse_fen(tricky_position);

  show_board();

//  printf("%d\n", random_UInt64());

  return 0;
}
