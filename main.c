#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

UInt64 get_bishop_attacks(int square, UInt64 occupancy);
UInt64 get_rook_attacks(int square, UInt64 occupancy);
void init_slider_attacks(int bishop);
void slider_attacks_table();

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

  show_board();

  return 0;
}
