#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"

int main()
{
  leaper_attacks_table();

  slider_attacks_table();

//  ui64 occupancy = 0ULL;
//  set_bit(occupancy, e6);
//  set_bit(occupancy, f4);
//  set_bit(occupancy, c4);
//
//  show_bitboard(occupancy);
//  show_bitboard(get_queen_attacks(e4, occupancy));


  parse_fen("8/8/8/3p4/8/8/8/8 w - - ", &board);

  show_board(board);
//  show_attacked_squares(white);

  show_bitboard(piece_bitboards[p]);
  show_bitboard(pawn_attacks_table[white][e4]);
  show_bitboard(pawn_attacks_table[white][e4] & piece_bitboards[p]);

  printf("is d5 attacked by white pawn? %s\n", (pawn_attacks_table[white][e4] & piece_bitboards[p]) ? "y" : "n");
//
//  show_bitboard(side_bitboards[white]);
//  show_bitboard(side_bitboards[black]);
//  show_bitboard(side_bitboards[both]);
//
//  show_bitboard(piece_bitboards[p]);

  return 0;
}
