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


//  parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ", &board);
//
//  show_board(board);

  int move = encode_move(e7, e8, P, Q, 0, 1, 1, 0);

  int source_square = get_move_source(move);
  int target_square = get_move_target(move);
  int piece = get_move_piece(move);
  int promoted_piece = get_move_promoted(move);

  printf("source square: %s\n", square_to_coordinates[source_square]);
  printf("target square: %s\n", square_to_coordinates[target_square]);
  printf("piece: %c\n", ascii_pieces[piece]);
  printf("promoted piece: %c\n", ascii_pieces[promoted_piece]);

  printf("capture: %d\n", get_move_capture(move));
  printf("double pawn push: %d\n", get_move_double(move));
  printf("enpassant: %d\n", get_move_enpassant(move));
  printf("castling square: %d\n", get_move_castling(move));

//  generate_moves(board);

//  show_bitboard(piece_bitboards[p]);
//  show_bitboard(pawn_attacks_table[white][e4]);
//  show_bitboard(pawn_attacks_table[white][e4] & piece_bitboards[p]);

//  printf("is d5 attacked by white pawn? %s\n", (pawn_attacks_table[white][e4] & piece_bitboards[p]) ? "y" : "n");
//
//  show_bitboard(side_bitboards[white]);
//  show_bitboard(side_bitboards[black]);
//  show_bitboard(side_bitboards[both]);
//
//  show_bitboard(piece_bitboards[p]);

  return 0;
}
